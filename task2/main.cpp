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

//--------------------------------
// FUNZIONE NESTED DISSECTION
//------------------------------

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
 //inizializza max e min
    if (tagliaLungoX) {
        minimo = insieme[0].i;
        massimo = insieme[0].i;
    } else {
        minimo = insieme[0].j;
        massimo = insieme[0].j;
    }
 // scelta del massimo e del minimo
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
    //identifica se si tratta di una colonna ed eventualmente cambia direzione del taglio

    if (minimo == massimo) {
        nestedDissection(insieme, !tagliaLungoX, ordine);
        return;
    }

    int taglio = (minimo + massimo) / 2;
 //scrittura V1 V2 VS
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

    //------------------------------
    //BLOCCO 1 : LETTURA DEI NODI
    //-----------------------------

    std::ifstream coords("../task1/coords.txt");

    if (!coords) {
        std::cout << "Errore: non riesco ad aprire ../task1/coords.txt" << std::endl;
        return 1;
    }

    std::vector<Nodo> nodi;

    Nodo nodo;

    while (coords >> nodo.n >> nodo.i >> nodo.j >> nodo.x >> nodo.y) {
        nodi.push_back(nodo);
    }
    //verifica:
    std::cout << "Ho letto " << nodi.size() << " nodi." << std::endl;
   
 //--------------------------------------------------
 //BLOCCO 2 : PREPARAZIONE E CHIAMATA DELLA FUNZIONE
 //--------------------------------------------------

 std::vector<int> ordine;
 nestedDissection(nodi, true, ordine); //entra nella funzione
  
 //--------------------------------------
 //BLOCCO 3: ESECUZIONE NESTED DISSECTION
 //--------------------------------------


 //--------------------------------------
 // BLOCCO 4 : CREAZIONE ORDERING.TXT
 //-----------------------------------
std::ofstream ordering("ordering.txt");

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
