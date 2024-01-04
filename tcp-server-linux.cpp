/* #include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include <pthread.h>
 */
#include "component/server/sfunctions.h"

// defines
#define SOCKET int

using namespace std;

int main()
{
    SOCKET serverSocket, newClientSocket;
    struct sockaddr_in server, client;
    int c, recv_len;
    char buf[512];
    Person person;
    pthread_t threadId;

    char *userList = NULL;

    vector<userDetail> *users = new vector<userDetail>;

    ThreadArgs *args = new ThreadArgs;

    serverAdresses serverAddress;

    // creating socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket: %s\n", gai_strerror(errno));
    }

    printf("Socket created. \n");

    bool isPortChanged = false;
    while (true)
    {
        createServer(&server, &serverAddress);
        // bind
        if (bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) != 0)
        {
            printf("Bind connection error: %s\n", gai_strerror(errno));
            /*         errcode = CONN;
                    printf("%s", errorHandler(errcode));*/
            if (isPortChanged == false)
            {
                // changeServerPortAddress(&serverAddress);
                isPortChanged = true;
                continue;
            }
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Bind is done.\n");
            break;
        }
    }

    // Listen to incoming connections
    listen(serverSocket, 3);

    while (true)
    {
        // Accept listening for data
        socklen_t socketLen = sizeof(struct sockaddr_in);
        newClientSocket = accept(serverSocket, (struct sockaddr *)&client, &socketLen);

        if (newClientSocket == -1)
        {
            printf("accept failed with error code: %d\n", gai_strerror(errno));
        }
        puts("\n\n-------------------------------------");
        puts("Connection accepted");
        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Waiting for data...\n\n");

        args->clientSocket = newClientSocket;
        args->users = users;

        fflush(stdout);

        // clear buffer by filling bull, it might have previously received data
        memset(person.name, '\0', strlen(person.name));
        memset(person.messageToWho, '\0', strlen(person.messageToWho));
        memset(person.message, '\0', strlen(person.message));

        // multi thread using for chatting with client
        if (pthread_create(&threadId, NULL, handleClient, (void *)args) != 0)
        {
            printf("Thread creation failed\n");
            close(newClientSocket);
        }
        printf("Thread created.\n\n");
    }
    getchar();

    close(serverSocket);

    return 0;
}