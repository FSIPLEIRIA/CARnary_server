//
// Created by carlostojal on 20-07-2023.
//

#ifndef CARNARY_SERVER_PROTOCOL_H
#define CARNARY_SERVER_PROTOCOL_H

/*! \brief Heartbeat value sent over the watcher socket. */
enum heartbeat_t {
    KEEPALIVE,
    PANIC // when a panic is received, all the services should be signaled to terminate. in redundancy, signals are also used.
};

/*! \brief Status of the negotiation. */
enum negotiation_status_t {
    CREATED, // the negotiation was created, but the negotiation was not started yet
    NEGOTIATING, // the negotiation process is occurring
    FAILED_NEGOTIATION, // the negotiation process failed
    OK, // the negotiation is working as expected
    RUNTIME_FAILURE // the negotiation failed during its runtime
};

/*! \brief Daemon failure code (return value). */
enum failure_code_t {
    SUCCESS,
    DAEMON_PROCESS_INIT_FAILURE,
    DAEMON_INIT_FAILURE,
    EMERGENCY
};

#endif //CARNARY_SERVER_PROTOCOL_H
