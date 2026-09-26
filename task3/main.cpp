#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

struct Nodo {
    int n;
    int i;
    int j;
    double x;
    double y;
};

int main() {
    // --------------------------------------------------
    // 1. Lettura dei nodi da coords.txt
    // --------------------------------------------------

    std::ifstream coords("task1/coords.txt");

    if (!coords) {
        std::cout << "Errore nell'apertura di coords.txt."
                  << std::endl;
        return 1;
    }

    std::vector<Nodo> nodi;
    Nodo nodo;

    while (coords >> nodo.n
                  >> nodo.i
                  >> nodo.j
                  >> nodo.x
                  >> nodo.y) {
        nodi.push_back(nodo);
    }

    coords.close();

    if (nodi.empty()) {
        std::cout << "coords.txt non contiene nodi."
                  << std::endl;
        return 1;
    }

    std::cout << "Ho letto "
              << nodi.size()
              << " nodi."
              << std::endl;

    // --------------------------------------------------
    // 2. Ricostruzione di N
    // --------------------------------------------------

    int N = 0;

    for (const Nodo& nodo : nodi) {
        if (nodo.i > N) {
            N = nodo.i;
        }
    }

    double h = 1.0 / (N + 1);
    double kappa = 0.01;
    double coefficiente = kappa / (h * h);

    // --------------------------------------------------
    // 3. Scelta dell'ordinamento
    // --------------------------------------------------

    int scelta;

    std::cout << "Scegli l'ordinamento:" << std::endl;
    std::cout << "0 - ordinamento naturale" << std::endl;
    std::cout << "1 - ordinamento di ordering.txt" << std::endl;
    std::cin >> scelta;

    std::vector<int> nuovoIndice(nodi.size());

    if (scelta == 0) {
        // Nell'ordinamento naturale il nuovo indice
        // coincide con quello originale.
        for (Nodo nodo : nodi) {
            nuovoIndice[nodo.n] = nodo.n;
        }
    } else if (scelta == 1) {
        std::ifstream ordering("task2/ordering.txt");

        if (!ordering) {
            std::cout << "Errore nell'apertura di ordering.txt."
                      << std::endl;
            return 1;
        }

        int m;
        int n;

        while (ordering >> m >> n) {
            nuovoIndice[n] = m;
        }

        ordering.close();
    } else {
        std::cout << "Scelta non valida." << std::endl;
        return 1;
    }

    // --------------------------------------------------
    // 4. Apertura di connectivity.txt
    // --------------------------------------------------

    std::ifstream connectivity("task1/connectivity.txt");

    if (!connectivity) {
        std::cout << "Errore nell'apertura di connectivity.txt."
                  << std::endl;
        return 1;
    }

    // --------------------------------------------------
    // 5. Creazione dei file A.txt e rhs.txt
    // --------------------------------------------------

    std::ofstream matriceA("task3/A.txt");
    std::ofstream rhsFile("task3/rhs.txt");

    if (!matriceA || !rhsFile) {
        std::cout << "Errore nella creazione dei file di output."
                  << std::endl;
        return 1;
    }

    // Conserviamo più cifre decimali nei file.
    matriceA << std::setprecision(17);
    rhsFile << std::setprecision(17);

    std::vector<double> rhs(nodi.size());

    // --------------------------------------------------
    // 6. Elementi diagonali di A e valori di rhs
    // --------------------------------------------------

    for (Nodo nodo : nodi) {
        int riga = nuovoIndice[nodo.n];

        // Elemento diagonale:
        // A[riga,riga] = -4 * kappa / h^2
        matriceA << riga << " "
                 << riga << " "
                 << -4.0 * coefficiente
                 << std::endl;

        // Funzione sorgente:
        // f(x,y) = exp(-10 * (x^2 + y^2))
        double f = std::exp(
            -10.0 * (nodo.x * nodo.x +
                     nodo.y * nodo.y)
        );

        // Il sistema è A*u = -f.
        // I contributi del bordo sono nulli perché u_bordo = 0.
        rhs[riga] = -f;
    }

    // --------------------------------------------------
    // 7. Elementi di A relativi alle connessioni
    // --------------------------------------------------

    int e;
    int n1;
    int n2;

    while (connectivity >> e >> n1 >> n2) {
        int indice1 = nuovoIndice[n1];
        int indice2 = nuovoIndice[n2];

        // Ogni arco non orientato produce due elementi
        // simmetrici della matrice.
        matriceA << indice1 << " "
                 << indice2 << " "
                 << coefficiente
                 << std::endl;

        matriceA << indice2 << " "
                 << indice1 << " "
                 << coefficiente
                 << std::endl;
    }

    connectivity.close();

    // --------------------------------------------------
    // 8. Scrittura di rhs.txt nell'ordine delle righe
    // --------------------------------------------------

    for (double valoreRhs : rhs) {
        rhsFile << valoreRhs << std::endl;
    }

    matriceA.close();
    rhsFile.close();

    std::cout << "File A.txt creato." << std::endl;
    std::cout << "File rhs.txt creato." << std::endl;

    return 0;
}
