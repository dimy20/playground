
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
    struct addrinfo hints, *server_info;
    int rv;
    char s[INET6_ADDRSTRLEN];
    if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	};
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if(rv = getaddrinfo(argv[1],PORT,&hints,&server_info) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
    };

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
    if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

    void *addr = (struct sockaddr_in *)get_in_addr((struct sockaddr *)p->ai_addr);
    inet_ntop(p->ai_family,addr,s,sizeof(s));
    printf("client: connecting to %s \n",s);
    freeaddrinfo(server_info);

    if ((numbytes = recv(fd, buff, BUFFSIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}
    buff[BUFFSIZE] = '\0';
    printf("client: received '%s'\n",buff);
	close(fd);
	return 0;

}


