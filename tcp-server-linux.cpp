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
#include <map>
#include <list>

// #include </mnt/hdd/Desktop/C++/Datacom Project/Lab Example/errhandler.h>

// defines
#define SOCKET int

using namespace std;
struct userDetail{
    char name;
    SOCKET socket;
};
void addUserToList(char name, SOCKET socket){
    map<char*,userDetail> userMap;
    userMap["user"]={name, socket};
    printf("%s\n", userMap["user"].name);
}
struct serverPerson
{
    char name[15] = "";
    char messageToWho[15] = "";
    char message[100] = "";
};

int main()
{
    SOCKET socketObj, newSocket;
    struct sockaddr_in server, client;
    int c, recv_len;
    char buf[512];
    serverPerson joe;

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
        printf("%s", errorHandler(errcode));
        exit(EXIT_FAILURE); */
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
        while (1)
        {
            printf("Waiting for data...\n");

            fflush(stdout);

            // clear buffer by filling bull, it might have previously received data
            memset(joe.name, '\0', strlen(joe.name));
            memset(joe.messageToWho, '\0', strlen(joe.messageToWho));
            memset(joe.message, '\0', strlen(joe.message));

            // try to receive some data, this is a blocking call
            if ((recv_len = recv(newSocket, &joe, 512, 0)) == -1)
            {
                printf("recv() failed with error code: %d\n", gai_strerror(errno));
                break;
            }

            // print details of the client/peer and the data received
            printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            addUserToList(*joe.name, newSocket);
            // printf("Item index: %d\n", findItemIndex(buf));

            // Now reply the client with the same data
            if (send(newSocket, "message received", strlen("message received"), 0) == -1)
            {
                printf("sendto() failed with error code: %d\n", gai_strerror(errno));
                break;
            }
            printf("Message send\n");
            break;
        }
    }
    getchar();

    close(socketObj);

    return 0;
}