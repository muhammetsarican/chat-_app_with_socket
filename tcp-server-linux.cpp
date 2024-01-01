#include <stdio.h>
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
// #include <sstream>

#include <pthread.h>

// #include </mnt/hdd/Desktop/C++/Datacom Project/Lab Example/errhandler.h>
// #include "bothFunctions.h"
#include "component/server/sfunctions.h"

// defines
#define SOCKET int

using namespace std;

int main()
{
    SOCKET socketObj, newSocket;
    struct sockaddr_in server, client;
    int c, recv_len;
    char buf[512];
    serverPerson person;
    pthread_t threadId;

    char *userList = NULL;

    vector<userDetail> *users = new vector<userDetail>;

    ThreadArgs *args = new ThreadArgs;

    serverAdresses serverAddress;

    // errcodes errcode;

    // creating socket
    if ((socketObj = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket: %s\n", gai_strerror(errno));
    }

    printf("Socket created. \n");

    bool isPortChanged = false;
    while (true)
    {
        createServer(&server, &serverAddress);
        // bind
        if (bind(socketObj, (struct sockaddr *)&server, sizeof(server)) != 0)
        {
            printf("Bind connection error: %s\n", gai_strerror(errno));
            /*         errcode = CONN;
                    printf("%s", errorHandler(errcode));*/
            if (isPortChanged == false)
            {
                changeServerPortAddress(&serverAddress);
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
    listen(socketObj, 3);

    while (true)
    {
        // Accept listening for data
        socklen_t socketLen = sizeof(struct sockaddr_in);
        newSocket = accept(socketObj, (struct sockaddr *)&client, &socketLen);

        if (newSocket == -1)
        {
            printf("accept failed with error code: %d\n", gai_strerror(errno));
        }

        puts("Connection accepted");
        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Waiting for data...\n");

        args->clientSocket = newSocket;
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
            close(newSocket);
        }
        printf("Thread created.\n\n");
    }
    getchar();

    close(socketObj);

    return 0;
}