//
// Created by carlostojal on 09-09-2023.
//

#ifndef CARNARY_SERVER_NEGOTIATION_H
#define CARNARY_SERVER_NEGOTIATION_H

#include <sys/types.h>
#include <string>

namespace carnary::server {

    /*! \brief Represents a negotiation between the server and a critical system. */
    class Negotiation {
        
        private:
            /*! \brief PID of the system negotiating. */
            pid_t systemPID;

            /*! \brief TCP port of the negotiaing socket. */
            std::uint8_t tcpPort;

            /*! \brief Minimum heartbeat rate. When exceeded, the server enters emergency state. */
            std::uint16_t minHeartbeatRate;

            /*! \brief Name of the service for debugging purposes. */
            std::string serviceName;

        public:
            Negotiation(pid_t systemPID, uint8_t tcpPort, uint16_t minHeartbeatRate);
    };
}

#endif // CARNARY_SERVER_NEGOTIATION_H
