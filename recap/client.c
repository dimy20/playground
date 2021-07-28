
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490"
#define BUFFSIZE 100

void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
       return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char **argv){
    int fd, numbytes;    
    char buff[BUFFSIZE];
    // hints specifies criteria for selecting the list of socket address structures 
    // that will be returned by getaddrinfo() to server_info
    struct addrinfo hints, *server_info;
    char *node;
    int rv;
    //safe
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	};
    // input from user
    node = argv[1];
    // init values for hints
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC; // handle both ipv4 and/or ipv6 of node and PORT
    hints.ai_socktype = SOCK_STREAM; // TCP

    // get list of socket address structures of node and PORT
    if(rv = getaddrinfo(node,PORT,&hints,&server_info) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
    };

    // loop throught them and stick with the first one that succeeds
    struct addrinfo *p;
    for(p = server_info; p != NULL; p=p->ai_next){
        if((fd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1){
            perror("client: socket");
            continue;
        }
        if(connect(fd,p->ai_addr,p->ai_addrlen) == -1){
            perror("client : connect");
            close(fd);
            continue;
        }
        break;
    }
    // none succeeded
    if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

    // get socket address 
    void *addr = (struct sockaddr_in *)get_in_addr((struct sockaddr *)p->ai_addr);
    // convert address from network bytes to presentational 
    inet_ntop(p->ai_family,addr,s,sizeof(s));
    // present
    printf("client: connecting to %s \n",s);
    // we dont need this list anymore
    freeaddrinfo(server_info);

    // get data from server
    if ((numbytes = recv(fd, buff, BUFFSIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}
    buff[BUFFSIZE] = '\0';
    printf("client: received '%s'\n",buff);

    // the end
	close(fd);
	return 0;

}


