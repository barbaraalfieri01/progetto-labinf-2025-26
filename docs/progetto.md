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

## Sviluppo della Task 3 - 23 settembre 2026

### Lettura dei dati e scelta dell'ordinamento

Il programma legge i nodi da `task1/coords.txt` e ricostruisce `N` cercando il
massimo valore degli indici di griglia `i` e `j`. Calcola quindi il passo
`h = 1 / (N + 1)` e il coefficiente `kappa / h^2`, con `kappa = 0.01`.

L'utente puo' scegliere tra due ordinamenti:

- nell'ordinamento naturale, il nuovo indice coincide con l'identificatore
  originale del nodo;
- nell'ordinamento riordinato, il programma legge le associazioni `m n` dal
  file `task2/ordering.txt`.

Il vettore `nuovoIndice` permette di passare dall'identificatore originale di
un nodo al suo indice nel sistema lineare.

### Costruzione della matrice e del termine noto

La matrice viene esportata in `task3/A.txt`. Ogni riga contiene:

`i j A[i,j]`

Per ciascun nodo viene scritto l'elemento diagonale
`-4 * kappa / h^2`. Il programma legge poi gli archi da
`task1/connectivity.txt`: ogni arco non orientato genera due elementi
simmetrici della matrice, entrambi uguali a `kappa / h^2`.

Il termine noto viene esportato in `task3/rhs.txt`. Per ogni nodo viene
calcolata la sorgente

`f(x,y) = exp(-10 * (x^2 + y^2))`

e viene salvato il valore `-f(x,y)`, coerentemente con il sistema
`A * u = -f`. I contributi del bordo sono nulli perche' la condizione assegnata
e' `u0 = 0`.

I file `A.txt` e `rhs.txt` vengono generati automaticamente e non sono
conservati nel repository. Una prima verifica con `N = 2` ha prodotto una
matrice con 4 elementi diagonali e 8 elementi fuori diagonale, corrispondenti
ai 4 archi del grafo rappresentati in entrambe le direzioni.

## Sviluppo della Task 4 - 23 settembre 2026

### Lettura dei file e costruzione della matrice sparsa

La Task 4 viene sviluppata nel notebook Python `task4/task4.ipynb`. Il
notebook legge `task3/A.txt` e `task3/rhs.txt` mediante `numpy.loadtxt`.

Le tre colonne di `A.txt` vengono separate in:

- indici di riga, convertiti in interi;
- indici di colonna, convertiti in interi;
- valori della matrice, mantenuti come numeri decimali.

Questi tre vettori vengono utilizzati per costruire direttamente la matrice
sparsa `A` in formato CSC. La dimensione della matrice viene ricavata dal
numero di elementi del vettore `rhs`.

### Fattorizzazione e soluzione

Per utilizzare CHOLMOD e' stato creato l'ambiente Conda `progetto-labinf`,
contenente Python 3.11, NumPy, SciPy, Matplotlib, scikit-sparse e ipykernel.

Poiche' `A` e' definita negativa, il notebook fattorizza la matrice `-A`:

`-A = L * L^T`

La funzione `calcola_L` richiama la fattorizzazione di
`sksparse.cholmod` con ordinamento interno naturale e restituisce la matrice
triangolare inferiore `L`. Nella versione installata della libreria vengono
utilizzati il parametro `ordering_method="natural"` e il metodo `factor.L()`.

Il sistema originale `A * u = rhs` viene trasformato in
`(-A) * u = -rhs` e risolto in due passaggi con
`scipy.sparse.linalg.spsolve_triangular`:

1. soluzione di `L * y = -rhs`;
2. soluzione di `L^T * u = y`.



