//
// Created by carlostojal on 20-07-2023.
//

#ifndef CARNARY_SERVER_CARNARYSERVER_H
#define CARNARY_SERVER_CARNARYSERVER_H

#include <iostream>
#include <vector>
#include <memory>
#include "Negotiation.h"

namespace carnary::server {
    /*! \brief This class contains the functionality of the CARnary server. */
    class CARnaryServer {

        private:
            /*! \brief Vector of tracked negotiations. */
            std::vector<std::unique_ptr<carnary::server::Negotiation>> negotiations; 

        public:
            CARnaryServer();

            /*! \brief Add a negotiation to begin. */
            void addNegotiation(std::unique_ptr<carnary::server::Negotiation>& negotiation);
    };
}


#endif //CARNARY_SERVER_CARNARYSERVER_H
