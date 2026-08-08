#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 56
#define BUFFER_SIZE 4096

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <server_ip> <filename>\n", argv[0]);
        return -1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *filename = argv[2];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock, filename, strlen(filename), 0);

    char save_path[1024];
    snprintf(save_path, sizeof(save_path), "downloaded_%s", filename);
    FILE *file = fopen(save_path, "wb");

    int bytes_received;
    while ((bytes_received = read(sock, buffer, BUFFER_SIZE)) > 0)
    {
        if (strncmp(buffer, "ERROR:", 6) == 0)
        {
            printf("%s", buffer);
            fclose(file);
            remove(save_path);
            close(sock);
            return -1;
        }
        fwrite(buffer, 1, bytes_received, file);
    }

    printf("Finished downloading %s to %s\n", filename, save_path);

    fclose(file);
    close(sock);
    return 0;
}

// question 6
// Write a program to create a server that listens to port 56 using stream sockets. Write a simple client program to connect to the server. Run multiple clients that request the server for binary files. The server should service each client one after the other before terminating the connection

// question 7
// Write a program to create a server that listens to port 57 using stream sockets. Write a simple client program to connect to the server. Run multiple clients that request the server for text files. The server should service all clients concurrently.
