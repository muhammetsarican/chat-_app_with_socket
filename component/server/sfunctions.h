#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>

#include <vector>
#include <string>

#include "../bothFunctions.h"

using namespace std;
#define SOCKET int

struct ThreadArgs
{
    int clientSocket;
    vector<userDetail> *users;
};

void changeServerPortAddress(serverAdresses *serverAddress)
{
    serverAddress->portAddress = 8800;
    printf("Server:port address changed to: %d\n", serverAddress->portAddress);
}

void createServer(struct sockaddr_in *server, serverAdresses *serverAddress)
{
    printf("Connected on %d port\n", serverAddress->portAddress);
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr(serverAddress->hostAddress);
    server->sin_port = htons(serverAddress->portAddress);
}

void sendMessageToAllClients(vector<userDetail> *userList, userDetail newUser)
{
    string message = newUser.name;
    message += " joined to server!";

    for (auto user : *userList)
    {
        if (send(user.socket, message.c_str(), message.length(), 0) == -1)
        {
            perror("send() failed");
            // Handle send failure if needed
        }
    }
}

void *handleClient(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int clientSocket = threadArgs->clientSocket;
    struct Person person;
    int recv_len;

    vector<userDetail> *users = threadArgs->users;

    userDetail newUser;

    bool isUserExist = false;

    char *userList = vectorToChar(*users);
    // Now send other users list to client
    if (send(clientSocket, userList, strlen(userList), 0) == -1)
    {
        printf("sendto() failed with error code: %d\n", gai_strerror(errno));
    }

    printf("User list sent to client\n");

    while (true)
    {
        // Getting user informations from client
        if ((recv_len = recv(clientSocket, &person, 512, 0)) == -1)
        {
            printf("recv() failed with error code: %d\n", gai_strerror(errno));
            close(clientSocket);
            pthread_exit(NULL);
        }

        printf("Message received from client\n");

        if (isUserExist == false)
        {
            newUser.name = person.name;
            newUser.socket = clientSocket;

            sendMessageToAllClients(users, newUser);

            users->push_back(newUser);

            printf("\nNew user added, Name: %s \nSocket: %d \n", newUser.name, newUser.socket);

            isUserExist = true;
        }

        // Now reply the client with the same data
        if (send(clientSocket, "message received", strlen("message received"), 0) == -1)
        {
            printf("sendto() failed with error code: %d\n", gai_strerror(errno));
        }
        printf("Message send\n");
    }

    close(clientSocket);
    pthread_exit(NULL);
}