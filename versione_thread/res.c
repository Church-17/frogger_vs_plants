#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "res.h"

#define LIST_SETTINGS {"language", "difficulty", "skin"}

int game_settings[] = {0, 1, 2}; // DEFAULT SETTTINGS
str strContainer[][N_LANGUAGE] = {
    {"New game", "Nuova partita"},
    {"Best scores", "Migliori punteggi"},
    {"Settings", "Impostazioni"},
    {"Credits", "Crediti"},
    {"Quit", "Esci"},
    {"Language", "Lingua"},
    {"Difficulty", "Difficoltà"},
    {"Cancel", "Annula"},
    {"Easy", "Facile"},
    {"Normal", "Normale"},
    {"Difficult", "Difficile"},
    {"Common", "Comune"},
    {"Rare", "Rara"},
    {"Epic", "Epica"},
    {"SOPR Project 23-24", "Progetto SOPR 23-24"},
    {"Pause", "Pausa"},
    {"Resume", "Continua"},
    {"Home menu", "Torna al menu"}
};

void rd_settings(void) {
    FILE* fptr = fopen(SETTINGS_PATH, "r");
    if(fptr == NULL) {
        wr_settings();
    } else {
        for(int i = 0; !feof(fptr) && i < N_SETTINGS; i++) {
            fscanf(fptr, "%*s = %d\n", &(game_settings[i]));
        }
        fclose(fptr);
    }
}

void wr_settings(void) {
    str str_settings[N_SETTINGS] = LIST_SETTINGS;
    FILE* fptr = fopen(SETTINGS_PATH, "w");
    if(fptr == NULL) {
        return;
    }
    for(int i = 0; i < N_SETTINGS; i++) {
        fprintf(fptr, "%s = %d\n", str_settings[i], game_settings[i]);
    }
    fclose(fptr);
}

UserScore* rd_best(void) {
    int i;
    dalloc(UserScore, best, N_BEST);
    for(i = 0; i < N_BEST; i++) {
        alloc(char, best[i].user, LIM_STR_BUFF);
        sprintf(best[i].user, "NULL");
        best[i].score = -1;
    }
    FILE* fptr = fopen(BEST_PATH, "r");
    if(fptr == NULL) {
        wr_best(best);
    } else {
        for(i = 0; i < N_BEST; i++) {
            fscanf(fptr, "%s = %d", best[i].user, &(best[i].score));
        }
        fclose(fptr);
    }
    return best;
}

void wr_best(UserScore* best) {
    FILE* fptr = fopen(BEST_PATH, "w");
    if(fptr == NULL) {
        return;
    }
    for(int i = 0; i < N_BEST; i++) {
        fprintf(fptr, "%s = %d\n", best[i].user, best[i].score);
    }
    fclose(fptr);
}