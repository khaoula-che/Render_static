#include <stdio.h>
#include <stdlib.h>
#include "player.h"

#define PIECE_EMPTY '.'
#define MAX_PLAYERS 4

// Définition des symboles des joueurs
char player_pieces[MAX_PLAYERS] = {'O', 'X', '#', '*'};

// Structure de base pour un joueur
typedef struct {
    char symbol;
    char* name;
} Player;

// Structure de base pour le jeu
typedef struct {
    char** grid;
    int grid_height;
    int grid_width;
    int player_turn;
    int win_condition;
    Player** players;  // Tableau des joueurs
    int nb_players;
} Game;

// Initialisation du jeu
Game* init_game(int grid_height, int grid_width, int win_condition, int nb_players) {
    Game* game = malloc(sizeof(Game));
    game->grid = malloc(grid_height * sizeof(char*));
    for (int i = 0; i < grid_height; i++) {
        game->grid[i] = malloc(grid_width * sizeof(char));
        for (int j = 0; j < grid_width; j++) {
            game->grid[i][j] = PIECE_EMPTY;
        }
    }
    game->player_turn = 0;
    game->grid_height = grid_height;
    game->grid_width = grid_width;
    game->win_condition = win_condition;
    game->nb_players = nb_players;

    // Initialisation des joueurs
    game->players = malloc(nb_players * sizeof(Player*));
    for (int i = 0; i < nb_players; i++) {
        game->players[i] = malloc(sizeof(Player));
        game->players[i]->symbol = player_pieces[i];
        game->players[i]->name = malloc(20 * sizeof(char));
        snprintf(game->players[i]->name, 20, "Joueur %d", i + 1);  // Nom par défaut
    }

    return game;
}

// Libérer la mémoire
void free_game(Game* game) {
    for (int i = 0; i < game->grid_height; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
    for (int i = 0; i < game->nb_players; i++) {
        free(game->players[i]->name);
        free(game->players[i]);
    }
    free(game->players);
    free(game);
}

// Afficher la grille
void print_grid(Game* game) {
    for (int i = 0; i < game->grid_height; i++) {
        for (int j = 0; j < game->grid_width; j++) {
            printf("%c ", game->grid[i][j]);
        }
        printf("\n");
    }
    for (int j = 0; j < game->grid_width; j++) {
        printf("%d ", j + 1);
    }
    printf("\n");
}

// Placer une pièce dans la grille
void move_piece(Game* game, int col) {
    if (col < 0 || col >= game->grid_width) {
        printf("Colonne invalide !\n");
        return;
    }

    if (game->grid[0][col] != PIECE_EMPTY) {
        printf("Colonne déjà pleine !\n");
        return;
    }

    int row = game->grid_height - 1;
    while (row >= 0 && game->grid[row][col] != PIECE_EMPTY) {
        row--;
    }
    game->grid[row][col] = game->players[game->player_turn]->symbol;
    game->player_turn = (game->player_turn + 1) % game->nb_players;
}

// Vérification si un joueur a gagné
int check_win(Game* game) {
    int count;
    for (int i = 0; i < game->grid_height; i++) {
        for (int j = 0; j <= game->grid_width - game->win_condition; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i][j + k]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->player_turn - 1;
            }
        }
    }
    for (int i = 0; i <= game->grid_height - game->win_condition; i++) {
        for (int j = 0; j < game->grid_width; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i + k][j]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->player_turn - 1;
            }
        }
    }
    for (int i = game->win_condition - 1; i < game->grid_height; i++) {
        for (int j = 0; j <= game->grid_width - game->win_condition; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i - k][j + k]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->player_turn - 1;
            }
        }
    }
    for (int i = 0; i <= game->grid_height - game->win_condition; i++) {
        for (int j = 0; j <= game->grid_width - game->win_condition; j++) {
            count = 1;
            for (int k = 1; k < game->win_condition; k++) {
                if (game->grid[i][j] != PIECE_EMPTY && game->grid[i][j] == game->grid[i + k][j + k]) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == game->win_condition) {
                return game->player_turn - 1;
            }
        }
    }
    return -1;
}

// Boucle principale du jeu
void game_loop(Game* game) {
    while (1) {
        print_grid(game);
        int col;
        printf("%s (%c), choisissez une colonne (1-%d) : ", game->players[game->player_turn]->name, game->players[game->player_turn]->symbol, game->grid_width);
        scanf("%d", &col);

        if (col < 1 || col > game->grid_width) {
            printf("Colonne invalide, choisissez une colonne valide entre 1 et %d.\n", game->grid_width);
            continue;
        }

        move_piece(game, col - 1);
        int winner = check_win(game);
        if (winner != -1) {
            print_grid(game);
            printf("%s (%c) a gagné !\n", game->players[winner]->name, game->players[winner]->symbol);
            break;
        }
    }
}
