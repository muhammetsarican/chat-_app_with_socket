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

struct serverPerson
{
    char name[15] = "";
    char messageToWho[15] = "";
    char message[100] = "";
};

struct ThreadArgs
{
    int clientSocket;
    vector<userDetail> *users;
};

void *handleClient(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int clientSocket = threadArgs->clientSocket;
    struct serverPerson person;
    int recv_len;

    vector<userDetail> *users = threadArgs->users;
    printf("user list handleclient: %s\n", users->size());

    userDetail newUser;

    bool isUserExist = false;

    while (true)
    {
/*         // Now send other users list to client
        if (send(clientSocket, users, sizeof(users), 0) == -1)
        {
            printf("sendto() failed with error code: %d\n", gai_strerror(errno));
        }
        printf("User list send\n"); */

        // Now reply the client with the same data
        /*         if (send(clientSocket, "message received", strlen("message received"), 0) == -1)
                {
                    printf("sendto() failed with error code: %d\n", gai_strerror(errno));
                }
                printf("Message send\n"); */

        // try to receive some data, this is a blocking call
        if ((recv_len = recv(clientSocket, &person, 512, 0)) == -1)
        {
            printf("recv() failed with error code: %d\n", gai_strerror(errno));
            close(clientSocket);
            pthread_exit(NULL);
        }

        if (isUserExist == false)
        {
            newUser.name = person.name;
            newUser.socket = clientSocket;

            users->push_back(newUser);

            printf("User list length: %d\n", users->size());

            printf("User added, Name: %s, Socket: %d\n", newUser.name, newUser.socket);

            isUserExist = true;
        }

        // print details of the client/peer and the data received
        printf("Message: %s, To who: %s\n", person.message, person.messageToWho);

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

void changeServerPortAddress(serverAdresses *serverAddress)
{
    serverAddress->portAddress = 8800;
    printf("Server:port address changed to: %d\n", serverAddress->portAddress);
}

void createServer(struct sockaddr_in *server, serverAdresses *serverAddress)
{
    printf("connected on %d port\n", serverAddress->portAddress);
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr(serverAddress->hostAddress);
    server->sin_port = htons(serverAddress->portAddress);
}