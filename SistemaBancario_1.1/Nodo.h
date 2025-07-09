/**
 * @file Nodo.h
 * @brief Definición de la clase template Nodo para estructuras de datos enlazadas.
 * @author Sistema Bancario
 * @date 2024
 * @details Este archivo contiene la declaración de la clase template Nodo que sirve como
 * elemento básico para construir listas enlazadas simples y dobles en el sistema bancario.
 */

#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

/**
 * @class Nodo
 * @brief Clase template que representa un nodo para estructuras de datos enlazadas.
 * @tparam T Tipo de dato que almacena el nodo.
 * @details Esta clase template proporciona la funcionalidad básica para crear nodos
 * que pueden ser utilizados en listas simplemente enlazadas o doblemente enlazadas.
 * Cada nodo contiene un valor de tipo T y punteros al nodo siguiente y anterior.
 */
template <typename T>
class Nodo {
public:
    /**
     * @brief Constructor completo que inicializa el nodo con valor y enlaces.
     * @param _valor Valor a almacenar en el nodo.
     * @param _siguiente Puntero al siguiente nodo en la lista.
     * @param _anterior Puntero al nodo anterior en la lista.
     */
    Nodo(T _valor, Nodo* _siguiente, Nodo* _anterior);
    
    /**
     * @brief Constructor simplificado para listas simplemente enlazadas.
     * @param _valor Valor a almacenar en el nodo.
     * @param _siguiente Puntero al siguiente nodo en la lista.
     */
    Nodo(T _valor, Nodo* _siguiente);
    
    /**
     * @brief Constructor por defecto.
     * @details Inicializa un nodo vacío con punteros a nullptr.
     */
    Nodo();
    
    /**
     * @brief Destructor de la clase Nodo.
     * @details Destructor que permite la limpieza automática del nodo.
     */
    ~Nodo();
    
    /**
     * @brief Obtiene el valor almacenado en el nodo.
     * @return Valor de tipo T almacenado en el nodo.
     */
    T get_valor();
    
    /**
     * @brief Establece el valor del nodo.
     * @param new_valor Nuevo valor a almacenar en el nodo.
     */
    void set_valor(T new_valor);
    
    /**
     * @brief Obtiene el puntero al siguiente nodo.
     * @return Puntero al siguiente nodo o nullptr si es el último.
     */
    Nodo<T>* get_siguiente();
    
    /**
     * @brief Establece el puntero al siguiente nodo.
     * @param new_siguiente Puntero al nuevo nodo siguiente.
     */
    void set_siguiente(Nodo<T>* new_siguiente);
    
    /**
     * @brief Obtiene el puntero al nodo anterior.
     * @return Puntero al nodo anterior o nullptr si es el primero.
     */
    Nodo<T>* get_anterior();
    
    /**
     * @brief Establece el puntero al nodo anterior.
     * @param new_anterior Puntero al nuevo nodo anterior.
     */
    void set_anterior(Nodo<T>* new_anterior);
    
private:
    /**
     * @brief Valor almacenado en el nodo.
     */
    T valor;
    
    /**
     * @brief Puntero al siguiente nodo en la lista.
     */
    Nodo<T>* siguiente;
    
    /**
     * @brief Puntero al nodo anterior en la lista.
     */
    Nodo<T>* anterior;
};

#endif