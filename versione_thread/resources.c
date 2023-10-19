#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "resources.h"

#define LIM_TMP_BUFF 100
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

void rd_params() {
    FILE* fptr = fopen(SETTINGS_PATH, "r");
    str var_params[N_SETTINGS] = LIST_SETTINGS;
    char tmp_buff[LIM_TMP_BUFF];
    int tmp_int, j;
    if(fptr == NULL) {
        wr_params();
    } else {
        for(int i = 0; !feof(fptr); i++) {
            fscanf(fptr, "%s = %d", tmp_buff, &tmp_int);
            for(j = 0; j < N_SETTINGS; j++) {
                if(strcmp(tmp_buff, var_params[j]) == 0) {
                    game_params[j] = tmp_int;
                    break;
                }
            }
        }
        fclose(fptr);
    }
}

void wr_params() {
    FILE* fptr = fopen(SETTINGS_PATH, "w");
    str var_params[N_SETTINGS] = LIST_SETTINGS;
    if(fptr == NULL) {
        return;
    }
    for(int i = 0; i < N_SETTINGS; i++) {
        fprintf(fptr, "%s = %d\n", var_params[i], game_params[i]);
    }
    fclose(fptr);
}