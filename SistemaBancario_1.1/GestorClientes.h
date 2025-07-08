/**
 * @file GestorClientes.h
 * @brief Definición de la clase GestorClientes para ordenamiento y búsqueda de clientes y movimientos.
 *
 * Esta clase proporciona utilidades para ordenar listas de clientes y movimientos por diferentes campos,
 * utilizando algoritmos de radix sort, counting sort y búsqueda binaria.
 */

#ifndef GESTORCLIENTES_H
#define GESTORCLIENTES_H
#include <iostream>
#include "Cliente.h"
#include <string>
#include <functional>
#include <vector>

/**
 * @class GestorClientes
 * @brief Utilidades para ordenamiento y búsqueda de clientes y movimientos.
 *
 * Permite ordenar listas de clientes por campos string o numéricos, así como ordenar y buscar movimientos por fecha.
 */
class GestorClientes {
public:
    /**
     * @brief Counting sort por carácter para ordenar clientes por un campo string.
     * @param clientes Lista de clientes
     * @param n Número de clientes
     * @param pos Posición del carácter (de derecha a izquierda)
     * @param getter Función para obtener el campo string del cliente
     */
    void countingSortByChar(ListaDoble<Cliente*>& clientes, int n, int pos, std::function<std::string(Cliente*)> getter);
    /**
     * @brief Radix sort para ordenar clientes por un campo string.
     * @param clientes Lista de clientes
     * @param n Número de clientes
     * @param getter Función para obtener el campo string del cliente
     */
    void radixSortStringCampo(ListaDoble<Cliente*>& clientes, int n, std::function<std::string(Cliente*)> getter);
    /**
     * @brief Obtiene el valor máximo de un campo numérico en la lista de clientes.
     * @param clientes Lista de clientes
     * @param n Número de clientes
     * @param getter Función para obtener el campo numérico del cliente
     * @return Valor máximo encontrado
     */
    int getMaxCampoNumerico(const ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter);
    /**
     * @brief Counting sort para un dígito específico de un campo numérico.
     * @param clientes Lista de clientes
     * @param n Número de clientes
     * @param exp Exponente del dígito (1, 10, 100, ...)
     * @param getter Función para obtener el campo numérico del cliente
     */
    void countSortCampoNumerico(ListaDoble<Cliente*>& clientes, int n, int exp, std::function<int(Cliente*)> getter);
    /**
     * @brief Radix sort para ordenar clientes por un campo numérico.
     * @param clientes Lista de clientes
     * @param n Número de clientes
     * @param getter Función para obtener el campo numérico del cliente
     */
    void radixSortCampoNumerico(ListaDoble<Cliente*>& clientes, int n, std::function<int(Cliente*)> getter);
    /**
     * @brief Búsqueda binaria para un campo numérico.
     * @param lista Lista de clientes
     * @param n Número de clientes
     * @param valor Valor a buscar
     * @param getter Función para obtener el campo numérico del cliente
     * @param ascendente true si la lista está ordenada ascendente
     * @return Índice del elemento encontrado o -1 si no existe
     */
    int busquedaBinariaNumerica(ListaDoble<Cliente*>& lista, int n, int valor, std::function<int(Cliente*)> getter, bool ascendente = true);
    /**
     * @brief Búsqueda binaria para un campo string.
     * @param lista Lista de clientes
     * @param n Número de clientes
     * @param valor Valor a buscar
     * @param getter Función para obtener el campo string del cliente
     * @param ascendente true si la lista está ordenada ascendente
     * @return Índice del elemento encontrado o -1 si no existe
     */
    int busquedaBinariaString(ListaDoble<Cliente*>& lista, int n, const std::string& valor, std::function<std::string(Cliente*)> getter, bool ascendente = true);
    /**
     * @brief Obtiene una clave numérica (YYYYMMDD) a partir de una fecha.
     * @param f Fecha
     * @return Clave numérica de la fecha
     */
    static int dateKey(const Fecha& f);
    /**
     * @brief Obtiene una clave numérica (YYYYMMDD) a partir de un movimiento.
     * @param m Movimiento
     * @return Clave numérica de la fecha del movimiento
     */
    static int dateKey(const Movimiento& m);
    /**
     * @brief Radix sort para ordenar un vector de movimientos por fecha.
     * @param arr Vector de movimientos
     */
    static void radixSortFecha(std::vector<Movimiento>& arr);
    /**
     * @brief Búsqueda binaria para encontrar el primer movimiento con fecha >= target.
     * @param arr Vector de movimientos
     * @param target Clave numérica de la fecha objetivo
     * @return Índice del primer movimiento >= target
     */
    static int lowerBound(const std::vector<Movimiento>& arr, int target);
    /**
     * @brief Búsqueda binaria para encontrar el primer movimiento con fecha > target.
     * @param arr Vector de movimientos
     * @param target Clave numérica de la fecha objetivo
     * @return Índice del primer movimiento > target
     */
    static int upperBound(const std::vector<Movimiento>& arr, int target);
};

#endif