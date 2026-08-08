#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 56
#define BUFFER_SIZE 4096

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Sequential server listening on port %d...\n", PORT);

    while (1)
    {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("\n[+] Client connected from %s\n", inet_ntoa(address.sin_addr));

        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(client_socket, buffer, BUFFER_SIZE - 1);

        if (valread > 0)
        {
            buffer[strcspn(buffer, "\r\n")] = 0;
            printf("    Requested file: %s\n", buffer);

            FILE *file = fopen(buffer, "rb");
            if (file == NULL)
            {
                char *err_msg = "ERROR: File not found.\n";
                send(client_socket, err_msg, strlen(err_msg), 0);
                printf("    File not found.\n");
            }
            else
            {
                int bytes_read;
                char file_buf[BUFFER_SIZE];
                while ((bytes_read = fread(file_buf, 1, BUFFER_SIZE, file)) > 0)
                {
                    send(client_socket, file_buf, bytes_read, 0);
                }
                fclose(file);
                printf("    File sent successfully.\n");
            }
        }

        close(client_socket);
        printf("[-] Connection terminated.\n");
    }

    return 0;
}