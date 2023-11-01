// Include libs
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "res.h"

// Define constant
#define LIST_SETTINGS {"language", "difficulty", "skin", "color_1", "color_2"}
#define SETTINGS_PATH "/home/matte/.game_settings.ini"
#define BEST_PATH "/home/matte/.game_best.dat"
#define LIM_STR_BUFF 100

// Define inter-object variables
int game_settings[N_SETTINGS] = {0}; // Default settings
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
    {"Home menu", "Torna al menu"},
    {"Main color","Colore principale"},
    {"Secondary color", "Colore secondario"},
    {"White","Bianco"},
    {"Red","Rosso"},
    {"Green","Verde"},
    {"Yellow","Giallo"},
    {"Blue","Blu"},
    {"Cyan","Ciano"}
};

// Read settings from settings file
void rd_settings(void) {
    int i, j;
    str str_settings[N_SETTINGS] = LIST_SETTINGS;
    FILE* fptr = fopen(SETTINGS_PATH, "r"); // Open settings file
    if(fptr == NULL) { // If settings file cannot be opened...
        wr_settings(); // Write new default settings file
    } else {
        // Read settings file and overwrite game_settings
        Dict_str_int dict = check_conf_file(fptr, N_SETTINGS, LIM_STR_BUFF);
        for(i = 0; i < dict.len; i++) {
            for(j = 0; j < N_SETTINGS; j++) {
                if(!strcmp(dict.key[i], str_settings[j])) {
                    game_settings[j] = dict.val[i];
                }
            }
        }
        for(i = 0; i < dict.len; i++) {
            free(dict.key[i]);
        }
        free(dict.key);
        free(dict.val);
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