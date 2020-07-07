#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



#define PORT 4242


#define BUFFER_LENGTH 4096



int
main(void) {

   
    struct sockaddr_in client, server;

   
    int serverfd, clientfd;

    char buffer[BUFFER_LENGTH];

    fprintf(stdout, "iniciando server\n");

   
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd == -1) {
        perror("error  servidor/socket:");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Servidor/socket criado: %d\n", serverfd);


    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    memset(server.sin_zero, 0x0, 8);


    
    int yes = 1;
    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR,
                  &yes, sizeof(int)) == -1) {
        perror("Socket  error:");
        return EXIT_FAILURE;
    }


    
    if(bind(serverfd, (struct sockaddr*)&server, sizeof(server)) == -1 ) {
        perror("Socket bind error:");
        return EXIT_FAILURE;
    }


  
    if(listen(serverfd, 1) == -1) {
        perror("Listen error:");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Rodando na porta %d\n", PORT);

    socklen_t client_len = sizeof(client);
    if ((clientfd=accept(serverfd,
        (struct sockaddr *) &client, &client_len )) == -1) {
        perror("Accept error:");
        return EXIT_FAILURE;
    }


    
    strcpy(buffer, "Hello, cliente!\n\0");


  
    if (send(clientfd, buffer, strlen(buffer), 0)) {
        fprintf(stdout, "Client conectado.\nEsperando a mensagem do cliente ...\n");

        
        do {

            memset(buffer, 0x0, BUFFER_LENGTH);

           
            int message_len;
            if((message_len = recv(clientfd, buffer, BUFFER_LENGTH, 0)) > 0) {
                buffer[message_len - 1] = '\0';
                printf("Client: %s\n", buffer);
            }


            
            if(strcmp(buffer, "tcheu") == 0) {
                send(clientfd, "tchau", 3, 0);
            } else {
                send(clientfd, "..\n", 4, 0);
            }

        } while(strcmp(buffer, "bye"));
    }

    
    close(clientfd);

    close(serverfd);

    printf("Connection closed\n\n");

    return EXIT_SUCCESS;
}
