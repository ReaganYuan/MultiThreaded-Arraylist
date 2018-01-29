#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in server , client;
pthread_mutex_t mutex;


extern int count;

char* reverse_string(char *str)
{
    char temp;
    size_t len = strlen(str) - 1;
    size_t i;
    size_t k = len;

    for(i = 0; i < len; i++)
    {
        temp = str[k];
        str[k] = str[i];
        str[i] = temp;
        k--;
        if(k == (len / 2))
        {
            break;
        }
    }
    return str;
}


void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char  client_message[2000];

    bool is_alola = false;
    char* reverse_alola = calloc(256,sizeof(char));
    char* newline = "\n";
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
                //Send the message back to client
        if(is_alola == true)
        {
            write(sock,reverse_alola,strlen(reverse_alola));
            write(sock,newline,1);
            is_alola = false;
        }

        if(strcmp(client_message,"ALOLA!\r\n") == 0)
        {
            is_alola = true;
            reverse_alola = reverse_string(client_message);

        }

    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}

int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        return EXIT_FAILURE;
    }
    int n_value = atoi(argv[1]);

    pthread_mutex_init(&mutex, NULL);

    int socketfd = socket(AF_INET,SOCK_STREAM,0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);


    if( bind(socketfd,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Error Bind Failed");
        return 1;
    }
    puts("Successful Binding\n");

    //Listen
    listen(socketfd , n_value);

    puts("Waiting for incoming connections...\n");

    pthread_mutex_lock(&mutex);
    int  client_sock , *new_sock;
    int c = sizeof(struct sockaddr_in);
    // int read_size;/
    // char client_message[2000];
    pthread_t login_threads[n_value];
    //accept connection from an incoming client
    while( (client_sock = accept(socketfd, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        for(int i = 0 ; i < n_value; i++)
        {
            new_sock = malloc(1);
            *new_sock = client_sock;

        if( pthread_create( &login_threads[i] , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    pthread_mutex_unlock(&mutex);



    return 0;


}
