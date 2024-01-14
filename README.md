# Frogger VS Plants

## Dipendenze
Per poter utilizzare la musica e gli effetti sonori occorre installare il media player `mpg123` eseguendo il comando:
```
sudo apt install mpg123
```
o in alternativa eseguendo dalla directory del gioco il comando:
```
make install
```
Il progetto non ha bisogno di ulteriori librerie o framework per il suo funzionamento.

## Funzionalità

### Resize:
Il gioco rimarrà al centro del terminale, anche in caso di ridimensionamento della finestra. Se il terminale non sarà abbastanza grande per contenere la schermata di gioco, comparirà un avviso che ne richiederà l'espansione.

### Musica & effetti:
La navigazione nei menu e le partite sono accompagnate da colonne sonore ed effetti audio.

### Migliori punteggi:
Il gioco tiene traccia dei migliori punteggi ottenuti nelle partite giocate; l'utente a cui viene attribuito un punteggio è il proprietario della sessione corrente.

### Impostazioni:
Il gioco comprende un menù impostazioni dal quale l'utente può cambiare alcuni aspetti del gioco, tra cui:
- Lingua: è possibile scegliere la lingua di gioco;
- Difficoltà: è possibile scegliere la difficoltà del gioco;
- Skin della rana: è possibile scegliere l'aspetto della rana che si preferisce;
- Colori: è possibile scegliere il colore del menù, e il colore della selezione nel menù;
- Volume: è possibile modificare il volume della musica e degli effetti sonori;

Le impostazioni correnti vengono mantenute anche ai successivi riavvii del gioco.

### Pausa:
È possibile mettere in pausa una partita premendo `p`, ed uscire dalla partita con `q`.

## Dettagli implementativi
### Controllo integrità dei file:
Ogni volta che si accede in lettura ai file contenenti impostazioni o migliori punteggi, se ne controlla la correttezza di formato. In questo modo si evitano irregolarità di codifica e incongruenze nei valori (ad esempio, saranno gestiti i casi in cui nei file sono presenti valori di un'impostazione non corrispondenti ad alcun settaggio possibile nel gioco). La gestione dei dati salvati è dinamica, ne consegue che l'ordine di comparsa di questi dati (impostazioni e migliori punteggi) all'interno dei file, è irrilevante.

### Tempo durante la pausa:
La funzione `usleep` ritorna immediatamente quando il processo riceve il segnale `SIGSTOP`, perciò quando si metteva in pausa il gioco, il timer non scandiva un secondo, ma di meno. Per evitare questo si implementa una funzione che divide il tempo di attesa in intervalli, in modo che il segnale `SIGSTOP` possa interrompere il processo tra uno e l'altro.

### Segnalazione errori:
Nel caso in cui si verifichi un errore (ad esempio nelle chiamate di sistema o nelle funzioni della libreria ncurses) viene eseguita l'uscita dal programma e visualizzato uno specifico messaggio di errore nello standard error.
