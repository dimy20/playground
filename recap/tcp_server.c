#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
typedef struct server {
    int fd;
} server_t;
int main(){
    server_t server = {0};
    char buff[256] = "hello from the server";
    char buff_rcv[256];
    int server_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_address; 
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    server_address.sin_addr.s_addr = INADDR_ANY;
    //server socket bound to ip and port

    printf("%d",server_address.sin_port);
    bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    
    listen(server_socket,10);


    //close(server_socket);
    while(1) {
        int client_socket = accept(server_socket,NULL,NULL);
        printf("received from client : %s", buff_rcv);
        send(client_socket,buff,sizeof(buff),0);

        recv(client_socket,&buff_rcv,sizeof(buff_rcv),0);
    }
    
    return 0;
}