#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
typedef struct server {
    int listen_fd;
} server_t;

const int BACKLOG = 5;
const int PORT = 5000;
//reads n bytes from socket
void read_n_bytes(int socket, unsigned int n, void *buffer){
    int bytesRead = 0;
    int result;
    while(bytesRead < n){
        result = read(socket,buffer + bytesRead,n - bytesRead);
        if (result = -1){
            perror("error reading from socket");
        }
        if(result = 0){
            perror("EOF");
            break;
        }
    }

}
int server_listen(server_t * server, int port){
    int err = 0;

    struct sockaddr_in server_addr ={0};
    server_addr.sin_family = AF_INET;
    //translates an unsigned long integer into network byte order.
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // translates an unsigned short integer into network byte order.
    server_addr.sin_port = htons(port);

    //struct sockaddr server_addr= {0};

        err = (server->listen_fd = socket(AF_INET,SOCK_STREAM,0));
        if(err == -1){
            perror("socket");
            printf("Failed to create socket endpoint\n");
            return -1;
        };
        // we are binding the recently created socket to the adress defined above
        err = bind(server->listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));

        if(err == -1){
            perror("socket");
            printf("Failed to bind socket to and addres \n");
            return -1;
        };

        // set socket to be used to accept incoming request, it also changes 
        // the state of the socket from CLOSED to LISTEN
        err = listen(server->listen_fd,BACKLOG);

        if (err == -1) {
		perror("listen");
		printf("Failed to put socket in passive mode\n");
		return err;
	}
    // get socket fd




    // binds socket to addr, but still remains closed
    //err = bind(server->listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
      if (err == -1) {
                perror("write");
                printf("failed to write\n");
                return err;
        }
    return server->listen_fd;
}


int server_accept(server_t * server){
    int err = 0;
    int conn_fd;
    socklen_t client_len;
    struct sockaddr_in client_addr;
    client_len = sizeof(client_addr);
    // await connectiosn on the server listening socket, when a connection arrives it opens a new socket for the client and 
    // assign the actual values to client_addr and client_len
    err = (conn_fd = accept(server->listen_fd,(struct sockaddr*)&client_addr,&client_len));

    if (err == -1) {
            perror("accept");
            printf("failed accepting connection\n");
            return err;
        }
 /*    err = close(conn_fd);
	if (err == -1) {
		perror("close");
		printf("failed to close connection\n");
		return err;
	} */

	return err;

}



int main(){

    int err = 0;
    server_t server = {0};
    err = server_listen(&server,PORT);
    printf("%d",err);
    if(err == -1){
        printf("%s","An error ocurred while starting the server \n");
        return err;
    }else{
        printf("%s","server is now listening on port 5000 \n");
    }
    int len = 10;
    char * buff = (char*)malloc(sizeof(char)*len);
    while(1){
        err = server_accept(&server);
        if(err == -1){
            printf("failed accepting connection \n");
        }else{
            printf("client connected \n");
            read_n_bytes(err,10,buff);
            for(int i =0; i<10;i++){
                printf("%c",buff[0]);
            }
        }
    }
    return 0;
}
