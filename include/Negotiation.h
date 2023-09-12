//
// Created by carlostojal on 09-09-2023.
//

#ifndef CARNARY_SERVER_NEGOTIATION_H
#define CARNARY_SERVER_NEGOTIATION_H

#include <sys/types.h>
#include <string>
#include <stdexcept>
#include <netinet/in.h>
#include "protocol.h"

namespace carnary::server {

    /*! \brief Represents a negotiation between the server and a critical system. */
    class Negotiation {
        
        private:
            /*! \brief PID of the system negotiating. */
            pid_t systemPID = -1;

            /*! \brief PID of the watcher. */
            pid_t watcherPID = -1;

            /*! \brief TCP port of the monitoring socket. */
            std::uint8_t monitoringPort;

            /*! \brief Was the monitoring port already set? */
            bool monitoringPortSet = false;

            /*! \brief Minimum heartbeat rate. When exceeded, the server enters emergency state. */
            std::uint16_t minHeartbeatRate;

            /*! \brief Name of the service for debugging purposes. */
            std::string serviceName;

            /*! \brief Negotiation status. */
            negotiation_status_t status = CREATED;

            /*! \brief Client address from the socket. */
            struct sockaddr_in clientAddress;

            /*! \brief Pipe between daemon and watcher. */
            int pipe[2];

        public:
            Negotiation(std::string serviceName, pid_t systemPID, uint16_t minHeartbeatRate);

            /*! \brief Begin the negotiation. */
            void begin(); // TODO
            
            /*! \brief System PID. */
            pid_t getSystemPID() const;

            /*! \brief Watcher PID: */
            pid_t getWatcherPID() const;

            /*! \brief Monitoring TCP port. */
            std::uint8_t getMonitoringPort() const;

            /*! \brief Minimum acceptable heartbeat rate. */
            std::uint16_t getMinHeartbeatRate() const;

            /*! \brief Service name description. (E.g. "planning", "rs_bridge", etc.) */
            std::string getServiceName() const;

            /*! \brief Negotiation status. */
            negotiation_status_t getStatus() const;

            /*! \brief Get the pipe pointer between the daemon and the watcher. */
            int* getPipe() const;

            /*! \brief Get the clients address pointer. */
            struct sockaddr_in* getClientAddress() const;

            void setWatcherPID(pid_t pid);

            /*! \brief Set the monitoring port. Can only be done before beginning the negotiation protocol. */
            void setMonitoringPort(std::uint8_t monitoringPort);

    };
}

#endif // CARNARY_SERVER_NEGOTIATION_H
