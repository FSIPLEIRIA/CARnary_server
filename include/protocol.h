//
// Created by carlostojal on 20-07-2023.
//

#ifndef CARNARY_SERVER_PROTOCOL_H
#define CARNARY_SERVER_PROTOCOL_H

#include <sys/types.h>
#include <cstdint>

struct negotiation_t {
    pid_t pid; // the PID of the system, to kill it in some occasion
    std::uint32_t maxWait; // the maximum time in ms the carnary can wait without receiving keep-alive
    std::string serviceName; // "rs_bridge", "control", "planning", etc.
};

enum heartbeat_t {
    KEEPALIVE,
    PANIC // when a panic is received, all the services should be signaled to terminate. in redundancy, signals are also used.
};

#endif //CARNARY_SERVER_PROTOCOL_H
