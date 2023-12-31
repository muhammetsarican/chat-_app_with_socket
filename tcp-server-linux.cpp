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

// defines
#define SOCKET int

using namespace std;

struct serverPerson
{
    char name[15] = "";
    char messageToWho[15] = "";
    char message[100] = "";
};

struct userDetail
{
    char *name;
    SOCKET socket;
};

struct ThreadArgs
{
    int clientSocket;
    vector<userDetail> *users;
};

char *vectorToChar(vector<userDetail> *users)
{
    // Convert vector to string
    vector<userDetail> userList = *users;
    char welcomeMessage[15]="# User List #\n";

    if (users->size() != 0)
    {
        int total = 0;
        for (size_t i = 0; i < users->size() - 1; ++i)
        {
            total += strlen(userList[i].name);
        }
        printf("Total character count: %d\n", total);

        // # User List #
        char *convertedstr{new char[total]};
        // convertedstr=(char *)"# User List #\n";

        // strcpy(convertedstr, (char *)welcomeMessage);

        for (size_t i = 0; i < users->size(); ++i)
        {
            // printf("Vector to char: %s\n", userList[i].name);
            // strcat(convertedstr, "* ");
            strcat(convertedstr, userList[i].name);
            // strcpy(convertedstr, userList[i].name);
            // convertedstr="%s%s",convertedstr,userList[i].name;

            if (i != users->size() - 1)
            {
                // printf(" ");
                // strcat(convertedstr, " ");
                // convertedstr="%s ",convertedstr;
            }
        }
        printf("User List: %s\n", convertedstr);
        return convertedstr;
    }
    return NULL;
}

void *handleClient(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int clientSocket = threadArgs->clientSocket;
    struct serverPerson person;
    int recv_len;

    vector<userDetail> *users = threadArgs->users;

    userDetail newUser;

    bool isUserExist = false;

    while (true)
    {
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

int main()
{
    SOCKET socketObj, newSocket;
    struct sockaddr_in server, client;
    int c, recv_len;
    char buf[512];
    serverPerson joe;
    pthread_t threadId;

    char *userList = NULL;

    vector<userDetail> *users = new vector<userDetail>;

    ThreadArgs *args = new ThreadArgs;

    // errcodes errcode;

    // creating socket
    if ((socketObj = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket: %s\n", gai_strerror(errno));
    }

    printf("Socket created. \n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8888);

    // bind
    if (bind(socketObj, (struct sockaddr *)&server, sizeof(server)) != 0)
    {
        printf("Bind connection error: %s\n", gai_strerror(errno));
        /*         errcode = CONN;
                printf("%s", errorHandler(errcode));*/
        exit(EXIT_FAILURE);
    }

    printf("Bind is done.\n");

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

        if ((userList = vectorToChar(users)) != NULL)
        {
            printf("user exist\n");
            printf("mainUserList: %s\n", userList);
        }
        else
        {
            printf("no user.\n");
        }

        fflush(stdout);

        // clear buffer by filling bull, it might have previously received data
        memset(joe.name, '\0', strlen(joe.name));
        memset(joe.messageToWho, '\0', strlen(joe.messageToWho));
        memset(joe.message, '\0', strlen(joe.message));

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