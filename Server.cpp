
#include "Server.h"

Server::Server(int port) throw(const char *) {
    stopRun = false;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw "Socket has failed";
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *) &server, sizeof(server)) < 0)
        throw "Bind has failed!";
    if (listen(fd, CLIENT_LIMIT) < 0)
        throw "Listen has failed!";
}

void Server::start(ClientHandler &ch) throw(const char *) {
    t = new thread([&ch, this]() {
        while (!stopRun) {
            cout << "Waiting for a client" << endl;
            socklen_t clientSize = sizeof(this->client);
            alarm(3);
            int aCLient = accept(fd, (struct sockaddr *) &client, &clientSize);
            if (aCLient < 0) {
                throw "Accept has failed";
            }
            cout << "Client connected" << endl;
            ch.handle(aCLient);
            close(aCLient);
            sleep(2);
        }
        close(fd);
    });
}

void Server::stop() {
    stopRun = true;
    t->join(); // do not delete this!
}

Server::~Server() {
    delete this->t;
}

