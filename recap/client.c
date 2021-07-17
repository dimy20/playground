#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
int main(){
    int network_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    server_address.sin_addr.s_addr = INADDR_ANY; 
    
    int err = connect(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
    if(err == -1){
         perror("something went wrong");
    }
    char buff[256];
    char test_buff[256]= "this is a test";
    send(network_socket,&test_buff,sizeof(test_buff),0);


    recv(network_socket,&buff,sizeof(buff),0);
    printf("sever sent : %s",buff);




    close(network_socket);
    return 0;
}


