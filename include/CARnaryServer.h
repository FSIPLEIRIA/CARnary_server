//
// Created by carlostojal on 20-07-2023.
//

#ifndef CARNARY_SERVER_CARNARYSERVER_H
#define CARNARY_SERVER_CARNARYSERVER_H

#include <thread>
#include <mutex>

#define CARNARY_NEGOTIATION_PORT 6666
#define MAX_NEGOTIATION_QUEUE_LEN 20

/*! \brief This class contains the functionality of the CARnary server. */
class CARnaryServer {

    private:
        /*! \brief The socket of the negotiation service. */
        int sockfd;

        /*! \brief The port of the negotiation service. */
        int port;

        /*! \brief The current monitoring port to assign. */
        int currentMonitoringPort = CARNARY_NEGOTIATION_PORT + 1;



        void createNegotiationSocket();

    public:
        CARnaryServer(int port);

        void start();
};


#endif //CARNARY_SERVER_CARNARYSERVER_H
