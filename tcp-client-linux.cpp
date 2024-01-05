#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

#include <map>
#include <thread>

#include "component/client/cfunctions.h"

using namespace std;

// defines
#define SOCKET int
#define map std::map<char *, char *>

class userInfo
{
public:
    char *name = NULL;
    char *messageToWho = NULL;
    char *message = NULL;
    size_t inputSize;

    void describeYourself()
    {
        printf("Enter your name: ");
        if (getline(&name, &inputSize, stdin) >= 0)
            printf("This is your name: %s\n", name);
        else
            printf("Error\n");
    }
    void setMessage()
    {
        printf("Enter name, you want to send message to who: ");
        if (getline(&messageToWho, &inputSize, stdin) >= 0)
            printf("This is your messageToWho: %s\n", messageToWho);
        else
            printf("Error\n");

        printf("Enter your message: ");
        if (getline(&message, &inputSize, stdin) >= 0)
            printf("This is your message: %s\n", message);
        else
            printf("Error\n");
    }
    userInfo getUser(userInfo user)
    {
        user.name = name;
        user.messageToWho = messageToWho;
        user.message = message;
        return user;
    }
};

int main()
{
    Person userinfo;
    userInfo myInfo;
    SOCKET serverSocket;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    int recv_size;

    serverAdresses serverAdress;

    vector<userDetail> *users = new vector<userDetail>;

    Msg clientMessage;

    char userList[512];

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Could not create socket: %d", gai_strerror(errno));
    }

    printf("Socket created. \n");

    server.sin_addr.s_addr = inet_addr(serverAdress.hostAddress);
    server.sin_family = AF_INET;
    server.sin_port = htons(serverAdress.portAddress);

    // Connect to remote server
    if (connect(serverSocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        puts("connect error\n");
        return 1;
    }
    printf("Connected.\n");

    // Get online user list from server
    if ((recv_size = recv(serverSocket, userList, 512, 0)) == -1)
    {
        puts("recv failed\n");
    }
    printf("%s\n", userList);

    // New thread added to getting messages coming from server as unexpectedly
    thread receiveThread(receiveMessages, serverSocket);
    while (true)
    {
        // Describe yourself at first
        clientMessage.person = getMessage(&userinfo);
        clientMessage.err=MESG;
        clientMessage.type=CLIENT;

        string userMessage=clientMessage.person.message;
        string message=generateMsg(clientMessage);

        // Send some data
        if (send(serverSocket, message.c_str(), message.length(), 0) == -1)
        {
            puts("Send failed");
            return 1;
        }
        puts("Data Send\n");

        if (userMessage.find("close") != -1)
        {
            break;
        }
        // // Receive a reply from the server
        // if ((recv_size = recv(serverSocket, &server_reply, 2000, 0)) == -1)
        // {
        //     puts("recv failed\n");
        // }
        // puts("Reply received\n");

        // Add a null terminating character to make it a proper string before
        // server_reply[recv_size] = '\0';
        // printf("%s\n", server_reply);
    }

    return 0;
}