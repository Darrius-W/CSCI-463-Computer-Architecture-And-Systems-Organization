#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
     int sock;
     struct sockaddr_in server;
     struct hostent *hp;
     char buf[2048];

     /* Create socket */
     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0) {
	  perror("opening stream socket");
	  exit(1);
     }
     /* Connect socket using name specified by command line. */
     server.sin_family = AF_INET;
     hp = gethostbyname(argv[1]);
     if (hp == 0)
       hp = "127.0.0.1";

     bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
     server.sin_port = htons(atoi(argv[2]));

     if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
	  ssize_t len = read(fileno(stdin), buf, sizeof(buf));
	  perror("connecting stream socket");
	  exit(1);
     }
     if (write(sock, DATA, sizeof(DATA)) < 0){
	  ssize_t len = write(fileno(stdout), buf, buf_len);
	  cout << len;
	  perror("writing on stream socket");
     }
     close(sock);
}
