

CARATTERISTICHE IMPLEMETATE
-----------------------------
RESIZE: Il gioco rimarrà al centro del terminale,
    anche in caso di ridimensionamento della finestra.
    Se il terminale non sarà abbastanza grande per contenere il gioco,
    comparirà un avviso che ne richiederà l'espanzione.

MIGLIORI PUNTEGGI: Il gioco tiene traccia dei migliori punteggi
    ottenuti nelle partite giocate.

IMPOSTAZIONI: Il gioco comprende un menù impostazioni dal quale
    l'utente può cambiare alcuni aspetti del gioco, tra cui:
    - LINGUA: è possibile scegliere la lingua di gioco;
    - COLORI: è possibile scegliere di quale colore deve essere il menù,
        e di quale colore deve essere la selezione nel menù;
    - SKIN DELLA RANA: è possibile scegliere l'aspetto della rana che si preferisce;
    - DIFFICOLTÀ: è possibile scegliere la difficoltà del gioco;
    Le impostazioni correnti vengono mantenute anche ai successivi riavvii del gioco.

PAUSA: È possibile mettere in pausa una partita premendo 'p'.


TECNICISMI IMPLEMENTATI
-----------------------------
CONTROLLO DEI FILE: Dei file contenenti le impostazioni e i migliori punteggi,
    durante la loro lettura, se ne controlla il formato,
    in modo che non vi siano irregolarità (come ad esempio un valore di
    un'impostazione non corrispondente ad alcun settaggio possibile nel gioco).
    L'ordine di comparsa nel file delle impostazioni o dei punteggi è indifferente.

TEMPO DURANTE LA PAUSA: La funzione usleep non viene interrotta quando il processo
    riceve un SIGSTOP, perciò quando si metteva in pausa il gioco, il timer aspettava
    parte di quel secondo durante la pausa. Per evitare questo abbiamo implementato
    una funzione che divide il tempo di attesa in intervalli, in modo che il SIGSTOP
    possa interrompere il processo tra uno e l'altro

