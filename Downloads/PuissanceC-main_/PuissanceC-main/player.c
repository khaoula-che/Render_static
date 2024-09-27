#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"


Player *newPlayer(int id, char *color, char symbol) {
    Player *p = malloc(sizeof(Player));

    p->id = id;
    p->color = malloc(strlen(color) + 1);
    strcpy(p->color, color);
    p->symbol = symbol;

    return p;
}

void freePlayer(Player *p) {
    free(p->color);
    free(p);
}