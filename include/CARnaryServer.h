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
#include <cstdlib>
#include <thread>
#include <mutex>
#include "protocol.h"

// the TCP port to reserve to the negotiation
#define DAEMON_TCP_NEGOTIATION_PORT 6666
// length of the queue of waiting clients 
#define NEGOTIATION_QUEUE_LEN 10

namespace carnary::server {
    /*! \brief This class contains the functionality of the CARnary server. */
    class CARnaryServer {

        private:

            /*! \brief Vector of registered negotiations. */
            std::vector<struct negotiation_t*> negotiations;

            /*! \brief Mutex which controls access to the negotiations vector. */
            std::mutex negotiationsMutex;

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

            /*! \brief Add a negotiation. */
            void addNegotiation(struct negotiation_t* negotiation);

            /*! \brief Registered negotiations getter. */
            std::vector<struct negotiation_t*> getNegotiations() const;

            /*! \brief SIGTERM signal handler. Enters emergency mode. */
            friend void signalHandler(int signum);

            /*! \brief The routine run when a client is accepted. */
            friend void negotiationRoutine(int clientfd);

    };
}


#endif //CARNARY_SERVER_CARNARYSERVER_H
