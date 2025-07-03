#ifndef GESTORCLIENTES_H
#define GESTORCLIENTES_H
#include <iostream>
#include "Cliente.h"
#include <iostream>
#include <string>
#include <functional>
#include <vector>

class GestorClientes {
public:
    void countingSortByChar(ListaDoble<Cliente*>& clientes, int n, int pos, std::function<std::string(Cliente*)> getter);
    void radixSortStringCampo(ListaDoble<Cliente*>& clientes, int n, std::function<std::string(Cliente*)> getter);
    int getMaxCampoNumerico(const ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter);
    void countSortCampoNumerico(ListaDoble<Cliente*>& clientes, int n, int exp, std::function<int(Cliente*)> getter);
    void radixSortCampoNumerico(ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter);
    int busquedaBinariaNumerica(ListaDoble<Cliente*>& lista, int n, int valor, std::function<int(Cliente*)> getter, bool ascendente = true);
    int busquedaBinariaString(ListaDoble<Cliente*>& lista, int n, const std::string& valor, std::function<std::string(Cliente*)> getter, bool ascendente = true);
    static int dateKey(const Fecha& f);
    static int dateKey(const Movimiento& m);
    static void radixSortFecha(std::vector<Movimiento>& arr);
    static int lowerBound(const std::vector<Movimiento>& arr, int target);
    static int upperBound(const std::vector<Movimiento>& arr, int target);
};


#endif