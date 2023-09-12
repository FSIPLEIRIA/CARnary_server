//
// Created by carlostojal on 10-09-2023.
//

#ifndef CARNARY_SERVER_WATCHER_H
#define CARNARY_SERVER_WATCHER_H

#include <chrono>
#include <csignal>
#include "CARnaryServer.h"
#include "protocol.h"

namespace carnary::server {

    /*! \brief Watches a critical system. */
    class Watcher {
        
        private:
            /*! \brief The negotiation this watcher refers to. */
            struct negotiation_t* negotiation;

            carnary::server::CARnaryServer* instance;

            /*! \brief Socket file descriptor which receives hertbeats. */
            int sockfd = -1;

            /*! \brief Client address. */
            struct sockaddr_in clientAddr;

            /*! \brief The time point in which the last heartbeat was received */
            std::chrono::time_point<std::chrono::high_resolution_clock> lastHeartbeat;

            void enterDaemonEmergency();

        public:
            Watcher(struct negotiation_t* negotiation);

            /*! \brief Initialize the monitoring socket and wait for the client. */
            void init();

    };
}

#endif // CARNARY_SERVER_WATCHER_H

