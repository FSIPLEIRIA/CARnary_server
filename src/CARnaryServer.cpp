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

    // TODO: kill every system (send a SIGKILL)
    std::cout << "Killing every system... ";
    std::cout << "Done." << std::endl;

    // TODO: actuate the EBS (to be defined)
    std::cout << "Actuating the EBS... ";
    std::cout << "Done." << std::endl;
}

void CARnaryServer::createNegotiationSocket() {
    // TODO
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
    // TODO: create the negotiation socket

    try {
        this->setupSignalHandlers();
    } catch(std::runtime_error& ex) {
        throw ex;
    }
    
    throw std::runtime_error("Not implemented.");
}

void CARnaryServer::destroy() {
    // TODO: destroy the negotiation socket
    throw std::runtime_error("Not implemented.");
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

    // create the watcher in a child process
    pid_t watcher_pid = fork();
    if(watcher_pid > 0) {
        // daemon code

        // set the negotiation watcher PID
        negotiation->setWatcherPID(watcher_pid);

        // add the negotiation
        this->negotiations.push_back(std::move(negotiation));

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

