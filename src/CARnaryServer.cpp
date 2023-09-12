//
// Created by carlostojal on 20-07-2023.
//

#include "CARnaryServer.h"

using namespace carnary::server;

CARnaryServer* CARnaryServer::instance = nullptr;

CARnaryServer::CARnaryServer() {
}

void negotiationRoutine(int clientfd) {

    // get the daemon instance
    carnary::server::CARnaryServer* daemon = carnary::server::CARnaryServer::getInstance();

    // allocate a negotiation
    struct negotiation_t* negot = (struct negotiation_t*) malloc(sizeof(struct negotiation_t));

    // check allocation error
    if(negot == nullptr) {
        throw std::runtime_error("Error allocating the negotiation!");
    }

    // receive the negotiation: minRate and PID
    if(recv(clientfd, negot, sizeof(struct negotiation_t), MSG_TRUNC | MSG_WAITALL) < 0) {
        throw std::runtime_error("Error receiving client negotiation!");
    }

    // set negotiating status
    negot->status = NEGOTIATING;

    // set daemon PID
    negot->daemonPID = getpid();

    // check the values
    if(negot->systemPID < 0) {
        negot->status = FAILED_NEGOTIATION;
        std::uint8_t response = ERR_PID_NOT_PROVIDED;
        // the system has not provided its PIDs
        // respond with the error
        send(clientfd, &response, sizeof(std::uint8_t), MSG_TRUNC | MSG_WAITALL);
        throw std::runtime_error("Client has not provided its PID!");
    }

    // create the pipe
    if(pipe(negot->daemonWatcherPipe) < 0) {
        negot->status = FAILED_NEGOTIATION;
        throw std::runtime_error("Error creating the daemon-watcher pipe.");
    }

    // initialize the semaphore
    if(sem_init(&(negot->daemonWatcherSemaphore), 0, 0) < 0) {
        negot->status = FAILED_NEGOTIATION;
        throw std::runtime_error("Error initializing the daemon-watcher semaphore.");
    }

    // create a watcher
    // watchers are separate processes - not threads - because processes enable more contingency in case of failure (isolated address spaces, etc.)
    pid_t pid = fork();
    // check fork error
    if(pid < 0) {
        negot->status = FAILED_NEGOTIATION;
        // respond with the error
        std::uint8_t response = ERR_CREATING_WATCHER;
        if(send(clientfd, &response, sizeof(std::uint8_t), MSG_TRUNC | MSG_WAITALL) < 0) {
            throw std::runtime_error("Trouble sending the error to the client.");
        }
        throw std::runtime_error("Trouble creating the watcher process!");

    } else if(pid == 0) {

        // -- WATCHER PROCESS --

        // TODO: offload the watcher execution to its class

        // execution only continues after the daemon consigns a port
        if(sem_wait(&(negot->daemonWatcherSemaphore)) < 0) {
            negot->status = FAILED_NEGOTIATION;
            throw std::runtime_error("Error waiting for the semaphore!");
        }

        // read the updated negotiation, containing the port
        if(read(negot->daemonWatcherPipe[0], negot, sizeof(struct negotiation_t)) < 0) {
            negot->status = FAILED_NEGOTIATION;
            std::uint8_t response = WATCHER_NACK;
            // reply with a NACK
            if(write(negot->daemonWatcherPipe[1], &response, sizeof(std::uint8_t)) < 0) {
                negot->status = FAILED_NEGOTIATION;
                throw std::runtime_error("Error replying to the daemon!");
            }
            throw std::runtime_error("Error receiving data from the daemon through the pipe!");
        }

        // open the monitoring port (TCP socket)
        int watcherSocket = -1;
        try {
            watcherSocket = Utils::createSocket(negot->monitoringPort, TCP_SOCKET);
        } catch(std::runtime_error& ex) {
            negot->status = FAILED_NEGOTIATION;
            throw ex;
        }

        // reply with an ACK
        std::uint8_t response = WATCHER_ACK;
        if(write(negot->daemonWatcherPipe[1], &response, sizeof(std::uint8_t)) < 0) {
            negot->status = FAILED_NEGOTIATION;
            throw std::runtime_error("Error replying to the daemon!");
        }

    } else {

        // -- DAEMON PROCESS --

        negot->watcherPID = pid;

        // the negotiation port is the negotiation port with offset
        negot->monitoringPort = DAEMON_TCP_NEGOTIATION_PORT + daemon->getNegotiations().size() + 1;

        // give the negotiation and the monitoring port to the watcher through the pipe
        if(write(negot->daemonWatcherPipe[1], negot, sizeof(struct negotiation_t)) < 0) {
            negot->status = FAILED_NEGOTIATION;
            throw std::runtime_error("Error communicating with the watcher through the pipe!");
        }

        // signal the watcher to continue (read the updated negotiation)
        if(sem_post(&(negot->daemonWatcherSemaphore)) < 0) {
            negot->status = FAILED_NEGOTIATION;
            throw std::runtime_error("Error signaling the semaphore!");
        }

        std::uint8_t watcherResponse;

        // the watcher responds with ACK/NACK
        if(read(negot->daemonWatcherPipe[0], &watcherResponse, sizeof(std::uint8_t)) < 0) {
            negot->status = FAILED_NEGOTIATION;
            throw std::runtime_error("Error reading watcher response from the pipe!");
        }
        if(watcherResponse != WATCHER_ACK) {
            negot->status = FAILED_NEGOTIATION;
            throw std::runtime_error("The watcher had a problem!");
        }

        // this negotiation is now concluded
        negot->status = NEGOTIATION_OK;

        // close the client socket
        if(close(clientfd) < 0) {
            throw std::runtime_error("Error closing the negotiation file descriptor.");
        }
    }

}

void signalHandler(int signum) {

    CARnaryServer* instance = CARnaryServer::getInstance();

    if(signum == SIGTERM)
        instance->emergencyRoutine();
}

void CARnaryServer::emergencyRoutine() {

    std::cout << "** EMERGENCY MODE ENTERED **" << std::endl;

    // kill every system (send a SIGTERM)
    std::cout << "Killing every system... " << std::endl;
    for(auto& n : this->negotiations) {
        std::cout << "Killing " << n->serviceName << "...";
        if(kill(n->systemPID, SIGTERM) < 0) {
            std::cout << "ERROR." << std::endl;
        } else {
            std::cout << "Killed." << std::endl;
        }

    }
    std::cout << "Done." << std::endl;

    // TODO: actuate the EBS (to be defined)
    std::cout << "Actuating the EBS... ";
    std::cout << "Done." << std::endl;

    // destroy everything
    try {
        this->destroy();
    } catch(std::runtime_error& ex) {
        std::cerr << "Error destroying the daemon: " << strerror(errno) << std::endl;
    }

    // terminate the process
    exit(EMERGENCY);
}

void CARnaryServer::setupNegotiationSocket() {

    std::cout << "Setting up the negotiation socket... ";

    try {
        this->sockfd = carnary::server::Utils::createSocket(DAEMON_TCP_NEGOTIATION_PORT, TCP_SOCKET);
    } catch(std::runtime_error& ex) {
        throw ex;
    }

    // have a separate thread accepting clients - the negotiation thread
    // this way the main thread won't block

    auto negotiationAcceptingRoutine = [&]() {

        struct sockaddr_in tempClientAddr;
        socklen_t clientlen = sizeof(struct sockaddr);

        int clientfd;
        if ((clientfd = accept(sockfd, (struct sockaddr *) &tempClientAddr, &clientlen)) < 0) {
            throw std::runtime_error("Error accepting client");
        }

        // each client is accepted in a separate thread
        std::thread negotiationThread(negotiationRoutine, clientfd);
        negotiationThread.detach();
    };

    // start the thread
    std::thread negotiationThread(negotiationAcceptingRoutine);

    // detach the thread to unblock this one
    // the negotiation thread is dedicated to accepting clients
    negotiationThread.detach();

    std::cout << "Done." << std::endl;

    return;
}

void CARnaryServer::setupSignalHandlers() {

    std::cout << "Setting up the signal handlers... ";

    // initialize the SIGTERM signal handler
    sighandler_t sig = signal(SIGTERM, signalHandler);

    // check signal handler setup failure
    if(sig == SIG_ERR)
        throw std::runtime_error("Error setting up the signal handler.");

    std::cout << "Done." << std::endl;

    return;
}

CARnaryServer* CARnaryServer::getInstance() {
    if(instance == nullptr)
        instance = new CARnaryServer();

    return instance;
}

void CARnaryServer::init() {

    // setup the signal handlers
    try {
        this->setupSignalHandlers();
    } catch(std::runtime_error& ex) {
        throw ex;
    }
    
    // create the negotiation socket
    try {
        this->setupNegotiationSocket();
    } catch(std::runtime_error& ex) {
        throw ex;
    }
}

void CARnaryServer::destroy() {

    // close the negotiation socket
    if(close(this->sockfd) < 0) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("Error closing the negotiation socket");
    }

    for(auto& negot : this->negotiations) {

        // close the pipe reading end
        if(close(negot->daemonWatcherPipe[0]) < 0)
            throw std::runtime_error("Error closing a pipe reading end!");

        // close the pipe writing end
        if(close(negot->daemonWatcherPipe[1]) < 0)
            throw std::runtime_error("Error closing a pipe writing end!");

        // free the negotiation
        free(negot);
    }

}

void CARnaryServer::addNegotiation(struct negotiation_t* negotiation) {

    // lock access to the negotiations vector
    std::lock_guard negotiationsLock(this->negotiationsMutex);

    negotiations.push_back(negotiation);
}

std::vector<struct negotiation_t *> CARnaryServer::getNegotiations() const {
    return this->negotiations;
}
