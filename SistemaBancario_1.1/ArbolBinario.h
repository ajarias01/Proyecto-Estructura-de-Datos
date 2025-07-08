/**
 * @file ArbolBinario.h
 * @brief Definición de clases y métodos para árboles binarios de clientes.
 *
 * Este archivo contiene las definiciones de las clases template NodoArbol y ArbolBinario,
 * utilizadas para organizar y visualizar clientes en estructuras de árbol binario, permitiendo
 * la comparación por diferentes campos (DNI, nombre, apellido, teléfono, email).
 */

#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H

#include "Cliente.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

/**
 * @class NodoArbol
 * @brief Nodo de un árbol binario de clientes.
 *
 * Clase template que representa un nodo de un árbol binario, almacenando un puntero a Cliente,
 * el valor de comparación (según el campo seleccionado) y punteros a los hijos izquierdo y derecho.
 * @tparam T Tipo del valor de comparación (string o int)
 */
template<typename T>
class NodoArbol {
public:
    Cliente* cliente;              ///< Puntero al cliente asociado
    T valor_comparacion;           ///< Valor del campo por el que se ordena
    NodoArbol* izquierda;          ///< Puntero al hijo izquierdo
    NodoArbol* derecha;            ///< Puntero al hijo derecho
    /**
     * @brief Constructor de NodoArbol
     * @param c Puntero al cliente
     * @param valor Valor de comparación
     */
    NodoArbol(Cliente* c, T valor) : cliente(c), valor_comparacion(valor), izquierda(nullptr), derecha(nullptr) {}
};

/**
 * @class ArbolBinario
 * @brief Árbol binario de clientes con comparación flexible.
 *
 * Clase template que implementa un árbol binario para almacenar clientes, permitiendo ordenar
 * por diferentes campos. Incluye métodos para inserción, impresión y limpieza del árbol.
 * @tparam T Tipo del valor de comparación (string o int)
 */
template<typename T>
class ArbolBinario {
private:
    NodoArbol<T>* raiz;            ///< Puntero a la raíz del árbol
    int campo_seleccionado;        ///< Campo por el que se ordena el árbol
    /**
     * @brief Inserta recursivamente un nodo en el árbol.
     * @param nodo Nodo actual
     * @param cliente Puntero al cliente
     * @param valor Valor de comparación
     * @return Nuevo nodo raíz del subárbol
     */
    NodoArbol<T>* insertar_recursivo(NodoArbol<T>* nodo, Cliente* cliente, T valor);
    /**
     * @brief Calcula la profundidad máxima del árbol.
     * @param nodo Nodo actual
     * @return Profundidad máxima
     */
    int profundidad_maxima(NodoArbol<T>* nodo);
    /**
     * @brief Imprime los nodos de un nivel específico del árbol.
     * @param nodo Nodo actual
     * @param nivel_actual Nivel actual
     * @param nivel_objetivo Nivel a imprimir
     * @param offset Espaciado para visualización
     */
    void imprimir_nivel(NodoArbol<T>* nodo, int nivel_actual, int nivel_objetivo, int offset);
    /**
     * @brief Libera la memoria de todos los nodos del árbol.
     * @param nodo Nodo actual
     */
    void destruir_arbol(NodoArbol<T>* nodo);
    /**
     * @brief Obtiene el valor de comparación según el campo seleccionado.
     * @param cliente Puntero al cliente
     * @param campo Campo a comparar
     * @return Valor de comparación
     */
    T obtener_valor_comparacion(Cliente* cliente, int campo);

public:
    /**
     * @brief Constructor de ArbolBinario.
     * Inicializa el árbol vacío y el campo seleccionado en 0.
     */
    ArbolBinario();
    /**
     * @brief Destructor de ArbolBinario.
     * Libera toda la memoria ocupada por el árbol.
     */
    ~ArbolBinario();
    /**
     * @brief Inserta un cliente en el árbol según el campo seleccionado.
     * @param cliente Puntero al cliente
     * @param campo Campo por el que se ordena
     */
    void insertar(Cliente* cliente, int campo);
    /**
     * @brief Imprime el árbol binario por niveles en consola.
     */
    void imprimir_arbol();
    /**
     * @brief Limpia el árbol, liberando toda la memoria.
     */
    void limpiar();
    /**
     * @brief Obtiene el valor de un campo como string.
     * @param cliente Puntero al cliente
     * @param campo Campo a obtener
     * @return Valor como string
     */
    std::string obtener_valor_string(Cliente* cliente, int campo);
    /**
     * @brief Obtiene el valor de un campo como int.
     * @param cliente Puntero al cliente
     * @param campo Campo a obtener
     * @return Valor como int
     */
    int obtener_valor_int(Cliente* cliente, int campo);
};

// Implementaciones de template
template<typename T>
inline ArbolBinario<T>::ArbolBinario() : raiz(nullptr), campo_seleccionado(0) {}

template<typename T>
inline ArbolBinario<T>::~ArbolBinario() {
    destruir_arbol(raiz);
}

template<typename T>
inline void ArbolBinario<T>::destruir_arbol(NodoArbol<T>* nodo) {
    if (nodo) {
        destruir_arbol(nodo->izquierda);
        destruir_arbol(nodo->derecha);
        delete nodo;
    }
}

template<typename T>
inline NodoArbol<T>* ArbolBinario<T>::insertar_recursivo(NodoArbol<T>* nodo, Cliente* cliente, T valor) {
    if (!nodo) {
        return new NodoArbol<T>(cliente, valor);
    }
    
    if (valor < nodo->valor_comparacion) {
        nodo->izquierda = insertar_recursivo(nodo->izquierda, cliente, valor);
    } else {
        nodo->derecha = insertar_recursivo(nodo->derecha, cliente, valor);
    }
    
    return nodo;
}

template<typename T>
inline void ArbolBinario<T>::insertar(Cliente* cliente, int campo) {
    campo_seleccionado = campo;
    T valor = obtener_valor_comparacion(cliente, campo);
    raiz = insertar_recursivo(raiz, cliente, valor);
}

template<typename T>
inline T ArbolBinario<T>::obtener_valor_comparacion(Cliente* cliente, int campo) {
    // Esta función debe ser especializada para cada tipo
    return T{};
}

/**
 * @brief Especialización de obtener_valor_comparacion para std::string.
 * Obtiene el valor de comparación como string según el campo seleccionado.
 * @param cliente Puntero al cliente
 * @param campo Campo a comparar
 * @return Valor del campo como string
 */
template<>
std::string ArbolBinario<std::string>::obtener_valor_comparacion(Cliente* cliente, int campo);

/**
 * @brief Especialización de obtener_valor_comparacion para int.
 * Obtiene el valor de comparación como entero según el campo seleccionado.
 * @param cliente Puntero al cliente
 * @param campo Campo a comparar
 * @return Valor del campo como int
 */
template<>
int ArbolBinario<int>::obtener_valor_comparacion(Cliente* cliente, int campo);

/**
 * @brief Obtiene el valor de un campo del cliente como string.
 * @param cliente Puntero al cliente
 * @param campo Campo a obtener
 * @return Valor como string
 */
template<>
inline std::string ArbolBinario<std::string>::obtener_valor_string(Cliente* cliente, int campo) {
    switch (campo) {
        case 1: return cliente->get_dni();
        case 2: return cliente->get_nombres();
        case 3: return cliente->get_apellidos();
        case 4: return cliente->get_telefono();
        case 5: return cliente->get_email();
        default: return "";
    }
}

/**
 * @brief Obtiene el valor de un campo del cliente como int.
 * @param cliente Puntero al cliente
 * @param campo Campo a obtener
 * @return Valor como int
 */
template<>
inline int ArbolBinario<int>::obtener_valor_int(Cliente* cliente, int campo) {
    switch (campo) {
        case 1: return std::stoi(cliente->get_dni());
        case 4: return std::stoi(cliente->get_telefono());
        default: return 0;
    }
}

template<typename T>
inline int ArbolBinario<T>::profundidad_maxima(NodoArbol<T>* nodo) {
    if (!nodo) return 0;
    return 1 + std::max(profundidad_maxima(nodo->izquierda), profundidad_maxima(nodo->derecha));
}

template<typename T>
inline void ArbolBinario<T>::imprimir_nivel(NodoArbol<T>* nodo, int nivel_actual, int nivel_objetivo, int offset) {
    if (!nodo && nivel_actual <= nivel_objetivo) {
        // Imprimir espacios para nodos vacíos
        for (int i = 0; i < offset * 2 + 1; i++) {
            std::cout << " ";
        }
        return;
    }
    
    if (nivel_actual == nivel_objetivo && nodo) {
        // Imprimir el valor del nodo con espaciado
        std::string valor_mostrar;
        
        // Obtener una representación corta del valor para mostrar
        if (campo_seleccionado == 1) { // DNI
            valor_mostrar = nodo->cliente->get_dni().substr(nodo->cliente->get_dni().length() - 4);
        } else if (campo_seleccionado == 2) { // Nombre
            valor_mostrar = nodo->cliente->get_nombres().substr(0, 5);
        } else if (campo_seleccionado == 3) { // Apellido
            valor_mostrar = nodo->cliente->get_apellidos().substr(0, 5);
        } else if (campo_seleccionado == 4) { // Teléfono
            valor_mostrar = nodo->cliente->get_telefono().substr(nodo->cliente->get_telefono().length() - 4);
        } else if (campo_seleccionado == 5) { // Email
            size_t pos = nodo->cliente->get_email().find('@');
            valor_mostrar = (pos != std::string::npos) ? 
                           nodo->cliente->get_email().substr(0, std::min(pos, size_t(5))) : 
                           nodo->cliente->get_email().substr(0, 5);
        }
        
        int espacios_antes = offset - valor_mostrar.length() / 2;
        int espacios_despues = offset - valor_mostrar.length() + espacios_antes;
        
        for (int i = 0; i < espacios_antes; i++) std::cout << " ";
        std::cout << valor_mostrar;
        for (int i = 0; i < espacios_despues; i++) std::cout << " ";
        
    } else if (nivel_actual < nivel_objetivo && nodo) {
        int nuevo_offset = offset / 2;
        imprimir_nivel(nodo->izquierda, nivel_actual + 1, nivel_objetivo, nuevo_offset);
        imprimir_nivel(nodo->derecha, nivel_actual + 1, nivel_objetivo, nuevo_offset);
    }
}

template<typename T>
inline void ArbolBinario<T>::imprimir_arbol() {
    if (!raiz) {
        std::cout << "El árbol está vacío." << std::endl;
        return;
    }
    
    int profundidad = profundidad_maxima(raiz);
    int offset_inicial = (1 << (profundidad - 1)) * 4; // Espaciado base
    
    std::cout << "\n===========================================" << std::endl;
    std::cout << "===    ÁRBOL BINARIO DE CLIENTES    ===" << std::endl;
    
    const char* campos[] = {"", "DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    std::cout << "===    ORDENADO POR: " << campos[campo_seleccionado] << "    ===" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    for (int nivel = 0; nivel < profundidad; nivel++) {
        std::cout << "\nNivel " << nivel << ": ";
        imprimir_nivel(raiz, 0, nivel, offset_inicial >> nivel);
        std::cout << std::endl;
        
        // Imprimir líneas de conexión para niveles intermedios
        if (nivel < profundidad - 1) {
            std::cout << "        ";
            for (int i = 0; i < (1 << nivel); i++) {
                int espacios = (offset_inicial >> nivel) * 2;
                for (int j = 0; j < espacios / 4; j++) std::cout << " ";
                std::cout << "/";
                for (int j = 0; j < espacios / 2; j++) std::cout << " ";
                std::cout << "\\";
                for (int j = 0; j < espacios / 4; j++) std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "===========================================" << std::endl;
}

template<typename T>
inline void ArbolBinario<T>::limpiar() {
    destruir_arbol(raiz);
    raiz = nullptr;
}

#endif // ARBOLBINARIO_H
