#include<stdio.h>
#include<winsock2.h>

int main(){
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c, recv_len;
    char buf[512];

    printf("\nInitialising Winsock...");
    if(WSAStartup(MAKEWORD(2,2), &wsa)!=0)
    {
        printf("Failed. Error code: %d", WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    // Create a Socket
    if((s=socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET)
    {
        printf("Could not create socket: %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(8888);

    // Bind
    if(bind(s, (struct sockaddr *)&server, sizeof(server))==SOCKET_ERROR)
    {
        printf("Bind failed with error code: %d", WSAGetLastError());
    }

    puts("Bind is done");

    // Listen to incoming connections
    listen(s, 3);

    // Accept listening for data
    while(1)
    {
        c=sizeof(struct sockaddr_in);
        new_socket=accept(s, (struct sockaddr *)&client, &c);
        if(new_socket == INVALID_SOCKET)
        {
            printf("accept failed with error code: %d", WSAGetLastError());
        }

        puts("Connection accepted");

        printf("Waiting for data...\n");

        fflush(stdout);

        // clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', 512);

        // try to receive some data, this is a blocking call
        if((recv_len=recv(new_socket, buf, 512, 0))==SOCKET_ERROR)
        {
            printf("recv() failed with error code: %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        // print details of the client/peer and the data received
        printf("Received packet from %s:%d\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Data: %s\n\n", buf);

        // now reply the client with the same data
        if(send(new_socket, "mesaj geldi", 12, 0)==SOCKET_ERROR){
            printf("sendto() failed with error code: %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
    }

    getchar();

    closesocket(s);
    WSACleanup();

    return 0;
}