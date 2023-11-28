// Include libs
#include "utils.h"
#include "res.h"

// Fucntion prototypes
Dict_str_int check_conf_file(FILE* fptr, int lim_lines);

// Define constant
#define CHK_FILE_ERR -1
#define LIST_SETTINGS {"language", "difficulty", "skin", "color_1", "color_2"} // Array of setting labels
#define LIST_SET_ID {SET_LANG_ID, SET_DIFF_ID, SET_SKIN_ID, SET_COL1_ID, SET_COL2_ID} // Index of settings
#define LIST_N_OPTIONS {N_LANGUAGE, N_DIFFICULTY, N_SKIN, N_COLOR, N_COLOR} // N options of each settings
#define SETTINGS_PATH "/tmp/.game_settings.ini" // Path of settings file
#define BEST_PATH "/tmp/.game_best.dat" // Path of best scores files
#define FIRST_ALLOWED_CHAR (int)'!' // First allowed char in username
#define LAST_ALLOWED_CHAR (int)'~' // Last allowed char in username

// Define inter-object variables
int game_settings[N_SETTINGS] = {1, 0, 0, 0, 0}; // Default settings
str strContainer[][N_LANGUAGE] = { // Container of all strings
    {"New game", "Nuova partita"},
    {"Best scores", "Migliori punteggi"},
    {"Settings", "Impostazioni"},
    {"Credits", "Crediti"},
    {"Quit", "Esci"},
    {"Language", "Lingua"},
    {"Difficulty", "Difficoltà"},
    {"Cancel", "Annula"},
    {"Easy", "Facile"},
    {"Medium", "Medio"},
    {"Hard", "Difficile"},
    {"Common", "Comune"},
    {"Rare", "Rara"},
    {"Epic", "Epica"},
    {"SOPR Project 23-24", "Progetto SOPR 23-24"},
    {"Pause", "Pausa"},
    {"Resume", "Continua"},
    {"Home menu", "Menù principale"},
    {"Main color","Colore principale"},
    {"Secondary color", "Colore secondario"},
    {"White","Bianco"},
    {"Red","Rosso"},
    {"Green","Verde"},
    {"Yellow","Giallo"},
    {"Blue","Blu"},
    {"Cyan","Ciano"},
    {"Apply [Enter]", "Applica [Invio]"},
    {"Score", "Punteggio"},
    {"Retry", "Riprova"},
    {"Expand the terminal", "Espandi il terminale"},
    {"Minimum", "Minimo"},
    {"Actual", "Attuale"}
};

// Read settings from settings file
void rd_settings(void) {
    // Init vars & open settings file
    int i, j;
    str str_settings[N_SETTINGS] = LIST_SETTINGS;
    int ind_set[N_SETTINGS] = LIST_SET_ID;
    int len_opts[N_SETTINGS] = LIST_N_OPTIONS;
    FILE* fptr = fopen(SETTINGS_PATH, READ); // Open settings file
    if(fptr == NULL) { // If settings file cannot be opened...
        wr_settings(game_settings); // Write default settings file
        return;
    }
    // Read settings file
    Dict_str_int dict = check_conf_file(fptr, N_SETTINGS);
    if(dict.len == CHK_FILE_ERR) { // If settings file integrity is compromised...
        wr_settings(game_settings); // Write default settings file
        return;
    }
    // Overwrite game_settings
    for(i = 0; i < dict.len; i++) {
        for(j = 0; j < N_SETTINGS; j++) {
            if(!strcmp(dict.key[i], str_settings[j])) { // Check which settings is
                if(dict.val[i] >= len_opts[j]) { // If is already used or the value is too high...
                    for(; i < dict.len; i++) { // Free Dict
                        free(dict.key[i]);
                    }
                    free(dict.key);
                    free(dict.val);
                    wr_settings(game_settings); // Restore default for the other settings
                    return;
                }
                game_settings[ind_set[j]] = dict.val[i];
                break;
            }
        }
        free(dict.key[i]); // Free used key
    }
    // Free memory
    free(dict.key);
    free(dict.val);
}

// Write updated settings in settings file
void wr_settings(int* set) {
    // Init vars & open settings file
    int i;
    str str_settings[N_SETTINGS] = LIST_SETTINGS;
    int ind_set[N_SETTINGS] = LIST_SET_ID;
    FILE* fptr = fopen(SETTINGS_PATH, WRITE);
    // Write new settings in game_settings
    for(i = 0; i < N_SETTINGS; i++) {
        game_settings[i] = set[i];
    }
    if(fptr == NULL) { // If settings file cannot be writed...
        return; // Use previous settings
    }
    // Write settings file
    for(i = 0; i < N_SETTINGS; i++) {
        fprintf(fptr, "%s = %d\n", str_settings[i], game_settings[ind_set[i]]);
    }
    fclose(fptr); // Close settings file
}

// Read best scores file
Dict_str_int rd_best(void) {
    // Init vars & open best scores file
    Dict_str_int best;
    FILE* fptr = fopen(BEST_PATH, READ);
    if(fptr == NULL) { // If best scores file cannot be opened...
        best.len = 0;
        wr_best(best); // Write new empty best scores file
        return best;
    }
    // Read best scores
    best = check_conf_file(fptr, N_BEST);
    if(best.len == CHK_FILE_ERR) { // If best scores file integrity is compromised...
        best.len = 0;
        wr_best(best); // Write new empty best scores file
    }

    // Free unused memory
    for(int i = best.len; i < N_BEST; i++) {
        free(best.key[i]);
    }

    sort_dict(&best); // Sort dict
    return best;
}

// Write updated best scores file
void wr_best(Dict_str_int best) {
    // Open best scores file
    FILE* fptr = fopen(BEST_PATH, WRITE);
    if(fptr == NULL) { // If best scores file cannot be created
        return;
    }
    // Write best scores file
    for(int i = 0; i < best.len; i++) {
        fprintf(fptr, "%s = %d\n", best.key[i], best.val[i]);
    }
    fclose(fptr); // Close best scores file
}

// Check file format
Dict_str_int check_conf_file(FILE* fptr, int lim_lines) {
    // Init vars
    int line, col, achar;
    char numstr[LEN_STR_INT];
    Dict_str_int dict;
    alloc(str, dict.key, lim_lines);
    for(line = 0; line < lim_lines; line++) {
        alloc(char, dict.key[line], LIM_STR_BUFF);
    }
    alloc(int, dict.val, lim_lines);
    dict.len = 0;
    rewind(fptr); // Restore fptr start position

    // For each line...
    for(line = 0; line < lim_lines; line++) {
        // Check string
        for(col = 0; col < LIM_STR_BUFF; col++) {
            achar = getc(fptr);
            if(achar == EOF) { // Handle EOF
                if(col != 0) {
                    dict.len = CHK_FILE_ERR; // ERROR in file
                }
                return dict;
            }
            if(achar == ' ') { // Handle space
                if(col == 0) { // In first col error
                    dict.len = CHK_FILE_ERR; // ERROR in file
                    return dict;
                }
                break; // Otherwise string ended
            }
            if(achar < FIRST_ALLOWED_CHAR || achar > LAST_ALLOWED_CHAR) { // Check allowed char
                dict.len = CHK_FILE_ERR; // ERROR in file
                return dict;
            }
            dict.key[line][col] = (char)achar;
        }
        dict.key[line][col] = '\0'; // End string

        // Check ' = '
        if((achar = getc(fptr)) != '=' || (achar = getc(fptr)) != ' ') {
            dict.len = CHK_FILE_ERR; // ERROR in file
            return dict;
        }

        // Check value
        for(col = 0; col < LEN_STR_INT; col++) {
            achar = getc(fptr);
            if(achar == EOF || achar == '\n') { // Handle EOF or \n
                if(col == 0) { // In first col
                    dict.len = CHK_FILE_ERR; // ERROR in file
                    return dict;
                }
                break; // Otherwise end of line or file
            }
            if(achar < KEY_0 || achar > KEY_9) { // Check number char
                dict.len = CHK_FILE_ERR; // ERROR in file
                return dict;
            }
            numstr[col] = (char)achar;
        }
        numstr[col] = '\0'; // End string
        dict.val[line] = atoi(numstr); // Convert string to number
        (dict.len)++;
    }
    fclose(fptr); // Close settings file
    return dict;
}