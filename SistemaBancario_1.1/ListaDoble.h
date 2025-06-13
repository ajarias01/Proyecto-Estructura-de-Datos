#ifndef LISTADOBLE_H_INCLUDED
#define LISTADOBLE_H_INCLUDED

#include "Nodo.h"
#include <functional>
#include <type_traits> // Añadido

template <typename T>
class ListaDoble {
public:
    ListaDoble(Nodo<T>* _cabeza);
    ListaDoble();
    ~ListaDoble();
    Nodo<T>* get_cabeza();
    void set_cabeza(Nodo<T>*);
    Nodo<T>* get_cola();
    void set_cola(Nodo<T>*);
    void insertar_cola(T valor);
    void imprimir();
    bool esta_vacia();
    void recorrer(std::function<void(T)> func) const;
    void filtrar(std::function<bool(T)> criterio, std::function<void(T)> accion);
    Nodo<T>* buscar(std::function<bool(T)> criterio);
    void eliminar(Nodo<T>* nodo);
    bool esta_vacia() const;
    int getTam() const;
    T get_contador(int index) const;
    void set_contador(int index, T valor);
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
};

#endif