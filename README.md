

CARATTERISTICHE IMPLEMETATE
-----------------------------
RESIZE: Il gioco rimarrà al centro del terminale, anche in caso di ridimensionamento della finestra.
    Se il terminale non sarà abbastanza grande per contenere il gioco, comparirà un avviso che ne richiederà l'espanzione.

MUSICA & EFFETTI: Partite e menù sono accompagnate da colonne sonore ed effetti sonori

MIGLIORI PUNTEGGI: Il gioco tiene traccia dei migliori punteggi ottenuti nelle partite giocate.

IMPOSTAZIONI: Il gioco comprende un menù impostazioni dal quale l'utente può cambiare alcuni aspetti del gioco, tra cui:
    - LINGUA: è possibile scegliere la lingua di gioco;
    - DIFFICOLTÀ: è possibile scegliere la difficoltà del gioco;
    - SKIN DELLA RANA: è possibile scegliere l'aspetto della rana che si preferisce;
    - COLORI: è possibile scegliere il colore del menù, e il colore della selezione nel menù;
    - VOLUME: è possibile modificare il volume della musica e degli effetti sonori;
    Le impostazioni correnti vengono mantenute anche ai successivi riavvii del gioco.

PAUSA: È possibile mettere in pausa una partita premendo 'p', ed uscire dalla partita con 'q'.


TECNICISMI IMPLEMENTATI
-----------------------------
CONTROLLO DEI FILE: Dei file contenenti le impostazioni e i migliori punteggi, durante la loro lettura,
    se ne controlla il formato, in modo che non vi siano irregolarità (come ad esempio un valore di
    un'impostazione non corrispondente ad alcun settaggio possibile nel gioco).
    Nel file l'ordine di comparsa delle impostazioni o dei punteggi è indifferente.

TEMPO DURANTE LA PAUSA: La funzione usleep viene interrotta quando il processo riceve un SIGSTOP,
    perciò quando si metteva in pausa il gioco, il timer non scandiva un secondo, ma di meno. 
    Per evitare questo abbiamo implementato una funzione che divide il tempo di attesa in intervalli, 
    in modo che il SIGSTOP possa interrompere il processo tra uno e l'altro.

