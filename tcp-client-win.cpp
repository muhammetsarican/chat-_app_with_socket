#include<stdio.h>
#include<winsock2.h>

int main (){
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message, server_reply[2000];
    int recv_size;

    printf("\nInitialising Winsock...");
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
    {
        printf("Failed. Error Code: %d", WSAGetLasError());
        return 1;
    }

    printf("Initialised.\n");

    // Create a socket
    if((s=socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET)
    {
        printf("Could not create socket: %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_family=AF_INET;
    server.sin_port=htons(8888);

    // Connect to remote server
    if(connect(s, (struct sockaddr *)&server, sizeof(server))<0)
    {
        puts("connect error");
        return 1;
    }
    printf("Connected");

    // Send some data
    message="Merhaba Server";
    if(send(s, message, strlen(message), 0) < 0)
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