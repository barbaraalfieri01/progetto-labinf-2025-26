#include <iostream>

int main() {
    int N;

    std::cout << "Inserisci il numero di punti interni per lato N: ";
    std::cin >> N;

    double h = 1.0 / (N + 1);

    std::cout << "Numero totale di nodi interni: " << N * N << std::endl;
    std::cout << "Passo della griglia h: " << h << std::endl;

    std::cout << "Nodi interni della griglia: " << std::endl;

for (int j = 1; j <=N; j++){
    double y = j*h;

for (int i = 1; i <= N; i++) {
    int n = (j - 1) * N + (i - 1);
    double x = i * h;
    std::cout << n << " "
              << i << " "
              << j << " "
              << x << " "
              << y << std::endl;
}
}
    return 0;
}