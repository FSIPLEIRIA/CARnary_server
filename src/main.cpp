#include <iostream>
#include "CARnaryServer.h"
#include "protocol.h"

#define CARNARY_NEGOTIATION_PORT 6666

using namespace carnary::server;

int main(int argc, char *argv[]) {
    
    (int)argc;
    (char*)argv;

    std::cout << "** CARnary Daemon **" << std::endl;

    CARnaryServer server = CARnaryServer();

    try {
        std::cout << "Initializing the daemon... ";
        server.init();
    } catch(std::runtime_error ex) {
        std::cout << "Error initializing the daemon: " << ex.what() << std::endl;
        return DAEMON_INIT_FAILURE;
    }

    std::cout << "Initialization done." << std::endl;

    return 0;
}
