# CARnary Server

Lart critical systems monitor.

This is a daemon relying on UNIX inter-process communication mechanisms like sockets, pipes and signals to provide a reliable way of monitoring the heartbeat of critical systems. This also provides a single and unified point of entry to the emergency state, in which the Emergency Brake System (EBS) is activated. 

The distributed design allows the usage of containers, as well as running clients on separate processing units, like the EBS watchdog.

