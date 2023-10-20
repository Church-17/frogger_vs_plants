#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "resources.h"

#define LIST_SETTINGS {"language", "difficulty", "skin"}

int game_params[N_SETTINGS] = {0, 1, 2}; // DEFAULT SETTTINGS
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

void rd_params(void) {
    int tmp_int;
    char tmp_buff[LIM_STR_BUFF];
    str var_params[N_SETTINGS] = LIST_SETTINGS;
    FILE* fptr = fopen(SETTINGS_PATH, "r");
    if(fptr == NULL) {
        wr_params();
    } else {
        for(int i = 0; !feof(fptr) && i < N_SETTINGS; i++) {
            fscanf(fptr, "%s = %d", tmp_buff, &tmp_int);
            if(strcmp(tmp_buff, var_params[i]) == 0) {
                game_params[i] = tmp_int;
                break;
            }
        }
        fclose(fptr);
    }
}

void wr_params(void) {
    str var_params[N_SETTINGS] = LIST_SETTINGS;
    FILE* fptr = fopen(SETTINGS_PATH, "w");
    if(fptr == NULL) {
        return;
    }
    for(int i = 0; i < N_SETTINGS; i++) {
        fprintf(fptr, "%s = %d\n", var_params[i], game_params[i]);
    }
    fclose(fptr);
}

UserScore* rd_best(void) {
    int i;
    dalloc(UserScore, best, N_BEST);
    for(i = 0; i < N_BEST; i++) {
        best[i].user = alloc(char, LIM_STR_BUFF);
        sprintf(best[i].user, "NULL");
        best[i].score = -1;
    }
    FILE* fptr = fopen(BEST_PATH, "r");
    if(fptr == NULL) {
        wr_best(best);
    } else {
        for(i = 0; !feof(fptr) && i < N_BEST; i++) {
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