#include "../bothFunctions.h"

void describeYourself(Person *user)
{
    string name;
    printf("Enter your nick (Max. 15): ");
    // fgets(name.c_str(), 15, stdin);
    getline(cin, name);
    // scanf("%s", &name);
    strcpy(user->name, name.c_str());
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
        // printf("close function\n");
    }
}

Person getMessage(Msg *msg, Person *user)
{
    if (strlen(user->name) == 0)
    {
        msg->err = CONN;
        describeYourself(user);
        printf("[INFO] Message Format: friend_name->message\n");
    }
    else
    {
        msg->err = MESG;
        printf("You introduced yourself successfully.\n");
    }

    string message;

    // fgets(message, 100, stdin);
    getline(cin, message);
    // scanf("%s", &message);
    messageFormat(user, (char *)message.c_str());

    return *user;
}

void printIncomingMessage(Msg *msg)
{
    switch (msg->type)
    {
    case SERVER:
    { /* code */
        printf("%s: '%s' %s\n", msg->person.name, msg->person.messageToWho, msg->person.message);

        break;
    }
    case CLIENT:
    { /* code */
        printf("%s: %s\n", msg->person.name, msg->person.message);

        break;
    }

    default:
        break;
    }
}

void PetiveChat(int clientSocket)
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

        printIncomingMessage(&incomingMessage);
        memset(buffer, '\0', strlen(buffer));
    }
}
