#include <iostream>

int main() {
    int N;

    std::cout << "Inserisci il numero di punti interni per lato N: ";
    std::cin >> N;

    double h = 1.0 / (N + 1);

    std::cout << "Numero totale di nodi interni: " << N * N << std::endl;
    std::cout << "Passo della griglia h: " << h << std::endl;

    return 0;
}