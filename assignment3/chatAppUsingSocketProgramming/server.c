# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# define PORT 8081
# define BUFFER_SIZE 1024
int main () {
int server_fd , new_socket ;
struct sockaddr_in address ;
int addrlen = sizeof ( address ) ;
char buffer [1024]={0};
//char *message ="Hello fro Server";

// Create socket
server_fd = socket ( AF_INET , SOCK_STREAM , 0) ;


// Configure address
address . sin_family = AF_INET ;
address . sin_addr . s_addr = INADDR_ANY ;
address . sin_port = htons ( PORT ) ;


// Bind socket

bind(server_fd, (struct sockaddr*)&address, sizeof(address));

// Listen for connections

listen(server_fd,3);
printf("Server is waiting on port %d ...\n",PORT);


// Accept connection
new_socket= accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

while (1){

// Communication loop (to be implemented )

read(new_socket, buffer, 1024);

printf("client:%s \n", buffer);

if(strcmp(buffer, "Bye") == 0)
{
break;
}

//fgets(msg,stdin);
scanf("%s[\n]",buffer);
getchar();
send(new_socket, buffer, strlen(buffer),0);



}


close ( new_socket ) ;
close ( server_fd ) ;
return 0;

}
