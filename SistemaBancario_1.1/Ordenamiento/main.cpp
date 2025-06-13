#include <iostream>
#include <string>
#include "ordenamiento.h"
using namespace std;
#define MAX_NOMBRES 100

int main() {
    system("chcp 65001 > nul");
    int n;
    string nombres[MAX_NOMBRES];

    // Validar cantidad de nombres (solo enteros positivos)
    do {
        limpiar_linea("Ingrese la cantidad de nombres a ordenar (máximo " + to_string(MAX_NOMBRES) + "): ");
        n = ingresar_enteros("");
    } while (n <= 0 || n > MAX_NOMBRES);

    // Ingreso y validación de nombres (solo letras)
    for (int i = 0; i < n; ++i) {
        limpiar_linea("Ingrese el nombre #" + to_string(i + 1) + ": ");
        nombres[i] = ingresar_alfabetico("");
        if (nombres[i].empty()) {
            std::cout << "Nombre vacío. Intente de nuevo.\n";
            --i;
        }
    }

    ordenarNombresASCII(nombres, n);

    std::cout << "\nNombres ordenados (mayúsculas antes que minúsculas):\n";
    for (int i = 0; i < n; ++i) {
        std::cout << (i + 1) << ". " << nombres[i] << std::endl;
    }

    return 0;
}