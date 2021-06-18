#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

typedef struct server {
    int listen_fd;
} server_t;

int server_listen(server_t * server){
    int err = 0;
    const int PORT = 5000;
    struct sockaddr_in server_addr ={0};
    server_addr.sin_family = AF_INET;
    //translates an unsigned long integer into network byte order.
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //htons() translates an unsigned short integer into network byte order.
    server_addr.sin_port = htons(PORT);

    //struct sockaddr server_addr= {0};


    err = (server->listen_fd = socket(AF_INET,SOCK_STREAM,0));

    if(err == -1){
        perror("socket");
		printf("Failed to create socket endpoint\n");
        return -1;
    };

    // binds socket to addr, but still remains closed
    err = bind(server->listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
      if (err == -1) {
                perror("write");
                printf("failed to write\n");
                return err;
        }
    return server->listen_fd;
}


int server_accept(server_t * server);



int main(){
    int err = 0;
    server_t server = {0};
    err = server_listen(&server);
    printf("%d",err);
    if(err == -1){
        printf("%s","An error ocurred while starting the server");
        return err;
    }
    while(1){

    }
/*     while(1){
        err = server_accept(&server);
        if(err){
            printf("failed acceptiing connection");
            return err;
        };
    } */
    return 0;
}