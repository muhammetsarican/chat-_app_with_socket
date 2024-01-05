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

bool sendMessage(vector<userDetail> *users, Person *person)
{
    switch (person->type)
    {
    case SERVER:
    {
        Person newPerson = *person;
        string message = person->message;
        if (message.find("close") != -1)
        {
            strcpy(newPerson.message, "left from server!");

            for (auto user : *users)
            {
                if (send(user.socket, &newPerson, sizeof(newPerson), 0) == -1)
                {
                    perror("send() failed");
                    // Handle send failure if needed
                }
            }
            // break;
            /*             string serverMessage=person->name;
                        serverMessage+=" left from server!";

                        for (auto user : *users)
                        {
                            if (send(user.socket, &*person, serverMessage.length(), 0) == -1)
                            {
                                perror("send() failed");
                                // Handle send failure if needed
                            }
                        }
                        break; */
        }
        else
        {
            strcpy(person->message, "joined to server!");

            for (auto user : *users)
            {
                if (send(user.socket, &person, sizeof(person), 0) == -1)
                {
                    perror("send() failed");
                    // Handle send failure if needed
                }
            }
            /*             string serverMessage = person->name;
                        serverMessage += " joined to server!";

                        for (auto user : *users)
                        {
                            if (send(user.socket, serverMessage.c_str(), serverMessage.length(), 0) == -1)
                            {
                                perror("send() failed");
                                // Handle send failure if needed
                            }
                        } */
        }
        break;
    }
    case CLIENT:
    {
        printf("client\n");
        /*         bool isClientSocketExist = false;

                string messageToWho = person->messageToWho;

                for (auto user : *users)
                {
                    string userName = user.name;
                    if (messageToWho.find("global") != -1 || userName.find(person->messageToWho) != -1)
                    {
                        isClientSocketExist = true;
                        // Now reply the client with the same data
                        if (send(user.socket, person->message, strlen(person->message), 0) == -1)
                        {
                            printf("sendto() failed with error code: %d\n", gai_strerror(errno));
                        }
                        printf("%s's message sent to %s.", person->name, person->messageToWho);
                    }
                }

                return isClientSocketExist; */
        break;
    }
    default:
        break;
    }
    return false;
}

void dropUserFromList(vector<userDetail> *users)
{
    printf("user dropped from list successfully.");
}

void closeSocket(vector<userDetail> *users, SOCKET clientSocket, Person *person)
{
    close(clientSocket);
    sendMessage(users, person);
    dropUserFromList(users);
    pthread_exit(NULL);
}

void *handleClient(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;

    int clientSocket = threadArgs->clientSocket;
    int recv_len;
    char buffer[512];
    string comingMessage;

    vector<userDetail> *users = threadArgs->users;

    Person person;
    userDetail newUser;
    Msg msg;

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
        if ((recv_len = recv(clientSocket, buffer, 512, 0)) == -1)
        {
            printf("recv() failed with error code: %d\n", gai_strerror(errno));
            close(clientSocket);
            pthread_exit(NULL);
        }
        printf("%s\n", buffer);

        encodeMsg(&msg, buffer);

        printf("coming message: %s\n", msg.person.message);
        printf("coming message: %s\n", msg.person.name);
        printf("coming message: %s\n", msg.person.messageToWho);

        memset(buffer, '\0', strlen(buffer));
        /*         string personMessage = person.message;
                if (personMessage.find("close") != -1)
                {
                    printf("CloseSocket in server\n");
                    person.type = SERVER;
                    closeSocket(users, clientSocket, &person);
                } */

        /*         printf("Message received from client\n");
                bool isMessageSended = sendMessage(users, &person);

                if (isUserExist == false)
                {
                    newUser.name = person.name;
                    newUser.socket = clientSocket;

                    sendMessageToAllClients(users, newUser);

                    users->push_back(newUser);

                    printf("\nNew user added, Name: %s Socket: %d \n", newUser.name, newUser.socket);

                    isUserExist = true;
                } */
    }
}