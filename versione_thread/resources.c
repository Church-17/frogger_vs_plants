#include <stdlib.h>
#include "utils.h"
#include "resources.h"

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
    if(fptr == NULL) {
        wr_params();
    } else { // BISOGNA CONTROLLARE LA CONFORMITA' DEL FILE
        for(int i = 0; i < N_SETTINGS; i++) {
            fscanf(fptr, "%*s = %d", &(game_params[i]));
        }
        fclose(fptr);
    }
}

void wr_params() {
    FILE* fptr = fopen(SETTINGS_PATH, "w");
    str var_params[N_SETTINGS] = {"language", "difficulty", "skin"};
    if(fptr == NULL) {
        fprintf(stderr, "ERROR: Impossibile to create game settings file");
        return;
    }
    for(int i = 0; i < N_SETTINGS; i++) {
        fprintf(fptr, "%s = %d\n", var_params[i], game_params[i]);
    }
    fclose(fptr);
}