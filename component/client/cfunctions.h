#include <stdio.h>
#include <string.h>

#include "../bothFunctions.h"

using namespace std;

Person describeYourself()
{
    Person user;

    printf("Enter your name: ");
    fgets(user.name, 15, stdin);

    return user;
}

Person subOfString(Person user, char comingMessage[100], int endIndex)
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

    strcpy(user.messageToWho, messageToWho);
    strcpy(user.message, message);

    return user;
}

Person messageFormat(Person user, char message[100])
{
    for (int i = 0; i < strlen(message); i++)
    {
        if (message[i] == '-')
        {
            if (message[i + 1] == '>')
            {
                user = subOfString(user, message, i);
            }
        }
    }

    return user;
}

Person getMessage(Person user)
{
    if (strlen(user.name) == 0)
    {
        user = describeYourself();
    }
    else
    {
        printf("You introduced yourself successfully.\n");
    }

    char message[100] = "";

    printf("Please enter your message (Format: friend_name->message): ");
    fgets(message, 100, stdin);

    user = messageFormat(user, message);

    return user;
}

void receiveMessages(int clientSocket)
{
    char buffer[1024];
    while (true)
    {
        if (recv(clientSocket, buffer, sizeof(buffer), 0) <= 0)
        {
            printf("Receive failed or connection closed");
            break;
        }
        // Process received message (e.g., display or handle it)
        printf("Server: %s\n", buffer);
    }
}
