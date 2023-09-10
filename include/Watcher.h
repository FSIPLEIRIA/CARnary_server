//
// Created by carlostojal on 10-09-2023.
//

#ifndef CARNARY_SERVER_WATCHER_H
#define CARNARY_SERVER_WATCHER_H

#include <memory>
#include "Negotiation.h"

namespace carnary::server {

    /*! \brief Watches a critical system. */
    class Watcher {
        
        private:
            /*! \brief The negotiation this watcher refers to. */
            carnary::server::Negotiation negotiation;

        public:
            Watcher(std::unique_ptr<carnary::server::Negotiation>& negotiation);
        
    };
}

#endif // CARNARY_SERVER_WATCHER_H

