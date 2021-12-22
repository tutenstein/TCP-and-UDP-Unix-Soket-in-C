#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>



void error(const char *msg ){
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
    if (argc < 4)
        error("Do not protocol type or port no provided.Program terminated\n");
    int portno = atoi(argv[2]);
    int protocoltype= atoi(argv[1]);
    if (protocoltype == 1){
        int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;

        char buffer[255];

        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd<0)
            error("ERROR opening socket.");

        printf("Socket is created\n");

        server = gethostbyname(argv[3]);
        if (server == NULL)
            error("Error no such host");

        bzero((char *)&serv_addr,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server ->h_addr, (char *)&serv_addr.sin_addr.s_addr, server ->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
            error("connection failed");
        printf("Sucessfully conected with server\n");
        printf("You can write your message.\n");

        while(1){

            bzero(buffer, 255);
            printf("Enter the mesage: ");
            fgets(buffer,255,stdin);
            n = write(sockfd, buffer, strlen(buffer));
            if (n<0)
                error("Error on writting");
            printf("Waiting for reply from server...\nPlease don't send messages until the server reply.\n");

            bzero(buffer, 255);
            n = read(sockfd, buffer, 255);
            if (n<0)
                error("Error on reading");

            printf("Server: %s\n", buffer);
            int i = strncmp("exit", buffer, 4);
            if (i==0)
                break;
        }
        close(sockfd);


    }
    else if (protocoltype == 2) {
        int sockfd, n;
        struct sockaddr_in serv_addr;
        int servlen = sizeof(serv_addr);
        char server_message[255], client_message[255];

        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        if (sockfd<0)
            perror("Socket is not created.");
        printf("Socket is Created.\n");


        bzero((char *)&serv_addr,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = INADDR_ANY;



        while(1){

            bzero(client_message, 255);
            bzero(server_message, 255);
            printf("Enter the mesage: ");
            fgets(client_message,255,stdin);
            n = sendto(sockfd, client_message, strlen(client_message), 0,(struct sockaddr*)&serv_addr, servlen);
            if (n<0)
                error("Error on writting");
            printf("Waiting for reply from server...\nPlease don't send messages until the server reply.\n");
            n = recvfrom(sockfd, server_message, sizeof(server_message), 0,(struct sockaddr*)&serv_addr, &servlen);
            if (n<0)
                error("Error on reading");
            printf("Server: %s\n", server_message);
            int i = strncmp("exit", server_message, 4);
            if (i==0)
                break;
        }

        close(sockfd);

    }
    else{
        error("Entered wrong argument values. Program terminated.");
    }

    return 0;

}
