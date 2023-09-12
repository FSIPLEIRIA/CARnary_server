//
// Created by carlostojal on 20-07-2023.
//

#include "CARnaryServer.h"

using namespace carnary::server;

CARnaryServer* CARnaryServer::instance = nullptr;

CARnaryServer::CARnaryServer() {
}

void CARnaryServer::emergencyRoutine() {

    std::cout << "** EMERGENCY MODE ENTERED **" << std::endl;

    // kill every system (send a SIGTERM)
    std::cout << "Killing every system... " << std::endl;
    for(auto& n : this->negotiations) {
        std::cout << "Killing " << n->getServiceName() << "...";
        if(kill(n->getSystemPID(), SIGTERM) < 0) {
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

    // create the socket file descriptor:
    // AF_INET means IPv4
    // SOCK_STREAM means TCP. To use UDP, would use SOCK_DGRAM instead
    // 0 is the protocol value for IP
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // check the socket creation for errors
    if(this->sockfd == -1) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("Error creating the negotiation file descriptor");
    }
    
    // pre-fill the address with zeros
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));

    // fill the address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(DAEMON_TCP_NEGOTIATION_PORT);

    // bind the socket to the address
    if(bind(this->sockfd, (struct sockaddr*) &serverAddr, sizeof(struct sockaddr_in)) < 0) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("Error binding the address to the socket");
    }

    // listen to uncoming clients
    if(listen(this->sockfd, NEGOTIATION_QUEUE_LEN) < 0) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("Error listening for clients");
    }
}

void signalHandler(int signum) {

    CARnaryServer* instance = CARnaryServer::getInstance();

    if(signum == SIGTERM)
        instance->emergencyRoutine();
}

void CARnaryServer::setupSignalHandlers() {

    // initialize the SIGTERM signal handler
    sighandler_t sig = signal(SIGTERM, signalHandler);

    // check signal handler setup failure
    if(sig == SIG_ERR)
        throw std::runtime_error("Error setting up the signal handler.");

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

}

void CARnaryServer::addNegotiation(std::unique_ptr<carnary::server::Negotiation> negotiation) {

    try {
        // begin the negotiation process
        negotiation->begin();
    } catch(std::runtime_error ex) {
        std::cerr << "Trouble beginning the negotiation: " << ex.what() << std::endl;
        // enter emergency state
        return this->emergencyRoutine();
    }

    // create the negotiation transfer pipe
    if(pipe(negotiation->getPipe()) < 0) {
        std::cerr << "Trouble creating the negotiation transfer pipe: " << strerror(errno) << std::endl;
        return this->emergencyRoutine();
    }

    // create the watcher in a child process
    pid_t watcher_pid = fork();
    if(watcher_pid > 0) {
        // daemon code

        // set the negotiation watcher PID
        negotiation->setWatcherPID(watcher_pid);

        // add the negotiation
        {
            std::lock_guard negotiationsLock(this->negotiationsMutex);
            this->negotiations.push_back(std::move(negotiation));
        }

    } else if(watcher_pid == 0) {
        // watcher code

        // TODO
    } else {
        // error creating the watcher process
        std::cerr << "Trouble creating the watcher process: " << strerror(errno) << std::endl;
        // enter emergency state
        return this->emergencyRoutine();
    }

}

