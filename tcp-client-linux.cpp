#include <stdio.h>
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

int main(){
    SOCKET socketOne;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    int recv_size;

        // Create a socket
    if((socketOne=socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        printf("Could not create socket: %d", gai_strerror(errno));
    }

    printf("Socket created. \n"),

    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_family=AF_INET;
    server.sin_port=htons(8888);

        // Connect to remote server
    if(connect(socketOne, (struct sockaddr *)&server, sizeof(server))==-1)
    {
        puts("connect error\n");
        return 1;
    }
    printf("Connected.\n");

        // Send some data
    message="Fucking Nerd";
    if(send(socketOne, message, strlen(message), 0) == -1)
    {
        puts("Send failed");
        return 1;
    }
    puts("Reply received\n");

    // Add a null terminating character to make it a proper string before
    server_reply[recv_size]='\0';
    puts(server_reply);

    return 0;
}