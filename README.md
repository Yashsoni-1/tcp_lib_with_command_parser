# TCP_MULTITHREADED_SERVER

Complete multithreaded TCP library/server built in C++ over POSIX.

## OBJECTIVE
1. To manage multiple cliets through multiplexing.
2. To forcefully disconnect the client.
3. To gracefully shutdown the server.
4. To notify the application about new client, disconnection and message received.
5. To handle concurrency using mutex.
6. To apply the TCP demarcation.
7. To build socket programming C++ library over POSIX.

## DESCRIPTION
Created multithreaded TCP library/server in C++ over POSIX. This is a simple tcp server which is made with the tcp_lib(one of my previous project). This server/library can simultaneously accept new client and listen to existing client. This library consists of one database and two threads excluding main. Let us call one thread as CAS(connection accepting service) and other as DRS(data request service). All the connected clients are stored in DBM(central database). All these three entities are managed and controlled by central entity TCP_SERVER_CONTROLLER, let us call it TCS. Thread CAS is fully dedicated to accept the new connection request and notify the TCS about the new client. TCS store these clients in DBM. Thread DRS is fully dedicated to listen to these connected clients. 

## GETTING STARTED
- Just clone the git repo.
- Type make.
- Wooho, the your TCP server is ready.
- I have also provided to client program to test.
- You can also use your own.
- Just type ? to know about all the apis.

## ACKNOWLEDGMENT
- Sir, Abishesk Sagar for guidance and command parser library.

## NEXT STEPS
1. To replace all the new and delete with smart pointers.
2. To implement message processing and sending back to client functionality.
3. To make this server act as client and connect to other service.
4. To implement variable size demarcation.
