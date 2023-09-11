#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include "CARnaryServer.h"
#include "protocol.h"

#define CARNARY_NEGOTIATION_PORT 6666

using namespace carnary::server;

int main(int argc, char *argv[]) {
    
    (int)argc;
    (char*)argv;

    // the daemon should be a child process of some process like systemd

    pid_t pid = fork();
    if(pid > 0) {
        // parent process, nothing to do
        return 0;
    } else if(pid < 0) {
        // failure creating the child process
        std::cerr << "Error creating the daemon process: " << strerror(errno) << std::endl;
        return DAEMON_PROCESS_INIT_FAILURE;
    }

    // this is the child process (daemon process). continue execution normally

    std::cout << "** CARnary Daemon **" << std::endl;

    CARnaryServer* server = CARnaryServer::getInstance();

    try {
        std::cout << "Initializing the daemon... ";
        server->init();
    } catch(std::runtime_error ex) {
        std::cout << "Error initializing the daemon: " << ex.what() << std::endl;
        return DAEMON_INIT_FAILURE;
    }

    std::cout << "Initialization done." << std::endl;

    while(1) {
        // sleep some time, to do less cycles, having less CPU impact
        sleep(5);
    }

    return 0;
}
