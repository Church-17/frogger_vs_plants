/// MENU /// 
void view(const List_str title, const List_str sx, const List_str dx, const List_attr cols, const Game_t* gamevar);
/*
title: lista di titoli
sx: lista di scritte a sinistra
dx: lista di scritte a destra
cols: lista di colori (fg e bg)
gamevar: per passare a resize_proc()
*/
int menu(const List_str title, const List_str opts, const Game_t* gamevar);
/*
Ritorna l'indice dell'opzione selezionata

title: lista di titoli
opts: lista settaggi
gamevar: per passare a resize_proc()
*/
void check_key(int key, int* hl, const List_str set);
/*
Chiamata da menu per verificare l'azione di un tasto
Salva nell'indirizzo di hl l'indice dell'opzione selezionata

key: tasto premuto
*h1: indirizzo su cui salvare l'indice
set: lista settaggi
*/
int home_menu(void);
void best_scores_menu(void);
void settings_menu(void);
void credits_menu(void);
int pause_menu(const Game_t* gamevar);
int gameover_menu(int score, Game_t* gamevar);
int quit_menu(const Game_t* gamevar);
/*
Chiamano menu() o view() in base alla loro funzione

Quelle che prendon gamevar sono chiamate durante il gioco e ripristinano la schermata in caso di resize 
Le altre passano NULL per ripristinare la schermata demo()
*/

/// RES ///
void wr_settings(int* set);
/*
set: vettore dei settaggi (all'indice di ogni settaggio contiene il suo valore)

1. Scrive in game_settings (array globale) i nuovi settaggi.
2. Scrive nel file i nuovi settaggi (se non esiste un file, lo crea e carica i settaggi di default).
3. Chiude il file (e lo salva).
*/
void rd_settings(void);
/*
1. Apre il file in lettura (se non esiste, chiama wr_settings())
2. Crea un dizionario con i valori del file controllando che questi siano codificati correttamente con check_conf_file(), se non lo sono chiama wr_settings()
3. Se la codifica è corretta, per ogni settaggio si controlla che il valore settato non sia oltre il limite consentito, se lo è si libera il dizionario e si chiama wr_settings()
4. Se invece è tutto corretto, si modifica game settings con il valore del dizionario (letto dal file)
*/
void wr_best(Dict_str_int best);
/*
Apre il file dei migliori punteggi (se non esiste lo crea) e lo scrive in base al dizionario passato come argomento
*/
Dict_str_int rd_best(void);
/*
1. Apre un file in lettura e crea un dizionario verificando che la codifica sia corretta.
2. Se non è corretta, chiama wr_best() con il risultato che nel file non compare niente.
3. Ordina il dizionario chiamando la sort_dict().
*/
Dict_str_int check_conf_file(FILE* fptr, int lim_lines);
/*
Verifica la correttezza della codifica di un file; restituisce un dizionario con le coppie (chiave, valore) del file

fptr: puntatore al file che deve verificare
lim_lines: numero di linee che contiene il file

1. Porta il puntatore all'inizio del file e cicla per il numero di linee
2. Controlla ogni carattere della prima parola (la chiave). Questa non può contenere il carattere EOF, non può iniziare con uno spazio e non può contenere caratteri oltre quelli entro il range consentito
2. Se succede una di queste cose, ritorna un dizionario con len = -1 che indica dati non validi
3. Se non succede nessuna di queste cose, salva la parola (carattere per carattere) tra le chiavi del dizionario e la termina con un '\0'

4. Verifica il pattern ' = ', se questo non è rispettato, codifica errata, altrimenti procede alla verifica del valore

5. Controlla ogni carattere della seconda parola (il valore). Questa non può contenere il carattere EOF o \n e non può contenere caratteri oltre quelli entro il range consentito (numeri)
6. Se succede una di queste cose, ritorna un dizionario con len = -1 che indica dati non validi
7. Se non succede nessuna di queste cose, salva la parola (carattere per carattere) tra i valori del dizionario, la termina con un '\0' e la converte in numero.
*/

// UTILS //
time_t timestamp(void);
/*
Restituisce il tempo UNIX in millisecondi convertendo il risultato di gettimeofday() che lo restituisce in secondi + microsecondi
*/
int max_strlen(const List_str strings, int min_len);
/*
Restituisce il valore maggiore tra min_len e la lunghezza della stringa più lunga della lista passata come argomento.
*/
List_str dict_to_list(const str* obj, const int* ind, int len);
/*
Converte un dizionario con chiavi intere in una lista.
Lo scopo è quello di poter associare coppie indice-valore senza preoccuparsi dell'ordine degli indici, tanto viene automaticamente stabilito da questa funzione.

*obj: lista di stringhe contenuta nel dizionario (valori)
*ind: lista di indici del dizionario (chiavi)
len: lunghezza di queste due liste

Crea una List_str mantenendo l'associazione indice-valore.
*/
void sort_dict(Dict_str_int* dict);
/*
Ordina il dizionario dato con il Selection Sort
*/
WINDOW* new_win(int dim_y, int dim_x, int pos_y, int pos_x);
/*
Inizializza una nuova finestra abilitando keypad e gestendo un eventuale errore di inizializzazione.
*/
WINDOW* new_ctrwin(int dim_y, int dim_x);
/*
Inizializza una nuova finestra al centro dello schermo (utilizza LINES e COLS)
*/
void un_win(WINDOW* win);
/*
Cancella tutto il contenuto di una finestra e la elimina.
*/
void quit(int err_code);
/*
Termina la finestra di ncurses e stampa nello standard error una stringa di errore che ha come indice il codice err_code
*/

/// MAIN ///
int main(void);
/*
1. Inizializza il terminale di ncurses con tutte le funzioni necessarie;
2. Inizializza tutti i colori e le coppie di colori;
3. Legge i settaggi;
4. Chiama ceck_term()
5. Mette la musica in riproduzione.
6. Inizializza il main_scr (finestra di gioco)
7. Cicla finché non viene selezionata l'opzione di uscita (do_quit == TRUE);
8. Scorre per le opzioni (chiama menu) e per ogni opzione
    - Riproduce l'effetto di selezione e eventuali altri effetti
    - Stampa la demo e aggiorna lo schermo
    - Fa la sua azione (chiama il gioco, un menu, una view,...)
9. Una volta fuori, toglie la musica del gioco ed esce senza errori
*/
bool check_term(WINDOW* win);
/*
Verifica se il terminale è grande abbastanza da contenere la schermata di gioco.
Restituisce FALSE se era già grande abbastanza, TRUE se non lo era.

win: una finestra eventualmente presente nella schermata di gioco (menu, view, ...)

1. Se il terminale non è abbastanza grande
    - Sposta il main_scr e la finestra eventualmente passata nell'angolo in alto a sinistra
    - Crea una finestra di errore che dice di estendere la dimensione del terminale indicando l'attuale dimensione e quella necessaria (aggiornando quella attuale iterativamente)
    - Quando la dimensione viene sistemata, chiude quella finestra e restituisce TRUE 
2. Se il terminale è abbastanza grande, restituisce FALSE
*/
bool resize_proc(WINDOW* win, int dim_y, int dim_x, const Game_t* gamevar);
/*
Gestisce il resize del terminale

win: eventuale finestra presente nel main_scr, se non c'è, win == NULL
dim_x, dim_y: dimensioni della eventuale finestra
gamevar: serve a ristampare la schermata di gioco, se è NULL viene ristampata la demo

1. Risolve un bug di libreria, ridefinendo correttamente le variabili _maxy e _maxy del main_scr e della eventuale finestra passata
2. Sposta al centro del terminale il main_scr e la finestra.
3. Se c'è bisogno di ristampare (dipende da cosa restituisce check_term(), chiamata all'inizio) ristampa il gioco o la demo, altrimenti non ristampa.
4. Ritorna TRUE se è avvenuta la ristampa, FALSE altrimenti.
*/

/// GAME ///

void demo(void);
/*
Stampa la schermata iniziale chiamando print_background e stampando la figlet del titolo
*/
bool game();
/*
Gestisce la selezione del menu di gameover (valore restituito da play_game()) con delle flags
*/
int play_game(void);
/*
1. Seleziona la musica in base alla difficoltà
2. Itera per 5 volte
3. Chiama play_manche() e ne salva il valore in gamevar, della quale controlla timer (che contiene le informazioni sullo stato del gioco)
4. In caso di partita persa aggiorna le vite e fa un'altra iterazione
5. In altri casi ritorna alla game()
6. Nel caso di manche vinta, riproduce tutti i suoni, aggiunge il punteggio a quello già accumulato e fa l'animazione
7. In caso di vite terminate:
    - Se la partita è vinta moltiplica il punteggio per il numero di vite rimaste, salva il punteggio tra i record (se si deve fare) e riproduce la musica adatta
    - Se la partita è persa, riproduce la musica adatta e chiama gameover_menu()
*/