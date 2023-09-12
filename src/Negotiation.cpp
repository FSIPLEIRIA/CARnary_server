//
// Created by carlostojal on 09-09-2023.
//

#include "Negotiation.h"

using namespace carnary::server;

Negotiation::Negotiation(std::string serviceName, pid_t systemPID, uint16_t minHeartbeatRate) {

    this->serviceName = serviceName;
    this->systemPID = systemPID;
    this->minHeartbeatRate = minHeartbeatRate;

    this->monitoringPortSet = false;

    this->status = CREATED;
}

void Negotiation::begin() {

    if(!(this->monitoringPortSet))
        throw std::runtime_error("Monitoring port was not set!");

    // TODO: start the negotiation protocol
}

pid_t Negotiation::getSystemPID() const {
    return this->systemPID;
}

std::uint8_t Negotiation::getMonitoringPort() const {
    if(!(this->monitoringPortSet))
        throw std::runtime_error("The monitoring port was not set!");

    return this->monitoringPort;
}

std::uint16_t Negotiation::getMinHeartbeatRate() const {
    return this->minHeartbeatRate;
}

std::string Negotiation::getServiceName() const {
    return this->serviceName;
}

negotiation_status_t Negotiation::getStatus() const {
    return this->status;
}

int* Negotiation::getPipe() const {
    return (int*) this->pipe;
}

struct sockaddr_in* Negotiation::getClientAddress() const {
    return (struct sockaddr_in*) &(this->clientAddress);
}

void Negotiation::setWatcherPID(pid_t pid) {
    this->watcherPID = pid;
}

void Negotiation::setMonitoringPort(std::uint8_t monitoringPort) {
    if(this->status != CREATED)
        throw std::runtime_error("Monitoring port can only be set before beginning negotiation!");

    this->monitoringPort = monitoringPort;
    this->monitoringPortSet = true;
}

