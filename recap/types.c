
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#define NODE_OFFSET 10
#define BUFFER_SIZE 4096
#define PORT "5000" 



typedef struct addrinfo addrinfo_t;
int showIps(int argc, char *argv[]){

	addrinfo_t hints, *res;
	int status;
	char ipstrg[INET6_ADDRSTRLEN];
	if (argc != 2) {
	    fprintf(stderr,"usage: showip hostname\n");
	    return 1;
	}
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(argv[1],NULL,&hints,&res))!=0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	printf("IP addresses for %s:\n\n", argv[1]);

	addrinfo_t *p =  NULL;
	for(p=res;p!= NULL; p=p->ai_next){
		void *addr;
		char *ipver;
		
		if(p->ai_family == AF_INET){
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}else{
			struct sockaddr_in6 *ipv6= (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPV6";
		}

		inet_ntop(p->ai_family,addr,ipstrg,sizeof ipstrg);
		printf("%s \n",ipstrg);
		printf("%s \n",ipver);

	}
	freeaddrinfo(res);
}

void initHints(struct addrinfo *hints){
	memset(hints,0,sizeof *hints);
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
}

int main(int argc, char *argv[]){
	int status;
	int sockfd;
	struct addrinfo hints, *res;
	initHints(&hints);



	if((status = getaddrinfo(NULL,"5000",&hints,&res))!=0){
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
			return 2;
		}
	
	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	bind(sockfd,res->ai_addr,sizeof res->ai_addrlen);
	struct sockaddr_in * data = (struct sockaddr_in *)(res->ai_addr);


	char port[3];
	memcpy(port,res->ai_addr->sa_data+12,2);
	port[2] = 0;
	printf("%c",res->ai_addr->sa_data[0]);
	return 0;
}
