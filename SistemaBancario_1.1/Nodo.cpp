#include "Nodo.h"
#include "Fecha.h"
#include "Movimiento.h"
#include "Cliente.h"
#include "Cuenta.h"

template <typename T>
Nodo<T>::Nodo(T _valor, Nodo<T>* _siguiente, Nodo<T>* _anterior) {
    valor = _valor;
    siguiente = _siguiente;
    anterior = _anterior;
}

template <typename T>
Nodo<T>::Nodo(T _valor, Nodo<T>* _siguiente) {
    valor = _valor;
    siguiente = _siguiente;
    anterior = nullptr;
}

template <typename T>
Nodo<T>::Nodo() {
    siguiente = nullptr;
    anterior = nullptr;
}

template <typename T>
Nodo<T>::~Nodo() {
    // Destructor vacío
}

template <typename T>
T Nodo<T>::get_valor() {
    return valor;
}

template <typename T>
void Nodo<T>::set_valor(T new_valor) {
    valor = new_valor;
}

template <typename T>
Nodo<T>* Nodo<T>::get_siguiente() {
    return siguiente;
}

template <typename T>
void Nodo<T>::set_siguiente(Nodo<T>* new_siguiente) {
    siguiente = new_siguiente;
}

template <typename T>
Nodo<T>* Nodo<T>::get_anterior() {
    return anterior;
}

template <typename T>
void Nodo<T>::set_anterior(Nodo<T>* new_anterior) {
    anterior = new_anterior;
}

// Instanciaciones explícitas
template class Nodo<int>;
template class Nodo<double>;
template class Nodo<Fecha>;
template class Nodo<Cuenta*>;
template class Nodo<Movimiento>;
template class Nodo<Cliente>;
template class Nodo<Cliente*>;