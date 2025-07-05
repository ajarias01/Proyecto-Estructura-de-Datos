#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H

#include "Cliente.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

template<typename T>
class NodoArbol {
public:
    Cliente* cliente;
    T valor_comparacion;
    NodoArbol* izquierda;
    NodoArbol* derecha;
    
    NodoArbol(Cliente* c, T valor) : cliente(c), valor_comparacion(valor), izquierda(nullptr), derecha(nullptr) {}
};

template<typename T>
class ArbolBinario {
private:
    NodoArbol<T>* raiz;
    int campo_seleccionado;
    
    // Función auxiliar para insertar
    NodoArbol<T>* insertar_recursivo(NodoArbol<T>* nodo, Cliente* cliente, T valor);
    
    // Función auxiliar para calcular profundidad máxima
    int profundidad_maxima(NodoArbol<T>* nodo);
    
    // Función auxiliar para imprimir el árbol
    void imprimir_nivel(NodoArbol<T>* nodo, int nivel_actual, int nivel_objetivo, int offset);
    
    // Función auxiliar para liberar memoria
    void destruir_arbol(NodoArbol<T>* nodo);
    
    // Función para obtener valor de comparación según el campo
    T obtener_valor_comparacion(Cliente* cliente, int campo);

public:
    ArbolBinario();
    ~ArbolBinario();
    
    void insertar(Cliente* cliente, int campo);
    void imprimir_arbol();
    void limpiar();
    
    // Métodos específicos para diferentes tipos de datos
    std::string obtener_valor_string(Cliente* cliente, int campo);
    int obtener_valor_int(Cliente* cliente, int campo);
};

// Implementaciones de template
template<typename T>
ArbolBinario<T>::ArbolBinario() : raiz(nullptr), campo_seleccionado(0) {}

template<typename T>
ArbolBinario<T>::~ArbolBinario() {
    destruir_arbol(raiz);
}

template<typename T>
void ArbolBinario<T>::destruir_arbol(NodoArbol<T>* nodo) {
    if (nodo) {
        destruir_arbol(nodo->izquierda);
        destruir_arbol(nodo->derecha);
        delete nodo;
    }
}

template<typename T>
NodoArbol<T>* ArbolBinario<T>::insertar_recursivo(NodoArbol<T>* nodo, Cliente* cliente, T valor) {
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
void ArbolBinario<T>::insertar(Cliente* cliente, int campo) {
    campo_seleccionado = campo;
    T valor = obtener_valor_comparacion(cliente, campo);
    raiz = insertar_recursivo(raiz, cliente, valor);
}

template<typename T>
T ArbolBinario<T>::obtener_valor_comparacion(Cliente* cliente, int campo) {
    // Esta función debe ser especializada para cada tipo
    return T{};
}

// Declaraciones de especializaciones (se implementan en el .cpp)
template<>
std::string ArbolBinario<std::string>::obtener_valor_comparacion(Cliente* cliente, int campo);

template<>
int ArbolBinario<int>::obtener_valor_comparacion(Cliente* cliente, int campo);

template<typename T>
int ArbolBinario<T>::profundidad_maxima(NodoArbol<T>* nodo) {
    if (!nodo) return 0;
    return 1 + std::max(profundidad_maxima(nodo->izquierda), profundidad_maxima(nodo->derecha));
}

template<typename T>
void ArbolBinario<T>::imprimir_nivel(NodoArbol<T>* nodo, int nivel_actual, int nivel_objetivo, int offset) {
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
void ArbolBinario<T>::imprimir_arbol() {
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
void ArbolBinario<T>::limpiar() {
    destruir_arbol(raiz);
    raiz = nullptr;
}

#endif // ARBOLBINARIO_H
