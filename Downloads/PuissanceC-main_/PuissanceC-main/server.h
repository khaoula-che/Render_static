#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "player.h"
#include "board.h"
#include "colors.h"
#include "player.h"
#include "board.h"

typedef struct {
    int server_socket;
    socklen_t addr_len;
    struct sockaddr_in address;
} ServerSocket;


ServerSocket init_server(int port) {
    ServerSocket server;
    server.server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server.server_socket < 0) {
        perror("Erreur de création du socket");
        exit(EXIT_FAILURE);
    }
    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(port);
    server.addr_len = sizeof(server.address);

    if (bind(server.server_socket, (struct sockaddr*)&server.address, server.addr_len) < 0) {
        perror("Erreur lors du bind");
        close(server.server_socket);
        exit(EXIT_FAILURE);
    }

    
    if (listen(server.server_socket, 2) < 0) {
        perror("Erreur lors du listen");
        close(server.server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d\n", port);
    return server;
}
