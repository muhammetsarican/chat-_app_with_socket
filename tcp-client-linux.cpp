#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

// defines
#define SOCKET int

class userInfo
{
public:
    char *name = NULL;
    char *surname = NULL;
    char *message = NULL;
    size_t inputSize;

    // userInfo(char name[80]){

    // }

    void describeYourself()
    {
        printf("Enter your name: ");
        if (getline(&name, &inputSize, stdin) >= 0)
            printf("This is your string: %s\n", name);
        else
            printf("Error\n");

        printf("\nEnter your surname: ");
        if (getline(&surname, &inputSize, stdin) >= 0)
            printf("This is your string: %s\n", surname);
        else
            printf("Error\n");

        // return
    }
    void setMessage()
    {
        printf("Enter your message: ");
        if (getline(&message, &inputSize, stdin) >= 0)
            printf("This is your string: %s\n", message);
        else
            printf("Error\n");
    }
    userInfo getUser(userInfo user)
    {
        user.name = name;
        user.surname = surname;
        user.message = message;
        return user;
    }
};

int main()
{
    userInfo myInfo;
    SOCKET socketOne;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    int recv_size;

    // Create a socket
    if ((socketOne = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Could not create socket: %d", gai_strerror(errno));
    }

    printf("Socket created. \n"),

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Connect to remote server
    if (connect(socketOne, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        puts("connect error\n");
        return 1;
    }
    printf("Connected.\n");

    // describe yourself firstly
    myInfo.describeYourself();
    myInfo.setMessage();
    myInfo = myInfo.getUser(myInfo);
    puts(myInfo.message);
    // Send some data
    if (send(socketOne, myInfo.message, strlen(myInfo.message), 0) == -1)
    {
        puts("Send failed");
        return 1;
    }
    puts("Reply received\n");

    // Add a null terminating character to make it a proper string before
    server_reply[recv_size] = '\0';
    puts(server_reply);

    return 0;
}