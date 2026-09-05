# Documento di progetto

## Versione iniziale - 27 agosto 2026

## Obiettivo

L'obiettivo del progetto e' studiare numericamente la temperatura di equilibrio
su una piastra quadrata. Il dominio viene discretizzato con una griglia, dalla
quale si costruiscono un grafo e un sistema lineare sparso. Si confrontano poi
l'ordinamento naturale dei nodi e un ordinamento ottenuto tramite nested
dissection, osservandone l'effetto sulla fattorizzazione di Cholesky.

## Architettura iniziale

Il progetto verra' suddiviso nei seguenti moduli principali:

1. generazione della griglia e delle coordinate dei punti interni;
2. costruzione del grafo di adiacenza;
3. costruzione dell'ordinamento nested dissection;
4. generazione della matrice sparsa e del termine noto;
5. lettura e soluzione del sistema lineare in Python;
6. analisi sperimentale dei due ordinamenti.

I primi quattro moduli saranno sviluppati in C++. La soluzione numerica e
l'analisi sperimentale saranno realizzate in un notebook Python.

## Dati principali

In questa fase iniziale sono state individuate le seguenti informazioni:

- ogni nodo possiede un identificatore, due indici di griglia e due coordinate;
- ogni arco collega due nodi adiacenti orizzontalmente o verticalmente;
- l'ordinamento associa a ogni nuovo indice l'identificatore originale del nodo;
- la matrice viene memorizzata su file elencando riga, colonna e valore.

Le strutture dati C++ precise verranno scelte e documentate durante lo sviluppo.

## Flusso previsto dei dati

1. Il valore `N` determina la griglia.
2. La griglia produce `coords.txt` e `connectivity.txt`.
3. Il grafo produce `ordering.txt`.
4. Coordinate e ordinamento producono `A.txt` e `rhs.txt`.
5. Il notebook legge i file, risolve il sistema e genera grafici e misure.

## Strategia di sviluppo e verifica

Ogni modulo verra' costruito inizialmente per valori piccoli di `N`, in modo da
poter controllare manualmente i file prodotti. Dopo ogni passo verranno eseguiti
test semplici e il documento verra' aggiornato per descrivere lo stato effettivo
del programma.

## Sviluppo della Task 1 - 29 Agosto 2026

### Generazione della griglia e del file delle coordinate

Il programma della Task 1 riceve in input il numero `N` di punti interni per ciascuna direzione. Il passo della griglia e':
`h = 1 / (N + 1)`.

I punti interni hanno indici `i` e `j` compresi tra `1` e `N`. Le loro
coordinate sono `x = i * h` e `y = j * h`.

I punti vengono visitati riga per riga mediante due cicli annidati: `j` varia nel ciclo esterno e `i` varia nel ciclo interno.

L'identificatore naturale del nodo viene calcolato con la formula:

`n = (j - 1) * N + (i - 1)`.

Questa formula assegna identificatori progressivi da `0` a `N^2 - 1`.
Per ogni nodo il programma scrive nel file `task1/coords.txt` una riga nel
formato:

`n i j x y`

Il file delle coordinate viene generato automaticamente dal programma e non
viene conservato nel repository, perche' puo' essere ricreato eseguendo il
codice.
### Generazione del file delle connessioni

Per ogni nodo interno il programma controlla se esistono il vicino a destra e
il vicino in alto.

Il vicino a destra ha identificatore `n + 1` ed esiste quando `i < N`.
Il vicino in alto ha identificatore `n + N` ed esiste quando `j < N`.

Vengono considerati soltanto destra e alto per rappresentare ogni arco non
orientato una sola volta, evitando duplicati.

Gli archi vengono numerati progressivamente mediante il contatore `e`. Per
ogni arco il programma scrive nel file `task1/connectivity.txt` una riga nel
formato:

`e n1 n2`

I test eseguiti hanno prodotto:
- per `N = 1`: 1 nodo e 0 archi;
- per `N = 2`: 4 nodi e 4 archi;
- per `N = 3`: 9 nodi e 12 archi.

Il file viene generato automaticamente dal programma e non viene conservato
nel repository.

## Sviluppo della Task 2 - 5 settembre 2026

### Lettura dei nodi

Il programma legge i nodi dal file `task1/coords.txt`. Ogni nodo viene
rappresentato mediante la struttura `Nodo`, che contiene:

- l'identificatore originale `n`;
- gli indici di griglia `i` e `j`;
- le coordinate geometriche `x` e `y`.

I nodi vengono memorizzati in un `std::vector<Nodo>`.

La strategia di riordinamento è geometrica e utilizza gli indici `i` e `j`.
Non è quindi necessario leggere esplicitamente `connectivity.txt`: le
connessioni del grafo derivano dalla griglia regolare costruita nella Task 1.

### Ordinamento nested dissection

L'ordinamento viene costruito dalla funzione ricorsiva
`nestedDissection`.

A ogni chiamata la funzione:

1. sceglie la direzione del taglio, utilizzando alternativamente `i` e `j`;
2. determina gli estremi delle coordinate presenti;
3. sceglie una coordinata centrale come separatore;
4. divide i nodi nei vettori `V1`, `V2` e `VS`;
5. applica ricorsivamente lo stesso procedimento a `V1` e `V2`;
6. aggiunge alla fine i nodi del separatore `VS`.

La ricorsione termina quando l'insieme è vuoto oppure contiene un solo nodo.
La direzione viene alternata passando da un taglio verticale a uno
orizzontale e viceversa.

### File di ordinamento

Il risultato viene scritto nel file `task2/ordering.txt` nel formato:

`m n`

dove `m` è il nuovo indice del nodo e `n` è il suo identificatore originale.
Il file è generato automaticamente e non viene conservato nel repository.

Per `N = 3`, l'ordinamento ricorsivo ottenuto è:

`0 6 3 2 8 5 1 4 7`

Sono stati eseguiti test con `N = 1`, `N = 2`, `N = 3` e `N = 4`.
In ogni caso il file contiene esattamente `N^2` righe e ogni identificatore
originale compare una sola volta.

## Questioni ancora aperte


- gestione dell'ordinamento naturale e di quello riordinato;
- verifica dei segni della matrice e del termine noto;
- dimensioni massime eseguibili sul computer disponibile.

Queste questioni verranno risolte e documentate progressivamente.
