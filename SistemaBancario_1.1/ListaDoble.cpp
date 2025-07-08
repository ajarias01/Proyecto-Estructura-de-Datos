/**
 * @file ListaDoble.cpp
 * @brief Implementación de la clase template ListaDoble para listas doblemente enlazadas circulares.
 *
 * Este archivo contiene la implementación de la clase ListaDoble, que permite gestionar listas doblemente enlazadas
 * circulares genéricas, con operaciones de inserción, eliminación, recorrido, filtrado y acceso por índice.
 */

#include "ListaDoble.h"
#include "Fecha.h"
#include "Movimiento.h"
#include "Cliente.h"
#include <iostream>
#include <stdexcept>

/**
 * @brief Constructor de ListaDoble con cabeza inicial.
 * @param _cabeza Nodo cabeza inicial
 */
template <typename T>
ListaDoble<T>::ListaDoble(Nodo<T>* _cabeza) {
    cabeza = _cabeza;
    cola = _cabeza;
    if (cabeza) {
        cabeza->set_siguiente(cabeza);
        cabeza->set_anterior(cabeza);
    }
}

/**
 * @brief Constructor por defecto de ListaDoble.
 */
template <typename T>
ListaDoble<T>::ListaDoble() {
    cabeza = nullptr;
    cola = nullptr;
}

/**
 * @brief Destructor de ListaDoble. Libera todos los nodos y sus valores si son punteros.
 */
template <typename T>
ListaDoble<T>::~ListaDoble() {
    if (!esta_vacia()) {
        Nodo<T>* actual = cabeza;
        Nodo<T>* siguiente;
        do {
            siguiente = actual->get_siguiente();
            // Solo elimina si T es un puntero
            if constexpr (std::is_pointer_v<T>) {
                if (actual->get_valor() != nullptr) {
                    delete actual->get_valor();
                }
            }
            delete actual;
            actual = siguiente;
        } while (actual != cabeza);
    }
}

/**
 * @brief Obtiene el nodo cabeza de la lista.
 * @return Puntero al nodo cabeza
 */
template <typename T>
Nodo<T>* ListaDoble<T>::get_cabeza() {
    return cabeza;
}

/**
 * @brief Establece el nodo cabeza de la lista.
 * @param nueva_cabeza Nuevo nodo cabeza
 */
template <typename T>
void ListaDoble<T>::set_cabeza(Nodo<T>* nueva_cabeza) {
    cabeza = nueva_cabeza;
}

/**
 * @brief Obtiene el nodo cola de la lista.
 * @return Puntero al nodo cola
 */
template <typename T>
Nodo<T>* ListaDoble<T>::get_cola() {
    return cola;
}

/**
 * @brief Establece el nodo cola de la lista.
 * @param nueva_cola Nuevo nodo cola
 */
template <typename T>
void ListaDoble<T>::set_cola(Nodo<T>* nueva_cola) {
    cola = nueva_cola;
}

/**
 * @brief Inserta un valor al final de la lista.
 * @param valor Valor a insertar
 */
template <typename T>
void ListaDoble<T>::insertar_cola(T valor) {
    try {
        Nodo<T>* nuevo_nodo = new Nodo<T>(valor, nullptr, nullptr);
        if (esta_vacia()) {
            cabeza = nuevo_nodo;
            cola = nuevo_nodo;
            cabeza->set_siguiente(cabeza);
            cabeza->set_anterior(cabeza);
        } else {
            nuevo_nodo->set_anterior(cola);
            nuevo_nodo->set_siguiente(cabeza);
            cola->set_siguiente(nuevo_nodo);
            cabeza->set_anterior(nuevo_nodo);
            cola = nuevo_nodo;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en insertar_cola: " << e.what() << std::endl;
    }
}

/**
 * @brief Imprime todos los valores de la lista por consola.
 */
template <typename T>
void ListaDoble<T>::imprimir() {
    if (esta_vacia()) {
        std::cout << "Lista vacía\n";
        return;
    }
    Nodo<T>* aux = cabeza;
    do {
        std::cout << aux->get_valor() << " "; // Usa operador<< para todos los tipos
        aux = aux->get_siguiente();
    } while (aux != cabeza);
    std::cout << std::endl;
}

/**
 * @brief Verifica si la lista está vacía.
 * @return true si la lista está vacía, false en caso contrario
 */
template <typename T>
bool ListaDoble<T>::esta_vacia() {
    return (cabeza == nullptr);
}

/**
 * @brief Recorre la lista y aplica una función a cada elemento.
 * @param func Función a aplicar
 */
template <typename T>
void ListaDoble<T>::recorrer(std::function<void(T)> func)const {
    if (esta_vacia()) return;
    Nodo<T>* aux = cabeza;
    do {
        func(aux->get_valor());
        aux = aux->get_siguiente();
    } while (aux != cabeza);
}

/**
 * @brief Filtra la lista aplicando un criterio y ejecuta una acción sobre los elementos que cumplen.
 * @param criterio Función booleana de filtrado
 * @param accion Acción a ejecutar sobre los elementos filtrados
 */
template <typename T>
void ListaDoble<T>::filtrar(std::function<bool(T)> criterio, std::function<void(T)> accion) {
    if (esta_vacia()) return;
    Nodo<T>* aux = cabeza;
    do {
        if (criterio(aux->get_valor())) {
            accion(aux->get_valor());
        }
        aux = aux->get_siguiente();
    } while (aux != cabeza);
}

/**
 * @brief Busca el primer nodo que cumple un criterio.
 * @param criterio Función booleana de búsqueda
 * @return Puntero al nodo encontrado o nullptr si no existe
 */
template <typename T>
Nodo<T>* ListaDoble<T>::buscar(std::function<bool(T)> criterio) {
    if (esta_vacia()) return nullptr;
    Nodo<T>* aux = cabeza;
    do {
        if (criterio(aux->get_valor())) {
            return aux;
        }
        aux = aux->get_siguiente();
    } while (aux != cabeza);
    return nullptr;
}

/**
 * @brief Elimina un nodo de la lista, liberando memoria y el valor si es puntero.
 * @param nodo Nodo a eliminar
 */
template <typename T>
void ListaDoble<T>::eliminar(Nodo<T>* nodo) {
    try {
        if (!nodo || esta_vacia()) throw std::invalid_argument("Nodo inválido o lista vacía");
        if (nodo == cabeza && cabeza == cola) {
            if constexpr (std::is_pointer<T>::value) {
                delete nodo->get_valor(); // Solo para punteros
            }
            cabeza = nullptr;
            cola = nullptr;
            delete nodo;
            return;
        }
        Nodo<T>* anterior = nodo->get_anterior();
        Nodo<T>* siguiente = nodo->get_siguiente();
        anterior->set_siguiente(siguiente);
        siguiente->set_anterior(anterior);
        if (nodo == cabeza) cabeza = siguiente;
        if (nodo == cola) cola = anterior;
        if constexpr (std::is_pointer<T>::value) {
            delete nodo->get_valor(); // Solo para punteros
        }
        delete nodo;
    } catch (const std::exception& e) {
        std::cerr << "Error en eliminar: " << e.what() << std::endl;
    }
}

/**
 * @brief Verifica si la lista está vacía (versión const).
 * @return true si la lista está vacía, false en caso contrario
 */
template <typename T>
bool ListaDoble<T>::esta_vacia() const { // Añadir const aquí
    return (cabeza == nullptr);
}

/**
 * @brief Obtiene el valor en la posición dada.
 * @param index Índice del elemento
 * @return Valor en la posición indicada
 * @throws std::out_of_range si el índice es inválido
 */
template <typename T>
T ListaDoble<T>::get_contador(int index) const {
    if (esta_vacia()) throw std::out_of_range("Lista vacía");
    Nodo<T>* aux = cabeza;
    int i = 0;
    do {
        if (i == index) return aux->get_valor();
        aux = aux->get_siguiente();
        i++;
    } while (aux != cabeza);
    throw std::out_of_range("Índice fuera de rango");
}

/**
 * @brief Establece el valor en la posición dada.
 * @param index Índice del elemento
 * @param valor Nuevo valor a establecer
 * @throws std::out_of_range si el índice es inválido
 */
template <typename T>
void ListaDoble<T>::set_contador(int index, T valor) {
    if (esta_vacia()) throw std::out_of_range("Lista vacía");
    Nodo<T>* aux = cabeza;
    int i = 0;
    do {
        if (i == index) {
            aux->set_valor(valor);
            return;
        }
        aux = aux->get_siguiente();
        i++;
    } while (aux != cabeza);
    throw std::out_of_range("Índice fuera de rango");
}

/**
 * @brief Obtiene el tamaño (cantidad de elementos) de la lista.
 * @return Número de elementos en la lista
 */
template <typename T>
int ListaDoble<T>::getTam() const {
    if (esta_vacia()) return 0;
    int count = 0;
    Nodo<T>* aux = cabeza;
    do {
        count++;
        aux = aux->get_siguiente();
    } while (aux != cabeza);
    return count;
}
// Instanciaciones explícitas
template class ListaDoble<int>;
template class ListaDoble<double>;
template class ListaDoble<Fecha>;
template class ListaDoble<Cuenta*>;
template class ListaDoble<Movimiento>;
template class ListaDoble<Cliente*>;