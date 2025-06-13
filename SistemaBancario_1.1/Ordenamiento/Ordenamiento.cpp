#include "ordenamiento.h"

// Ordena los nombres distinguiendo mayúsculas y minúsculas (orden ASCII)
void ordenarNombresASCII(std::string nombres[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (nombres[i] > nombres[j]) {
                std::string temp = nombres[i];
                nombres[i] = nombres[j];
                nombres[j] = temp;
            }
        }
    }
}
void limpiar_linea(const string& mensaje) {
    cout << "\r\033[K" << mensaje;
}
string ingresar_alfabetico(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            palabra += c;
            printf("%c", c);
        } else if (c == 13) { 
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            printf("\b \b");
        } else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
int ingresar_enteros(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        if ((c>='0'&& c<='9') ) {
            palabra += c;
            printf("%c", c);
        } else if (c == 13) {
            return stoi(palabra);
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            printf("\b \b");
        }
    }
}

