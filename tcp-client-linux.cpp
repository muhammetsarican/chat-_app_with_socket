#include "component/client/cfunctions.h"

class userInfo
{
public:
    char *name = NULL;
    char *messageToWho = NULL;
    char *message = NULL;
    size_t inputSize;

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

    vector<UserDetail> *users = new vector<UserDetail>;

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
    thread receiveThread(PetiveChat, serverSocket);
    while (true)
    {
        // Describe yourself at first
        clientMessage.person = getMessage(&clientMessage, &userinfo);
        clientMessage.type = CLIENT;

        string userMessage = clientMessage.person.message;
        if (userMessage.find("close") != -1)
        {
            clientMessage.err=GONE;
        }
        string message = generateMsg(clientMessage);

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
    }

    return 0;
}