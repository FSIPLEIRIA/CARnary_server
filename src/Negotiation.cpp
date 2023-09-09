//
// Created by carlostojal on 09-09-2023.
//

#include "Negotiation.h"

using namespace carnary::server;

Negotiation::Negotiation(pid_t systemPID, uint8_t tcpPort, uint16_t minHeartbeatRate) {

    this->systemPID = systemPID;
    this->tcpPort = tcpPort;
    this->minHeartbeatRate = minHeartbeatRate;
}

