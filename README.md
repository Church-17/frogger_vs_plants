DIPENDENZE
----------------------------
Per poter utilizzare la musica e gli effetti sonori occorre installare il media player "mpg123" eseguendo il comando:

- make install

Il progetto non ha bisogno di ulteriori librerie o framework per il suo funzionamento.

CARATTERISTICHE IMPLEMETATE
----------------------------
RESIZE: Il gioco rimarrà al centro del terminale, anche in caso di ridimensionamento della finestra.
    Se il terminale non sarà abbastanza grande per contenere la schermata di gioco, comparirà un avviso che ne richiederà l'espansione.

MUSICA & EFFETTI: La navigazione nei menu e le partite sono accompagnate da colonne sonore ed effetti audio.

MIGLIORI PUNTEGGI: Il gioco tiene traccia dei migliori punteggi ottenuti nelle partite giocate; l'utente a cui viene 
            attribuito un punteggio è il proprietario della sessione corrente.

IMPOSTAZIONI: Il gioco comprende un menù impostazioni dal quale l'utente può cambiare alcuni aspetti del gioco, tra cui:
    - LINGUA: è possibile scegliere la lingua di gioco;
    - DIFFICOLTÀ: è possibile scegliere la difficoltà del gioco;
    - SKIN DELLA RANA: è possibile scegliere l'aspetto della rana che si preferisce;
    - COLORI: è possibile scegliere il colore del menù, e il colore della selezione nel menù;
    - VOLUME: è possibile modificare il volume della musica e degli effetti sonori;
    Le impostazioni correnti vengono mantenute anche ai successivi riavvii del gioco.

PAUSA: È possibile mettere in pausa una partita premendo 'p', ed uscire dalla partita con 'q'.

DETTAGLI IMPLEMENTATIVI
----------------------------
CONTROLLO DEI FILE: Ogni volta che si accede in lettura ai file contenenti impostazioni e migliori punteggi, se ne  
        controlla la correttezza di formato.
        In questo modo si evitano irregolarità di codifica e incongruenze nei valori (ad esempio, saranno gestiti i casi in cui nei file sono presenti valori di un'impostazione non corrispondenti ad alcun settaggio possibile nel gioco).

        La gestione dei dati salvati è dinamica, ne consegue che l'ordine di comparsa di questi dati (impostazioni e migliori punteggi) all'interno dei file, è irrilevante.

TEMPO DURANTE LA PAUSA: La funzione usleep ritorna immediatamente quando il processo riceve un SIGSTOP,
        perciò quando si metteva in pausa il gioco, il timer non scandiva un secondo, ma di meno. 
        Per evitare questo si implementa una funzione che divide il tempo di attesa in intervalli, 
        in modo che il SIGSTOP possa interrompere il processo tra uno e l'altro.

SEGNALAZIONE ERRORI: Nel caso in cui si verifichi un errore (ad esempio nelle chiamate di sistema o nelle funzioni della 
            libreria ncurses) viene eseguita l'uscita dal programma e visualizzato uno specifico messaggio di errore nello standard error.

