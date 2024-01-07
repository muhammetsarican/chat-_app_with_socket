/* #include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h> */

#include "../bothFunctions.h"

using namespace std;

void describeYourself(Person *user)
{
    printf("Enter your name: ");
    fgets(user->name, 15, stdin);
}

void subOfString(Person *user, char comingMessage[100], int endIndex)
{
    char messageToWho[15] = "";
    char message[100] = "";

    for (int i = 0; i < strlen(comingMessage); i++)
    {
        if (i < endIndex)
        {
            messageToWho[i] = comingMessage[i];
        }
        if (i >= endIndex + 2)
        {
            message[i - (endIndex + 2)] = comingMessage[i];
        }
    }

    strcpy(user->messageToWho, messageToWho);
    strcpy(user->message, message);
}

void messageFormat(Person *user, char message[100])
{
    string userMessage = message;

    for (int i = 0; i < strlen(message); i++)
    {
        if (message[i] == '-')
        {
            if (message[i + 1] == '>')
            {
                subOfString(user, message, i);
            }
        }
    }
    if (userMessage.find("close") != -1)
    {
        strcpy(user->message, "close");
        printf("close function\n");
    }
}

Person getMessage(Msg *msg, Person *user)
{
    if (strlen(user->name) == 0)
    {
        msg->err = CONN;
        describeYourself(user);
    }
    else
    {
        msg->err = MESG;
        printf("You introduced yourself successfully.\n");
    }

    char message[100] = "";

    printf("Please enter your message (Format: friend_name->message): ");
    fgets(message, 100, stdin);

    messageFormat(user, message);

    return *user;
}

void receiveMessages(int clientSocket)
{
    char buffer[512];
    Msg incomingMessage;

    while (true)
    {
        if (recv(clientSocket, buffer, 512, 0) <= 0)
        {
            printf("Receive failed or connection closed");
            break;
        }
        // Process received message (e.g., display or handle it)
        encodeMsg(&incomingMessage, buffer);
        printf("%s: %s\n", incomingMessage.person.name, incomingMessage.person.message);
        memset(buffer, '\0', strlen(buffer));
    }
    /*     char buffer[1024];
        while (true)
        {
            if (recv(clientSocket, buffer, sizeof(buffer), 0) <= 0)
            {
                printf("Receive failed or connection closed");
                break;
            }
            // Process received message (e.g., display or handle it)
            printf("\nServer: %s\n", buffer);
            memset(buffer, '\0', strlen(buffer));
        } */
}
