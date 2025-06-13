#include "GestorClientes.h"
using namespace std;

void GestorClientes::countingSortByChar(ListaDoble<Cliente*>& clientes, int n, int pos, std::function<std::string(Cliente*)> getter) {
    std::vector<Cliente*> output(n);
    int count[256] = {0};
    for (int i = 0; i < n; i++) {
        std::string clave = getter(clientes.get_contador(i));
        char c = (pos < clave.length()) ? tolower(clave[clave.length() - 1 - pos]) : 0;
        count[(unsigned char)c]++;
    }
    for (int i = 1; i < 256; i++)
        count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        std::string clave = getter(clientes.get_contador(i));
        char c = (pos < clave.length()) ? tolower(clave[clave.length() - 1 - pos]) : 0;
        output[--count[(unsigned char)c]] = clientes.get_contador(i);
    }
    for (int i = 0; i < n; i++)
        clientes.set_contador(i, output[i]); // Debes tener un método set(int, Cliente*)
}

void GestorClientes::radixSortStringCampo(ListaDoble<Cliente*>& clientes, int n, std::function<std::string(Cliente*)> getter) {
    int maxLen = 0;
    for (int i = 0; i < n; i++) {
        int len = getter(clientes.get_contador(i)).length();
        if (len > maxLen) maxLen = len;
    }
    for (int pos = 0; pos < maxLen; pos++)
        countingSortByChar(clientes, n, pos, getter);
}

int GestorClientes::getMaxCampoNumerico(const ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter) {
    int mx = getter(clientes.get_contador(0));
    for (int i = 1; i < n; i++)
        if (getter(clientes.get_contador(i)) > mx)
            mx = getter(clientes.get_contador(i));
    return mx;
}

void GestorClientes::countSortCampoNumerico(ListaDoble<Cliente*>& clientes, int n, int exp, std::function<int(Cliente*)> getter) {
    std::vector<Cliente*> output(n);
    int count[10] = {0};
    for (int i = 0; i < n; i++)
        count[(getter(clientes.get_contador(i)) / exp) % 10]++;
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        int idx = (getter(clientes.get_contador(i)) / exp) % 10;
        output[--count[idx]] = clientes.get_contador(i);
    }
    for (int i = 0; i < n; i++)
        clientes.set_contador(i, output[i]);
}

void GestorClientes::radixSortCampoNumerico(ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter) {
    int m = getMaxCampoNumerico(clientes, n, getter);
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSortCampoNumerico(clientes, n, exp, getter);
}