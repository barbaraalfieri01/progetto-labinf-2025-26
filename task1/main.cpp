#include <iostream>
#include <fstream>

int main() {
    int N;

    std::cout << "Inserisci il numero di punti interni per lato N: ";
    std::cin >> N;

    double h = 1.0 / (N + 1);
    std::ofstream coords("task1/coords.txt");
    std::ofstream connectivity("task1/connectivity.txt");
int e = 0;

    std::cout << "Numero totale di nodi interni: " << N * N << std::endl;
    std::cout << "Passo della griglia h: " << h << std::endl;

for (int j = 1; j <=N; j++){
    double y = j*h;

for (int i = 1; i <= N; i++) {
    int n = (j - 1) * N + (i - 1);
    double x = i * h;
    coords << n << " "
              << i << " "
              << j << " "
              << x << " "
              << y << std::endl;

if (i < N) {
    connectivity << e << " " << n << " " << n + 1 << std::endl;
    e++;
}

if (j < N) {
    connectivity << e << " " << n << " " << n + N << std::endl;
    e++;
}

}
}
coords.close();
connectivity.close();
std::cout << "File coords.txt creato." << std::endl;
std::cout << "File connectivity.txt creato." << std::endl;
    return 0;
}