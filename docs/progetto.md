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

## Questioni ancora aperte

- formula esatta usata per associare gli indici di griglia agli identificatori;
- scelta del separatore e condizioni di arresto della ricorsione;
- gestione dell'ordinamento naturale e di quello riordinato;
- verifica dei segni della matrice e del termine noto;
- dimensioni massime eseguibili sul computer disponibile.

Queste questioni verranno risolte e documentate progressivamente.
