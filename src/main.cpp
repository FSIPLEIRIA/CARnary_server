#include <iostream>
#include <sys/types.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <arpa/inet.h>
#include "protocol.h"

#define CARNARY_NEGOTIATION_PORT 6666
#define MAX_CONNECTION_QUEUE_LEN 20

int main() {

    // TODO: write error logs on a file

    /*

    // relates IP addresses with negotiations
    std::unordered_map<std::string ,struct negotiation_t> negotiations;
    std::mutex negotiationTableMutex;

    std::uint16_t currentPortNumber = CARNARY_NEGOTIATION_PORT + 1;
    std::mutex currentPortNumberMutex;

    std::cout << "##### CARnary Server #####" << std::endl;

    // initialize a negotiation socket
    std::cout << "Starting the negotiation socket...";

    // create the socket
    int negotiation_fd;
    if((negotiation_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Starting the negotiation socket failed: " << strerror(errno) << std::endl;
        exit(-1);
    }

    // set the options
    int opt = 1;
    if(setsockopt(negotiation_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        std::cerr << "Setting the negotiation socket options failed: " << strerror(errno) << std::endl;
        exit(-2);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(CARNARY_NEGOTIATION_PORT);

    // bind the socket to the port
    if(bind(negotiation_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        std::cerr << "Binding the socket to port failed:" << strerror(errno) << std::endl;
        exit(-3);
    }

    auto negotiationListener = [&] () {

        // listen to incoming connections
        if(listen(negotiation_fd, MAX_CONNECTION_QUEUE_LEN) < 0) {
            std::cerr << "Listening on the socket failed: " << strerror(errno) << std::endl;
            exit(-4);
        }

        std::cout << "Waiting for negotiations..." << std::endl;

        // indefinitely accept new connections
        while(true) {

            // accept the new client
            struct sockaddr_in new_client;
            int client_len = sizeof(new_client);
            if(accept(negotiation_fd, (struct sockaddr*) &new_client, (socklen_t*) &client_len) < 0) {
                std::cerr << "Accepting client failed: " << strerror(errno) << std::endl;
                exit(-5);
            }

            // receive the negotiation
            struct negotiation_t new_negotiation;
            if(recvfrom(negotiation_fd, &new_negotiation, sizeof(new_negotiation), 0, (struct sockaddr*) &new_client,
                        (socklen_t*) &client_len) != sizeof(new_negotiation)) {
                std::cerr << "Receiving negotiation failed: " << strerror(errno) << std::endl;
                exit(-6);
            }

            std::cout << "Negotiation received " << new_negotiation.serviceName << std::endl;

            // add the entry to the negotiation table
            negotiationTableMutex.lock();
            negotiations[inet_ntoa(new_client.sin_addr)] = new_negotiation;
            negotiationTableMutex.unlock();

            // TODO: create a dedicated socket to this system and respond with the port through the negotiation socket
            // finish the negotiation by replying with the consigned monitoring port number
            currentPortNumberMutex.lock();
            if(sendto(negotiation_fd, &currentPortNumber, sizeof(currentPortNumber), 0, (struct sockaddr*) &new_client,
                    client_len) != sizeof(currentPortNumber)) {
                std::cerr << "Finishing the negotiation failed: " << strerror(errno) << std::endl;
                exit(-7);
            }
            currentPortNumber++;
            currentPortNumberMutex.unlock();
        }
    };

    // start a thread to listen to incoming negotiations
    std::thread negotiationThread(negotiationListener);

    // TODO: start a thread to receive keepalive messages

    negotiationThread.join();

    */


    return 0;
}
