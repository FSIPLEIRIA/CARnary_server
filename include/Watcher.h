//
// Created by carlostojal on 10-09-2023.
//

#ifndef CARNARY_SERVER_WATCHER_H
#define CARNARY_SERVER_WATCHER_H

#include "protocol.h"

namespace carnary::server {

    /*! \brief Watches a critical system. */
    class Watcher {
        
        private:
            /*! \brief The negotiation this watcher refers to. */
            struct negotiation_t* negotiation;

        public:
            Watcher(struct negotiation_t* negotiation);

            /*! \brief Initialize the monitoring socket. */
            void init();
        
    };
}

#endif // CARNARY_SERVER_WATCHER_H

