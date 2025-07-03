#include "GestorClientes.h"
using namespace std;

int GestorClientes::dateKey(const Fecha& f) {
    return f.get_anuario() * 10000 + f.get_mes() * 100 + f.get_dia();
}

int GestorClientes::dateKey(const Movimiento& m) {
    return dateKey(m.get_fecha());
}
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
void GestorClientes::radixSortFecha(std::vector<Movimiento>& arr) {
    size_t n = arr.size();
    std::vector<Movimiento> output(n);
    int count[10] = {0};

    // Encontrar el máximo para determinar el número de dígitos
    int maxVal = dateKey(arr[0]);
    for (size_t i = 1; i < n; ++i) {
        int val = dateKey(arr[i]);
        if (val > maxVal) maxVal = val;
    }

    // Aplicar counting sort por cada dígito
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        std::fill(count, count + 10, 0);
        // Contar ocurrencias
        for (size_t i = 0; i < n; ++i)
            count[(dateKey(arr[i]) / exp) % 10]++;
        // Acumular posiciones
        for (int i = 1; i < 10; ++i)
            count[i] += count[i - 1];
        // Construir arreglo ordenado
        for (int i = n - 1; i >= 0; --i) {
            int idx = (dateKey(arr[i]) / exp) % 10;
            output[--count[idx]] = arr[i];
        }
        // Copiar de vuelta al arreglo original
        for (size_t i = 0; i < n; ++i)
            arr[i] = output[i];
    }
}

int GestorClientes::lowerBound(const std::vector<Movimiento>& arr, int target) {
    int lo = 0, hi = (int)arr.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (dateKey(arr[mid]) < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

int GestorClientes::upperBound(const std::vector<Movimiento>& arr, int target) {
    int lo = 0, hi = (int)arr.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (dateKey(arr[mid]) <= target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}