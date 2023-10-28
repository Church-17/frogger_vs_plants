// Include libs
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "res.h"

// Define constant
#define LIST_SETTINGS {"language", "difficulty", "skin"}

// Define inter-object variables
int game_settings[N_SETTINGS] = {0, 1, 2}; // DEFAULT SETTTINGS
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

// Read settings from settings file
void rd_settings(void) {
    FILE* fptr = fopen(SETTINGS_PATH, "r"); // Open settings file
    if(fptr == NULL) { // If settings file cannot be opened...
        wr_settings(); // Write new default settings file
    } else {
        // Read settings file and overwrite settings variable
        for(int i = 0; !feof(fptr) && i < N_SETTINGS; i++) {
            fscanf(fptr, "%*s = %d\n", &(game_settings[i]));
        }
        fclose(fptr); // Close settings file
    }
}

// Write updated settings in settings file
void wr_settings(void) {
    // Init vars & open settings file
    str str_settings[N_SETTINGS] = LIST_SETTINGS;
    FILE* fptr = fopen(SETTINGS_PATH, "w");

    if(fptr == NULL) { // If settings file cannot be created...
        return; // Use default settings without save them
    }
    // Write settings file
    for(int i = 0; i < N_SETTINGS; i++) {
        fprintf(fptr, "%s = %d\n", str_settings[i], game_settings[i]);
    }
    fclose(fptr); // Close settings file
}

// Read best scores file
List_UserScore rd_best(void) {
    // Init vars & open best scores file
    int i;
    List_UserScore best;
    alloc(UserScore, best.list, N_BEST);
    for(i = 0; i < N_BEST; i++) {
        alloc(char, best.list[i].user, LIM_STR_BUFF);
        sprintf(best.list[i].user, "NULL"); // NULL: placeholder for non-existing records
        best.list[i].score = -1; // Negative score identifies non-existing records
    }
    FILE* fptr = fopen(BEST_PATH, "r");

    if(fptr == NULL) { // If best scores file cannot be opened...
        wr_best(best); // Write new empty best scores file
    } else {
        // Read best scores and fill the UserScore list
        for(i = 0; i < N_BEST; i++) {
            fscanf(fptr, "%s = %d", best.list[i].user, &(best.list[i].score));
        }
        fclose(fptr); // Close best scores file
    }
    return best;
}

// Write updated best scores file
void wr_best(List_UserScore best) {
    // Open best scores file
    FILE* fptr = fopen(BEST_PATH, "w");

    if(fptr == NULL) { // If best scores file cannot be created
        return;
    }
    // Write best scores file
    for(int i = 0; i < N_BEST; i++) {
        fprintf(fptr, "%s = %d\n", best.list[i].user, best.list[i].score);
    }
    fclose(fptr); // Close best scores file
}