/**
 * @file ArbolBinario.h
 * @brief Definición de funciones para árboles Rojo-Negro de clientes.
 *
 * Este archivo contiene las declaraciones de las funciones para la creación, inserción,
 * balanceo y visualización de árboles Rojo-Negro, utilizados para organizar y mostrar
 * clientes en el sistema bancario.
 */

#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H

#include "Cliente.h"
#include "Banco.h"
#include <iostream>
#include <string>

// Forward declarations
class Banco;

/**
 * @brief Muestra el árbol Rojo-Negro de clientes con visualización ASCII.
 * @param banco Referencia al objeto Banco que contiene los clientes
 */
void mostrar_arbol_binario(Banco& banco);

/**
 * @brief Muestra el árbol Rojo-Negro de clientes con visualización SFML gráfica.
 * @param banco Referencia al objeto Banco que contiene los clientes
 */
void mostrar_arbol_binario_sfml(Banco& banco);

#endif // ARBOLBINARIO_H
