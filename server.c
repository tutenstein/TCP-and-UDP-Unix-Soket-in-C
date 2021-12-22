#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error(const char *msg ){
    perror(msg);
    exit(1);
}



int main(int argc, char *argv[])
{

    if (argc < 3)
        error("Do not protocol type or port no provided.Program terminated\n");
    int portno = atoi(argv[2]);
    int protocoltype= atoi(argv[1]);
    if (protocoltype == 1){
        int sockfd, newsockfd, n;
        char buffer[255];
        struct sockaddr_in serv_addr, cli_addr;
        socklen_t clilen;

        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd<0)
            error("ERROR opening socket.");
        printf("Socket created\n");

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);

        if (bind(sockfd, (struct sockaddr *)&serv_addr , sizeof(serv_addr))<0)
            error("binding failed");

        printf("bind done\n");
        listen(sockfd,5);
        printf("Waiting for incoming connections...\n");

        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd<0)
            error("Error on accept");
        printf("Connection accepted\n");

        while(1){
            bzero(buffer, 255);
            n = read(newsockfd, buffer, 255);
            if (n<0)
                error("Error on reading");
            printf("Client: %s\n", buffer);
            bzero(buffer, 255);
            printf("Enter the mesage: ");
            fgets(buffer,255,stdin);
            n = write(newsockfd, buffer, strlen(buffer));
            if (n<0)
                error("Error on writting");
            int i = strncmp("exit", buffer, 4);
            if (i==0)
                break;
        }

        close(newsockfd);
        close(sockfd);
    }
    else if (protocoltype == 2){

        int sockfd,n;
        char server_message[255], client_message[255];
        struct sockaddr_in serv_addr,cli_addr;
        int clilen = sizeof(cli_addr);


        sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        if (sockfd<0)
            error("Socket is not created.");
        printf("Socket is Created.\n");

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);

        if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
            error("Binding is failed");
        printf("binding is done.\n");

        while(1){
            printf("Waiting for data...\n");
            fflush(stdout);


            bzero(client_message, 255);
            bzero(server_message, 255);

            n = recvfrom(sockfd, client_message, sizeof(client_message), 0,(struct sockaddr*)&cli_addr, &clilen);
            if (n<0)
                error("Error on reading");
            printf("Received packet from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            printf("Client: %s\n", client_message);
            printf("Enter the mesage: ");
            fgets(server_message,255,stdin);
            n = sendto(sockfd, server_message, sizeof(server_message), 0,(struct sockaddr*)&cli_addr, clilen);
            if (n<0)
                error("Error on writting");
            int i = strncmp("exit", server_message, 4);
            if (i==0)
                break;
        }
        close(sockfd);

    }

    else{
        error("protocol type is incorrect. Program terminated.");
    }
    return 0;
}


