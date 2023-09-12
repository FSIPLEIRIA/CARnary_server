//
// Created by carlostojal on 20-07-2023.
//

#ifndef CARNARY_SERVER_CARNARYSERVER_H
#define CARNARY_SERVER_CARNARYSERVER_H

#include <iostream>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <csignal>
#include "Negotiation.h"

#define DAEMON_TCP_NEGOTIATION_PORT 6666

namespace carnary::server {
    /*! \brief This class contains the functionality of the CARnary server. */
    class CARnaryServer {

        private:
            /*! \brief Vector of tracked negotiations. */
            std::vector<std::unique_ptr<carnary::server::Negotiation>> negotiations;

            /*! \brief Negotiation socket file descriptor. */
            int sockfd = -1;

            /*! \brief Server address structure. */
            struct sockaddr_in serverAddr;

            /*! \brief Creates the negotiation socket. */
            void setupNegotiationSocket();

            /*! \brief Sets up the SIGTERM signal handler. */
            void setupSignalHandlers();

            /*! \brief Destroy the server. Close the socket and perform cleanups. */
            void destroy();

        protected:
            CARnaryServer();
    
            /*! \brief Singleton instance. */
            static carnary::server::CARnaryServer* instance;


        public:

            /*! \brief Get singleton instance. */
            static carnary::server::CARnaryServer* getInstance();

            /*! \brief Routine to run on system failure (emergency). */
            void emergencyRoutine();
            
            /*! \brief Initialize the daemon. Open the negotiation socket. */
            void init();

            /*! \brief Add a negotiation to begin. */
            void addNegotiation(std::unique_ptr<carnary::server::Negotiation> negotiation);

            /*! \brief SIGTERM signal handler. Enters emergency mode. */
            friend void signalHandler(int signum);

    };
}


#endif //CARNARY_SERVER_CARNARYSERVER_H
