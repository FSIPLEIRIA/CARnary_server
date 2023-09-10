//
// Created by carlostojal on 20-07-2023.
//

#include "CARnaryServer.h"

using namespace carnary::server;

void CARnaryServer::addNegotiation(std::unique_ptr<carnary::server::Negotiation>& negotiation) {

    try {
        // begin the negotiation process
        negotiation->begin();
    } catch(std::runtime_error ex) {
        std::cerr << "Trouble beginning the negotiation: " << ex.what() << std::endl;
        // TODO: enter emergency state
        return;
    }
}

