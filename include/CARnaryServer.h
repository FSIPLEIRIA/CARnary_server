//
// Created by carlostojal on 20-07-2023.
//

#ifndef CARNARY_SERVER_CARNARYSERVER_H
#define CARNARY_SERVER_CARNARYSERVER_H

#include <iostream>
#include <vector>
#include <memory>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include "Negotiation.h"

namespace carnary::server {
    /*! \brief This class contains the functionality of the CARnary server. */
    class CARnaryServer {

        private:
            /*! \brief Vector of tracked negotiations. */
            std::vector<std::unique_ptr<carnary::server::Negotiation>> negotiations; 

            /*! \brief Routine to run on system failure (emergency). */
            void emergencyRoutine();

        public:
            CARnaryServer();
            
            /*! \brief Initialize the daemon. Open the negotiation socket. */
            void init();

            /*! \brief Destroy the server. Close the socket and perform cleanups. */
            void destroy();

            /*! \brief Add a negotiation to begin. */
            void addNegotiation(std::unique_ptr<carnary::server::Negotiation> negotiation);
    };
}


#endif //CARNARY_SERVER_CARNARYSERVER_H
