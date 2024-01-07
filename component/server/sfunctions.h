#include "../bothFunctions.h"

bool sendMessage(vector<UserDetail> *users, Msg *msg, SOCKET clientSocket);
void saveLog(Msg *msg);

struct ThreadArgs
{
    int clientSocket;
    vector<UserDetail> *users;
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

void addNewUserToList(SOCKET clientSocket, Person person, vector<UserDetail> *users)
{
    // printf("new user joined to us\n");

    UserDetail newUser;

    strcpy(newUser.name, person.name);
    newUser.socket = clientSocket;

    users->push_back(newUser);

    printf("\nNew user added, Name: %s Socket: %d \n", newUser.name, newUser.socket);
}

Msg createNewMessage(errCodes err, char *name, char *message, char *messageToWho, connType type)
{
    Msg *newMsg = new Msg();
    Person person;

    strcpy(person.name, name);
    strcpy(person.message, message);
    strcpy(person.messageToWho, messageToWho);

    newMsg->err = err;
    newMsg->person = person;
    newMsg->type = type;

    return *newMsg;
}
void msgFromServer(SOCKET clientSocket, Msg *msg, vector<UserDetail> *users)
{
    string message = generateMsg(*msg);
    bool isMessageSended = false;
    char *username;

    for (auto user : *users)
    {
        if (user.socket == clientSocket)
        {
            username = user.name;
        }
        string messageToWho = msg->person.messageToWho;
        string userName = user.name;
        if (msg->type == SERVER || messageToWho.find("global") != -1 || userName.find(msg->person.messageToWho) != -1)
        {
            // printf("user name: %s\n", user.name);
            // printf("message to who: %s\n", messageToWho.c_str());
            // printf("user socket: %d\n", user.socket);
            if (send(user.socket, message.c_str(), message.length(), 0) == -1)
            {
                printf("sendto() failed with error code: %d\n", gai_strerror(errno));
            }
            isMessageSended = true;
        }
    }
    if (isMessageSended == false)
    {
        Msg newMsg;
        newMsg = createNewMessage(MERR, (char *)"SERVER", (char *)"user not found!", username, SERVER);
        sendMessage(users, &newMsg, clientSocket);
    }
}

void dropUserFromList(SOCKET clientSocket, vector<UserDetail> *users)
{
    vector<UserDetail> newListAfterRemove;
    for (auto user : *users)
    {
        if (user.socket == clientSocket)
        {
            // printf("%s named person not added to new list\n", user.name);
            continue;
        }
        // printf("%s named person added to new list\n", user.name);
        newListAfterRemove.push_back(user);
    }
    *users = newListAfterRemove;
    // printf("user dropped from list successfully.\n");
}

void closeSocket(vector<UserDetail> *users, SOCKET clientSocket)
{
    close(clientSocket);
    dropUserFromList(clientSocket, users);
    pthread_exit(NULL);
}

bool sendMessage(vector<UserDetail> *users, Msg *msg, SOCKET clientSocket)
{
    // printf("we are in send message function\n");
    Msg *newMsg = new Msg();
    switch (msg->err)
    {
    case CONN:
    { /* code */
        // printf("msg err is conn\n");
        addNewUserToList(clientSocket, msg->person, users);
        *newMsg = createNewMessage(MESG, (char *)"SERVER", (char *)"joined to server!", msg->person.name, SERVER);
        saveLog(msg);
        msgFromServer(clientSocket, newMsg, users);
        break;
    }
    case MESG:
    { /* code */
        // printf("someone send message to our one of the clients\n");
        saveLog(msg);
        msgFromServer(clientSocket, msg, users);
        return true;
    }
    case MERR:
    { /* code */
        break;
    }
    case GONE:
    { /* code */
        *newMsg = createNewMessage(GONE, (char *)"SERVER", (char *)"left from server!", msg->person.name, SERVER);
        saveLog(msg);
        msgFromServer(clientSocket, newMsg, users);

        closeSocket(users, clientSocket);
        break;
    }
    }
    saveLog(msg);
    msgFromServer(clientSocket, msg, users);

    return true;
}
string getTodayDate()
{
    time_t currentTime = time(nullptr);

    string dateString = asctime(localtime(&currentTime));

    return cutSpaces(dateString);
}
string logFormat(Msg *msg)
{
    string logStrFormat = getTodayDate() + " code:" + getErrCode(msg->err) + " connection type:" + getConnType(msg->type) + " person informations: name:" + msg->person.name + " receiver:" + msg->person.messageToWho + " message:" + msg->person.message + "\n";

    return logStrFormat;
}

void saveLog(Msg *msg)
{
    FILE *fptr;

    string logStr = logFormat(msg);
    fptr = fopen("/mnt/hdd/Desktop/C++/Datacom Project/Lab Example/component/logs/log.txt", "a");

    if (fptr == NULL)
    {
        printf("Error no file found!\n");
        // exit(1);
    }
    else
    {
        fprintf(fptr, "%s", logStr.c_str());
        fclose(fptr);
    }
}

void *ServiceClient(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;

    int clientSocket = threadArgs->clientSocket;
    int recv_len;
    char buffer[512];
    string comingMessage;

    vector<UserDetail> *users = threadArgs->users;

    Person person;
    UserDetail newUser;
    Msg msg;

    bool isMessageSended;

    char *userList = vectorToChar(*users);
    // Now send other users list to client
    if (send(clientSocket, userList, strlen(userList), 0) == -1)
    {
        printf("sendto() failed with error code: %d\n", gai_strerror(errno));
    }

    // printf("User list sent to client\n");

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

        isMessageSended = sendMessage(users, &msg, clientSocket);
        if (isMessageSended == false)
        {
            printf("Something gone wrong!\n");
        }
        // printf("Your message sent successfully\n");

        // printf("coming message: %s\n", msg.person.message);
        // printf("coming name: %s\n", msg.person.name);
        // printf("coming message to who: %s\n", msg.person.messageToWho);

        memset(buffer, '\0', strlen(buffer));
    }
}