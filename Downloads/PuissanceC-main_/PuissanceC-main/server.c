#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "player.h"
#include "board.h"
#include "colors.h"
#include "server.h"
#include "player.h"
#include "board.h"

void free_game(Game* game) {
    for (int i = 0; i < game->grid_height; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
    free(game);
}

void print_grid(Game* game) {
    printf("You have to place %d tokens to win !\n", game->win_condition);

    for (int i = 0; i < game->grid_height; i++) {
        printf("|");
        for (int j = 0; j < game->grid_width; j++) {
            printf(" %c |", game->grid[i][j]);
        }
        printf("\n");
    }

    for (int j = 0; j < game->grid_width; j++) {
        printf(" %3d ", j);
    }
    printf("\n");
}

void format_grid(Game* game, char* buffer) {
    strcpy(buffer, "You have to place tokens to win!\n");


    for (int i = 0; i < game->grid_height; i++) {
        strcat(buffer, "|");
        for (int j = 0; j < game->grid_width; j++) {
            char temp[4];
            snprintf(temp, sizeof(temp), " %c |", game->grid[i][j]);
            strcat(buffer, temp);
        }
        strcat(buffer, "\n");
    }

    for (int j = 0; j < game->grid_width; j++) {
        char temp[5];
        snprintf(temp, sizeof(temp), " %2d ", j);
        strcat(buffer, temp);
    }
    strcat(buffer, "\n");
}
int move_piece(Game* game, int col) {
    if (col < 0 || col >= game->grid_width) {
        printf("Colonne invalide !\n");
        return -1;
    }

    if (game->grid[0][col] != PIECE_EMPTY) {
        printf("Colonne déjà pleine !\n");
        return -1;
    }

    int row = game->grid_height - 1;
    while (row >= 0 && game->grid[row][col] != PIECE_EMPTY) {
        row--;
    }
    game->grid[row][col] = player_pieces[game->player_turn];
    game->player_turn = (game->player_turn + 1) % game->nb_players;
    return 0;
}

int check_win(Game* game) {
    int count;
    char current_piece;

    for (int i = 0; i < game->grid_height; i++) {
        for (int j = 0; j <= game->grid_width - game->win_condition; j++) {
            current_piece = game->grid[i][j];
            if (current_piece != PIECE_EMPTY) {
                count = 1;
                for (int k = 1; k < game->win_condition; k++) {
                    if (game->grid[i][j + k] == current_piece) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == game->win_condition) return game->player_turn == 0 ? 2 : 1;
            }
        }
    }

    return -1;
}

void game_loop_online(int client_socket, Game* game) {
    int winner = -1;
    char buffer[2048] = {0};

    while (winner == -1) {
        print_grid(game);

        if (game->player_turn == 0) {
            printf("Joueur 1 (O), entrez la colonne : ");
            int col;
            scanf("%d", &col);

            if (move_piece(game, col) != 0) {
                continue;
            }

            format_grid(game, buffer);
            send(client_socket, buffer, strlen(buffer), 0);
        } else {
            printf("En attente du coup du joueur 2...\n");
            recv(client_socket, buffer, sizeof(buffer), 0);
            int col = atoi(buffer);

            if (move_piece(game, col) != 0) {
                continue;
            }
        }

        winner = check_win(game);
    }

    printf("Le joueur %d a gagné !\n", winner);
    send(client_socket, "Le joueur a gagné !", strlen("Le joueur a gagné !"), 0);
}
