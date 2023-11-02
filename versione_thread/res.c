// Include libs
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "res.h"

// Define constant
#define LIST_SETTINGS {"language", "difficulty", "skin", "color_1", "color_2"}
#define LIST_N_OPTIONS {N_LANGUAGE, N_DIFFICULTY, N_SKIN, N_COLOR, N_COLOR}
#define SETTINGS_PATH "/home/matte/.game_settings.ini"
#define BEST_PATH "/home/matte/.game_best.dat"
#define FIRST_ALLOWED_CHAR '!'
#define LAST_ALLOWED_CHAR '~'
#define LEN_INTSTR 12

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
    int len_opts[N_SETTINGS] = LIST_N_OPTIONS;
    FILE* fptr = fopen(SETTINGS_PATH, "r"); // Open settings file
    if(fptr == NULL) { // If settings file cannot be opened...
        wr_settings(); // Write new default settings file
        return;
    }
    // Read settings file
    Dict_str_int dict = check_conf_file(fptr, N_SETTINGS, LIM_STR_BUFF);
    fclose(fptr); // Close settings file
    if(dict.len != N_SETTINGS) { // If settings file integrity is compromised...
        wr_settings();
        return;
    }
    // Overwrite game_settings
    bool used[N_SETTINGS] = {0};
    for(i = 0; i < dict.len; i++) {
        for(j = 0; j < N_SETTINGS; j++) {
            if(!strcmp(dict.key[i], str_settings[j])) { // Check which settings is
                if(used[j] || dict.val[i] >= len_opts[j]) { // If is already used or the value is too high...
                    wr_settings(); // Restore default for the other settings 
                    return;
                }
                game_settings[j] = dict.val[i];
                used[j] = 1; // Mark settings as used
            }
        }
        free(dict.key[i]);
    }
    // Free memory
    free(dict.key);
    free(dict.val);
}

// Write updated settings in settings file
void wr_settings(void) {
    // Init vars & open settings file
    str str_settings[N_SETTINGS] = LIST_SETTINGS;
    FILE* fptr = fopen(SETTINGS_PATH, "w");
    if(fptr == NULL) { // If settings file cannot be created...
        return; // Use previous settings
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
        return best;
    }
    // Read best scores
    Dict_str_int dict = check_conf_file(fptr, N_BEST, LIM_STR_BUFF);
    fclose(fptr); // Close best scores file
    if(dict.len != N_BEST) { // If settings file integrity is compromised...
        wr_best(best);
        return best;
    }
    // Fill the UserScore list
    for(i = 0; i < N_BEST; i++) {
        free(best.list[i].user);
        best.list[i].user = dict.key[i];
        best.list[i].score = dict.val[i];
    }
    // Free memory
    free(dict.key);
    free(dict.val);
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

// Check file format
Dict_str_int check_conf_file(FILE* fptr, int lines, int lim) {
    // Init vars
    int line, col, achar;
    char numstr[LEN_INTSTR];
    Dict_str_int dict;
    alloc(str, dict.key, lines);
    for(line = 0; line < lines; line++) {
        alloc(char, dict.key[line], lim);
    }
    alloc(int, dict.val, lines);
    dict.len = lines;
    rewind(fptr); // Restore fptr start position

    // For each line...
    for(line = 0; line < lines; line++) {
        // Check string
        for(col = 0; col < lim; col++) {
            achar = getc(fptr);
            if(achar == EOF) { // Handle EOF
                dict.len = -1; // ERROR in file
                return dict;
            }
            if(achar == ' ') { // Handle space
                if(col == 0) { // In first col error
                    dict.len = -1; // ERROR in file
                    return dict;
                }
                break; // Otherwise string ended
            }
            if(!is_char_in((char)achar, FIRST_ALLOWED_CHAR, LAST_ALLOWED_CHAR)) { // Check allowed char
                dict.len = -1; // ERROR in file
                return dict;
            }
            dict.key[line][col] = (char)achar;
        }
        dict.key[line][col] = '\0'; // End string

        // Check ' = '
        if((achar = getc(fptr)) != '=' || (achar = getc(fptr)) != ' ') {
            dict.len = -1; // ERROR in file
            return dict;
        }

        // Check value
        for(col = 0; col < LEN_INTSTR; col++) {
            achar = getc(fptr);
            if(achar == EOF || achar == '\n') { // Handle EOF or \n
                if(col == 0) { // In first col
                    dict.len = -1; // ERROR in file
                    return dict;
                }
                break; // Otherwise end of line or file
            }
            if(!is_char_in((char)achar, '0', '9')) { // Check number char
                dict.len = -1; // ERROR in file
                return dict;
            }
            numstr[col] = (char)achar;
        }
        numstr[col] = '\0'; // End string
        dict.val[line] = atoi(numstr); // Convert string to number
    }
    return dict;
}