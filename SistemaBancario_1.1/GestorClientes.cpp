/**
 * @file GestorClientes.cpp
 * @brief Implementación de utilidades para ordenamiento y búsqueda de clientes y movimientos.
 *
 * Este archivo contiene funciones para ordenar listas de clientes y movimientos por diferentes campos,
 * utilizando algoritmos de radix sort y counting sort, así como funciones de búsqueda binaria.
 */

#include "GestorClientes.h"
#include <algorithm>
#include <cctype>
using namespace std;

/**
 * @brief Obtiene una clave numérica (YYYYMMDD) a partir de una fecha.
 * @param f Fecha
 * @return Clave numérica de la fecha
 */
int GestorClientes::dateKey(const Fecha& f) {
    return f.get_anuario() * 10000 + f.get_mes() * 100 + f.get_dia();
}

/**
 * @brief Obtiene una clave numérica (YYYYMMDD) a partir de un movimiento.
 * @param m Movimiento
 * @return Clave numérica de la fecha del movimiento
 */
int GestorClientes::dateKey(const Movimiento& m) {
    return dateKey(m.get_fecha());
}

/**
 * @brief Counting sort por carácter para ordenar clientes por un campo string.
 * @param clientes Lista de clientes
 * @param n Número de clientes
 * @param pos Posición del carácter (de derecha a izquierda)
 * @param getter Función para obtener el campo string del cliente
 */
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

/**
 * @brief Radix sort para ordenar clientes por un campo string.
 * @param clientes Lista de clientes
 * @param n Número de clientes
 * @param getter Función para obtener el campo string del cliente
 */
void GestorClientes::radixSortStringCampo(ListaDoble<Cliente*>& clientes, int n, std::function<std::string(Cliente*)> getter) {
    int maxLen = 0;
    for (int i = 0; i < n; i++) {
        int len = getter(clientes.get_contador(i)).length();
        if (len > maxLen) maxLen = len;
    }
    for (int pos = 0; pos < maxLen; pos++)
        countingSortByChar(clientes, n, pos, getter);
}

/**
 * @brief Obtiene el valor máximo de un campo numérico en la lista de clientes.
 * @param clientes Lista de clientes
 * @param n Número de clientes
 * @param getter Función para obtener el campo numérico del cliente
 * @return Valor máximo encontrado
 */
int GestorClientes::getMaxCampoNumerico(const ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter) {
    int mx = getter(clientes.get_contador(0));
    for (int i = 1; i < n; i++)
        if (getter(clientes.get_contador(i)) > mx)
            mx = getter(clientes.get_contador(i));
    return mx;
}

/**
 * @brief Counting sort para un dígito específico de un campo numérico.
 * @param clientes Lista de clientes
 * @param n Número de clientes
 * @param exp Exponente del dígito (1, 10, 100, ...)
 * @param getter Función para obtener el campo numérico del cliente
 */
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

/**
 * @brief Radix sort para ordenar clientes por un campo numérico.
 * @param clientes Lista de clientes
 * @param n Número de clientes
 * @param getter Función para obtener el campo numérico del cliente
 */
void GestorClientes::radixSortCampoNumerico(ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter) {
    int m = getMaxCampoNumerico(clientes, n, getter);
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSortCampoNumerico(clientes, n, exp, getter);
}

/**
 * @brief Radix sort para ordenar un vector de movimientos por fecha.
 * @param arr Vector de movimientos
 */
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

/**
 * @brief Búsqueda binaria para encontrar el primer movimiento con fecha >= target.
 * @param arr Vector de movimientos
 * @param target Clave numérica de la fecha objetivo
 * @return Índice del primer movimiento >= target
 */
int GestorClientes::lowerBound(const std::vector<Movimiento>& arr, int target) {
    int lo = 0, hi = (int)arr.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (dateKey(arr[mid]) < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/**
 * @brief Búsqueda binaria para encontrar el primer movimiento con fecha > target.
 * @param arr Vector de movimientos
 * @param target Clave numérica de la fecha objetivo
 * @return Índice del primer movimiento > target
 */
int GestorClientes::upperBound(const std::vector<Movimiento>& arr, int target) {
    int lo = 0, hi = (int)arr.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (dateKey(arr[mid]) <= target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/**
 * @brief Búsqueda binaria para encontrar un cliente por campo numérico.
 * @param lista Lista de clientes ordenada por el campo numérico
 * @param n Número de clientes
 * @param valor Valor a buscar
 * @param getter Función para obtener el campo numérico del cliente
 * @param ascendente true si la lista está ordenada ascendente, false para descendente
 * @return Índice del cliente encontrado, -1 si no se encuentra
 */
int GestorClientes::busquedaBinariaNumerica(ListaDoble<Cliente*>& lista, int n, int valor, std::function<int(Cliente*)> getter, bool ascendente) {
    int izq = 0, der = n - 1;
    while (izq <= der) {
        int medio = izq + (der - izq) / 2;
        int valorMedio = getter(lista.get_contador(medio));
        
        if (valorMedio == valor) {
            return medio;
        } else if (ascendente ? (valorMedio < valor) : (valorMedio > valor)) {
            izq = medio + 1;
        } else {
            der = medio - 1;
        }
    }
    return -1;
}

/**
 * @brief Búsqueda binaria para encontrar un cliente por campo string.
 * @param lista Lista de clientes ordenada por el campo string
 * @param n Número de clientes
 * @param valor Valor a buscar
 * @param getter Función para obtener el campo string del cliente
 * @param ascendente true si la lista está ordenada ascendente, false para descendente
 * @return Índice del cliente encontrado, -1 si no se encuentra
 */
int GestorClientes::busquedaBinariaString(ListaDoble<Cliente*>& lista, int n, const std::string& valor, std::function<std::string(Cliente*)> getter, bool ascendente) {
    int izq = 0, der = n - 1;
    while (izq <= der) {
        int medio = izq + (der - izq) / 2;
        std::string valorMedio = getter(lista.get_contador(medio));
        
        // Convertir a minúsculas para comparación insensible a mayúsculas
        std::string valorMedioLower = valorMedio;
        std::string valorBusquedaLower = valor;
        std::transform(valorMedioLower.begin(), valorMedioLower.end(), valorMedioLower.begin(), ::tolower);
        std::transform(valorBusquedaLower.begin(), valorBusquedaLower.end(), valorBusquedaLower.begin(), ::tolower);
        
        if (valorMedioLower == valorBusquedaLower) {
            return medio;
        } else if (ascendente ? (valorMedioLower < valorBusquedaLower) : (valorMedioLower > valorBusquedaLower)) {
            izq = medio + 1;
        } else {
            der = medio - 1;
        }
    }
    return -1;
}