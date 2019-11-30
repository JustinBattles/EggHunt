#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <string.h>

#define PORT 47308
#define K 1024

int watch(int epfd, int fd) {
  struct epoll_event e;
  e.events = EPOLLIN | EPOLLRDHUP | EPOLLOUT;
  e.data.fd = fd;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &e) < 0) {
  perror("epoll_ctl");
  exit(1);
 }
  return 0;

}


int main(int argc, char *argv[]) {
int r =0, n;
int eggs = 0;
struct epoll_event ev[2];
char str[20];
int coordsX, coordsY;
char buf[K];
char phrase[20];
int coords[K][K];


int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
int epfd;

if ((epfd = epoll_create(1)) < 0) {
  perror("epoll-create");
  exit(0);
  }

 struct sockaddr_in addr;
 addr.sin_family = AF_INET;
 addr.sin_port = htons(PORT);

 struct hostent *he = gethostbyname("effie.indstate.edu");

 if (he == NULL) {
   herror("gethostbyname");
   exit(2);
 } 

 memcpy(&(addr.sin_addr.s_addr), (he->h_addr_list[0]), he->h_length);

 if (connect(sock, (struct sockaddr *) &addr, sizeof (struct sockaddr_in)) < 0) {
   perror("connect");
   exit(3);
   }

 watch(epfd, sock);

 while (eggs != 10) {
   if (n < 0) {
     perror("epoll_wait");
     continue;
 }
   for (int j = 0; j < K; j++) {
     for (int k = 0; k < K; k++) {
       n = epoll_wait(epfd, ev, 2, -1); 
         for (int i = 0; i < n; i++) {
           if (ev[i].events & (EPOLLRDHUP | EPOLLERR | EPOLLHUP)) {
             printf("error\n"); eggs = 10; 
	     break;
	   }
	   
         if (coords[j][k] != 1) {
           sprintf(str, "look %d,%d", j, k);
           sendto(sock, str, strlen(str), 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
           
	   if (ev[i].events & EPOLLIN) {
	     r = read(sock, buf, K);
	     sscanf(buf, "%d,%d: %s", &coordsX, &coordsY, phrase);
	  
             coords[coordsX][coordsY] = 1;

	     strcpy(buf, "");
	     
	     if (strcmp(phrase, "Found") == 0) {
	       printf("Found: %d, %d\n", coordsX, coordsY);
	       eggs++;

	   } else if (strcmp(phrase, "No") == 0) 
	       coords[coordsX][coordsY] = 1;
	
	}
	
      }

    }
   }
  
  } 
 
 } 


}
