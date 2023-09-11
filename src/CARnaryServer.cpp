//
// Created by carlostojal on 20-07-2023.
//

#include "CARnaryServer.h"

using namespace carnary::server;

CARnaryServer::CARnaryServer() {
}

void CARnaryServer::emergencyRoutine() {

    // TODO: kill every system
    // TODO: actuate the EBS
}

void CARnaryServer::init() {
    // TODO: create the negotiation socket
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

