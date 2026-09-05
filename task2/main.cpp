#include <iostream>
#include <fstream>
#include <vector>

struct Nodo {
    int n;
    int i;
    int j;
    double x;
    double y;
};

void nestedDissection(
    const std::vector<Nodo>& insieme,
    bool tagliaLungoX,
    std::vector<int>& ordine
) {
    if (insieme.empty()) {
        return;
    }

    if (insieme.size() == 1) {
        ordine.push_back(insieme[0].n);
        return;
    }

    int minimo;
    int massimo;

    if (tagliaLungoX) {
        minimo = insieme[0].i;
        massimo = insieme[0].i;
    } else {
        minimo = insieme[0].j;
        massimo = insieme[0].j;
    }

    for (auto nodo : insieme) {
        int coordinata;

        if (tagliaLungoX) {
            coordinata = nodo.i;
        } else {
            coordinata = nodo.j;
        }

        if (coordinata < minimo) {
            minimo = coordinata;
        }

        if (coordinata > massimo) {
            massimo = coordinata;
        }
    }

    if (minimo == massimo) {
        nestedDissection(insieme, !tagliaLungoX, ordine);
        return;
    }

    int taglio = (minimo + massimo) / 2;

    std::vector<Nodo> V1;
    std::vector<Nodo> V2;
    std::vector<Nodo> VS;

    for (auto nodo : insieme) {
        int coordinata;

        if (tagliaLungoX) {
            coordinata = nodo.i;
        } else {
            coordinata = nodo.j;
        }

        if (coordinata < taglio) {
            V1.push_back(nodo);
        } else if (coordinata > taglio) {
            V2.push_back(nodo);
        } else {
            VS.push_back(nodo);
        }
    }

    nestedDissection(V1, !tagliaLungoX, ordine);
    nestedDissection(V2, !tagliaLungoX, ordine);

    for (auto nodo : VS) {
        ordine.push_back(nodo.n);
    }
}

int main() {
    std::ifstream coords("task1/coords.txt");

    if (!coords) {
        std::cout << "Errore: non riesco ad aprire task1/coords.txt" << std::endl;
        return 1;
    }

    std::vector<Nodo> nodi;

    Nodo nodo;

    while (coords >> nodo.n >> nodo.i >> nodo.j >> nodo.x >> nodo.y) {
        nodi.push_back(nodo);
    }

    std::cout << "Ho letto " << nodi.size() << " nodi." << std::endl;
    for (auto nodo : nodi) {
    std::cout << nodo.n << " "
              << nodo.i << " "
              << nodo.j << " "
              << nodo.x << " "
              << nodo.y << std::endl;
}
int N = 0;

for (auto nodo : nodi) {
    if (nodo.i > N) {
        N = nodo.i;
    }
}

std::cout << "N ricostruito dal file: " << N << std::endl;
std::vector<int> ordine;

nestedDissection(nodi, true, ordine);

std::ofstream ordering("task2/ordering.txt");

if (!ordering) {
    std::cout << "Errore nella creazione di ordering.txt" << std::endl;
    return 1;
}

int m = 0;

for (int n : ordine) {
    ordering << m << " " << n << std::endl;
    m++;
}

ordering.close();
std::cout << "File ordering.txt creato." << std::endl;

    return 0;
}