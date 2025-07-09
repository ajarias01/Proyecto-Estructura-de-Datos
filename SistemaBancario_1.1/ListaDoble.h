/**
 * @file ListaDoble.h
 * @brief Declaración de la clase template ListaDoble para listas doblemente enlazadas genéricas.
 * @author 
 * @date 
 *
 * Esta clase implementa una lista doblemente enlazada con operaciones de inserción, búsqueda, filtrado,
 * recorrido y manipulación de nodos. Permite el uso de funciones lambda para operaciones personalizadas.
 */
#ifndef LISTADOBLE_H_INCLUDED
#define LISTADOBLE_H_INCLUDED

#include "Nodo.h"
#include <functional>
#include <type_traits>

/**
 * @class ListaDoble
 * @brief Clase template para gestionar listas doblemente enlazadas de tipo T.
 * @tparam T Tipo de dato almacenado en la lista.
 */
template <typename T>
class ListaDoble {
public:
    /**
     * @brief Constructor que inicializa la lista con una cabeza dada.
     * @param _cabeza Puntero al nodo cabeza inicial.
     */
    ListaDoble(Nodo<T>* _cabeza);
    /**
     * @brief Constructor por defecto. Inicializa una lista vacía.
     */
    ListaDoble();
    /**
     * @brief Destructor. Libera la memoria de los nodos de la lista.
     */
    ~ListaDoble();
    /**
     * @brief Obtiene el puntero a la cabeza de la lista.
     * @return Puntero al nodo cabeza.
     */
    Nodo<T>* get_cabeza();
    /**
     * @brief Establece el puntero a la cabeza de la lista.
     * @param cabeza Puntero al nuevo nodo cabeza.
     */
    void set_cabeza(Nodo<T>* cabeza);
    /**
     * @brief Obtiene el puntero a la cola de la lista.
     * @return Puntero al nodo cola.
     */
    Nodo<T>* get_cola();
    /**
     * @brief Establece el puntero a la cola de la lista.
     * @param cola Puntero al nuevo nodo cola.
     */
    void set_cola(Nodo<T>* cola);
    /**
     * @brief Inserta un nuevo nodo al final de la lista.
     * @param valor Valor a insertar en la lista.
     */
    void insertar_cola(T valor);
    /**
     * @brief Imprime los elementos de la lista por consola.
     */
    void imprimir();
    /**
     * @brief Verifica si la lista está vacía.
     * @return true si la lista está vacía, false en caso contrario.
     */
    bool esta_vacia();
    /**
     * @brief Recorre la lista y aplica una función a cada elemento.
     * @param func Función a aplicar a cada elemento.
     */
    void recorrer(std::function<void(T)> func) const;
    /**
     * @brief Filtra los elementos según un criterio y aplica una acción a los que cumplen.
     * @param criterio Función que retorna true si el elemento cumple el criterio.
     * @param accion Función a aplicar a los elementos que cumplen el criterio.
     */
    void filtrar(std::function<bool(T)> criterio, std::function<void(T)> accion);
    /**
     * @brief Busca un nodo que cumpla con un criterio dado.
     * @param criterio Función que retorna true si el elemento es el buscado.
     * @return Puntero al nodo encontrado o nullptr si no existe.
     */
    Nodo<T>* buscar(std::function<bool(T)> criterio);
    /**
     * @brief Elimina un nodo específico de la lista.
     * @param nodo Puntero al nodo a eliminar.
     */
    void eliminar(Nodo<T>* nodo);
    /**
     * @brief Verifica si la lista está vacía (versión constante).
     * @return true si la lista está vacía, false en caso contrario.
     */
    bool esta_vacia() const;
    /**
     * @brief Obtiene el tamaño (cantidad de nodos) de la lista.
     * @return Número de nodos en la lista.
     */
    int getTam() const;
    /**
     * @brief Obtiene el valor del nodo en la posición dada.
     * @param index Índice del nodo (comenzando en 0).
     * @return Valor almacenado en el nodo.
     */
    T get_contador(int index) const;
    /**
     * @brief Establece el valor del nodo en la posición dada.
     * @param index Índice del nodo (comenzando en 0).
     * @param valor Nuevo valor a asignar.
     */
    void set_contador(int index, T valor);
private:
    /**
     * @brief Puntero al nodo cabeza de la lista.
     */
    Nodo<T>* cabeza;
    /**
     * @brief Puntero al nodo cola de la lista.
     */
    Nodo<T>* cola;
};

#endif