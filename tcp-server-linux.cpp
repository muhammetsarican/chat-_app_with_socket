#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

// defines
#define SOCKET int

int main()
{
    SOCKET socketObj, newSocket;
    struct sockaddr_in server, client;
    int c, recv_len;
    char buf[512];
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
        printf("Bind failed with error code: %d\n", gai_strerror(errno));
    }

    printf("Bind is done.\n");

    // Listen to incoming connections
    listen(socketObj, 3);

    // Accept listening for data
    while (1)
    {
        socklen_t socketLen = sizeof(struct sockaddr_in);
        newSocket = accept(socketObj, (struct sockaddr *)&client, &socketLen);

        if (newSocket != -1)
        {
            printf("accept failed with error code: %d\n", gai_strerror(errno));
        }

        puts("Connection accepted");

        printf("Waiting for data...\n");

        fflush(stdout);

        // clear buffer by filling bull, it might have previously received data
        // memset(buf, '\0', 512);

        // try to receive some data, this is a blocking call
        if ((recv_len = recv(newSocket, buf, 512, 0)) == -1)
        {
            printf("recv() failed with error code: %d\n", gai_strerror(errno));
            break;
        }

        // print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Data: %s\n\n", buf);

        // Now reply the client with the same data
        if (send(newSocket, "message received", 12, 0) == -1)
        {
            printf("sendto() failed with error code: %d\n", gai_strerror(errno));
            break;
        }
    }

    getchar();

    close(socketObj);

    return 0;
}