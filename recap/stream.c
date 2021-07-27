/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10	 // how many pending connections queue will hold
#define MAX_CLIENTS 5
#define RW_FD 2



typedef struct c_pids {
    int pid[256];
    int len;
} cpid_t;

int add_pid(cpid_t * c,int pid){
    if(c->len == sizeof(c->pid)/sizeof(int)){
        return -1;
    }else{
        c->pid[c->len] = pid;
        c->len++;
        return 0;
    }
}
void init(cpid_t * c){
    memset(&(c->pid),0,sizeof(c->pid));
    c->len = 0;
}


void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;
    //at the time we are in the handler, a 2d or 3rd children
    //could have changed state or terminated thus sending a SIGCHILD
    // this signal would not be queued, looping prevents that from happening
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
// test
void send_to_parent(int write_fd, int data){
    write(write_fd, &data, sizeof(int));
}

int main(void)
{
	int fd[MAX_CLIENTS][RW_FD];

	

    int pipe_fd[2];
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
    
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

    cpid_t c;
    init(&c);

    

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	// index to keep track of pipes ends
	int n = 0;

	while(n < MAX_CLIENTS) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}


        struct sockaddr_in * addr = (struct sockaddr_in *)get_in_addr((struct sockaddr *)&their_addr);
        inet_ntop(their_addr.ss_family,addr,s,sizeof s);
		printf("server: got connection from %s\n", s);
		
		if(pipe(fd[n]) == -1){
			printf("error ocurred creating the pipe");
			exit(1);
		}

		if (!fork()) { // this is the child process

			// child doesnt need the listener
			close(sockfd);
			// close read end for this child
			close(fd[n][0]);
			// close siblings pipes
			for(int j =0; j<n;j++){
				close(fd[j][0]);
				close(fd[j][1]);
			}

/*             int c_pid = getpid();
            printf("new pid : %d \n", c_pid); */

			send_to_parent(fd[n][1],n);

			//socket stuff
			if (send(new_fd, "Hello, world!", 13, 0) == -1)
				perror("send");

			exit(0);
		}

		close(new_fd);  // parent doesn't need this



		n++;
	}


    for(int i = 0; i<n;i++){
        close(fd[i][1]);
    }

    for(int i = 0; i<n;i++){
        int res;
        read(fd[i][0],&res,sizeof(int));
        printf("res : %d \n",res);
    }
    for (int i = 0; i < MAX_CLIENTS; i++)
        close(fd[i][0]); 

	return 0;
}
