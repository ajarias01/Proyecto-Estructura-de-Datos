/**
 * @file Nodo.cpp
 * @brief Implementación de la clase template Nodo para estructuras de datos enlazadas.
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la implementación de la clase template Nodo que se utiliza
 * para crear listas enlazadas simples y dobles en el sistema bancario.
 */

#include "Nodo.h"
#include "Fecha.h"
#include "Movimiento.h"
#include "Cliente.h"
#include "Cuenta.h"

/**
 * @brief Constructor completo de la clase template Nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @param _valor Valor a almacenar en el nodo.
 * @param _siguiente Puntero al siguiente nodo en la lista.
 * @param _anterior Puntero al nodo anterior en la lista.
 * @details Constructor que inicializa un nodo con valor y enlaces tanto al nodo
 * siguiente como al anterior, útil para listas doblemente enlazadas.
 */
template <typename T>
Nodo<T>::Nodo(T _valor, Nodo<T>* _siguiente, Nodo<T>* _anterior) {
    valor = _valor;
    siguiente = _siguiente;
    anterior = _anterior;
}

/**
 * @brief Constructor simplificado de la clase template Nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @param _valor Valor a almacenar en el nodo.
 * @param _siguiente Puntero al siguiente nodo en la lista.
 * @details Constructor que inicializa un nodo con valor y enlace al siguiente nodo,
 * estableciendo el puntero anterior como nullptr. Útil para listas simplemente enlazadas.
 */
template <typename T>
Nodo<T>::Nodo(T _valor, Nodo<T>* _siguiente) {
    valor = _valor;
    siguiente = _siguiente;
    anterior = nullptr;
}

/**
 * @brief Constructor por defecto de la clase template Nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @details Constructor que inicializa un nodo vacío con todos los punteros
 * establecidos a nullptr. El valor se inicializa con el constructor por defecto del tipo T.
 */
template <typename T>
Nodo<T>::Nodo() {
    siguiente = nullptr;
    anterior = nullptr;
}

/**
 * @brief Destructor de la clase template Nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @details Destructor vacío que permite la destrucción automática del nodo.
 * No libera memoria de otros nodos para evitar destrucción en cadena accidental.
 */
template <typename T>
Nodo<T>::~Nodo() {
    // Destructor vacío
}

/**
 * @brief Obtiene el valor almacenado en el nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @return Valor de tipo T almacenado en el nodo.
 * @details Retorna una copia del valor almacenado en el nodo.
 */
template <typename T>
T Nodo<T>::get_valor() {
    return valor;
}

/**
 * @brief Establece el valor del nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @param new_valor Nuevo valor a almacenar en el nodo.
 * @details Asigna un nuevo valor al nodo, reemplazando el valor anterior.
 */
template <typename T>
void Nodo<T>::set_valor(T new_valor) {
    valor = new_valor;
}

/**
 * @brief Obtiene el puntero al siguiente nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @return Puntero al siguiente nodo en la lista, o nullptr si es el último.
 * @details Retorna el puntero al nodo que sigue a este en la estructura enlazada.
 */
template <typename T>
Nodo<T>* Nodo<T>::get_siguiente() {
    return siguiente;
}

/**
 * @brief Establece el puntero al siguiente nodo.
 * @tparam T Tipo de dato que almacena el nodo.
 * @param new_siguiente Puntero al nuevo nodo siguiente.
 * @details Actualiza el enlace al siguiente nodo en la estructura enlazada.
 */
template <typename T>
void Nodo<T>::set_siguiente(Nodo<T>* new_siguiente) {
    siguiente = new_siguiente;
}

/**
 * @brief Obtiene el puntero al nodo anterior.
 * @tparam T Tipo de dato que almacena el nodo.
 * @return Puntero al nodo anterior en la lista, o nullptr si es el primero.
 * @details Retorna el puntero al nodo que precede a este en la estructura doblemente enlazada.
 */
template <typename T>
Nodo<T>* Nodo<T>::get_anterior() {
    return anterior;
}

/**
 * @brief Establece el puntero al nodo anterior.
 * @tparam T Tipo de dato que almacena el nodo.
 * @param new_anterior Puntero al nuevo nodo anterior.
 * @details Actualiza el enlace al nodo anterior en la estructura doblemente enlazada.
 */
template <typename T>
void Nodo<T>::set_anterior(Nodo<T>* new_anterior) {
    anterior = new_anterior;
}

/**
 * @brief Instanciaciones explícitas de la clase template Nodo.
 * @details Se declaran instanciaciones explícitas para los tipos de datos
 * utilizados en el sistema bancario para asegurar la correcta compilación
 * y enlazado de las plantillas.
 */
// Instanciaciones explícitas
template class Nodo<int>;           ///< Nodo para enteros
template class Nodo<double>;        ///< Nodo para números de punto flotante
template class Nodo<Fecha>;         ///< Nodo para objetos Fecha
template class Nodo<Cuenta*>;       ///< Nodo para punteros a Cuenta
template class Nodo<Movimiento>;    ///< Nodo para objetos Movimiento
template class Nodo<Cliente>;       ///< Nodo para objetos Cliente
template class Nodo<Cliente*>;      ///< Nodo para punteros a Cliente