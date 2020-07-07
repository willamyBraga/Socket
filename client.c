#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>




#define PORT 4242



#define LEN 4096



#define SERVER_ADDR "127.0.0.1"




int
main(int argc, char *argv[]) {

 
    struct sockaddr_in server;
    
    int sockfd;

    int len = sizeof(server);
    int slen;

 
    char buffer_in[LEN];
   
    char buffer_out[LEN];


    fprintf(stdout, "Iniciado Cliente ...\n");


  
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error on client socket creation:");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Cliente socket criado: %d\n", sockfd);


  
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    memset(server.sin_zero, 0x0, 8);


    if (connect(sockfd, (struct sockaddr*) &server, len) == -1) {
        perror("Can't connect to server");
        return EXIT_FAILURE;
    }


  
    if ((slen = recv(sockfd, buffer_in, LEN, 0)) > 0) {
        buffer_in[slen + 1] = '\0';
        fprintf(stdout, "Servidor: %s\n", buffer_in);
    }


    
    while (true) {

    
        memset(buffer_in, 0x0, LEN);
        memset(buffer_out, 0x0, LEN);

        fprintf(stdout, "enviar mensagem ao servidor: ");
        fgets(buffer_out, LEN, stdin);

       
        send(sockfd, buffer_out, strlen(buffer_out), 0);

       
        slen = recv(sockfd, buffer_in, LEN, 0);
        printf("Mensagem enviada: %s\n", buffer_in);

       
        if(strcmp(buffer_in, "tchau") == 0)
            break;
    }


    close(sockfd);

    fprintf(stdout, "\nConnection closed\n\n");

    return EXIT_SUCCESS;
}
