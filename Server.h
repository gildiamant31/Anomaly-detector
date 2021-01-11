/*
 * Server.h
 *
 *  Gil Diamant, 314978412.
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <pthread.h>
#include <thread>
#include "CLI.h"

#define CLIENT_LIMIT 3

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
    AnomalyDetectionHandler() {}

    virtual void handle(int clientID) {
        SocketIO sio(clientID);
        CLI cli(&sio);
        cli.start();

    }
};


// implement on Server.cpp
class Server {
    thread *t; // the thread to run the start() method in
    bool stopRun;
    int fd;
    sockaddr_in server;
    sockaddr_in client;

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */
