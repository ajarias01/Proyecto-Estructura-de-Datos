/**
 * @file RespaldoDatos.h
 * @brief Definición de la clase RespaldoDatos para gestión de respaldos y recuperación de datos
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la declaración de la clase RespaldoDatos que proporciona
 *          funcionalidades completas para respaldar, restaurar y cifrar datos de clientes
 *          del sistema bancario. Incluye métodos para respaldos binarios, cifrado César,
 *          y gestión de archivos de respaldo con marcas de tiempo.
 */

#ifndef RESPALDODATOS_H
#define RESPALDODATOS_H

#include "ListaDoble.h"
#include "Cliente.h"
#include "Ahorro.h"
#include "Corriente.h"
#include <fstream>
#include <string>
#include <stdexcept> 
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <vector>

/**
 * @class RespaldoDatos
 * @brief Clase para gestión completa de respaldos y recuperación de datos del sistema bancario
 * @details Esta clase proporciona métodos estáticos para realizar respaldos binarios,
 *          cifrado de archivos, restauración de datos y gestión de archivos de respaldo.
 *          Incluye funcionalidades de cifrado César para proteger la información sensible
 *          y manejo de archivos con marcas de tiempo automáticas.
 */
class RespaldoDatos {
public:
    /**
     * @brief Guarda un respaldo de clientes con marca de tiempo automática
     * @param clientes Lista de clientes a respaldar
     * @details Genera un archivo de respaldo binario con nombre que incluye fecha y hora
     *          en formato YYYYMMDD_HHMMSS para identificación única del respaldo
     */
    static void guardarRespaldoClientesConFecha(const ListaDoble<Cliente*>& clientes);
    
    /**
     * @brief Obtiene el nombre del último archivo de respaldo disponible
     * @return String con el nombre del archivo de respaldo más reciente
     * @details Busca en el directorio actual el archivo de respaldo más reciente
     *          basándose en las marcas de tiempo de los nombres de archivo
     */
    static std::string obtenerUltimoRespaldo();
    
    /**
     * @brief Restaura clientes desde un archivo binario
     * @param nombreArchivo Nombre del archivo binario a restaurar
     * @return Puntero a ListaDoble con los clientes restaurados
     * @details Lee un archivo binario y reconstruye la estructura completa de clientes
     *          incluyendo cuentas y movimientos
     */
    static ListaDoble<Cliente*>* restaurarClientesBinario(const std::string& nombreArchivo);
    
    /**
     * @brief Realiza respaldo binario de clientes
     * @param nombreArchivo Nombre del archivo donde guardar el respaldo
     * @param clientes Lista de clientes a respaldar
     * @details Serializa toda la información de clientes en formato binario
     *          incluyendo datos personales, cuentas y movimientos
     */
    static void respaldoClientesBinario(const std::string& nombreArchivo, const ListaDoble<Cliente*>& clientes);
    
    /**
     * @brief Restaura clientes desde un archivo de texto cifrado
     * @param archivoTxt Nombre del archivo de texto cifrado
     * @param numCesar Número de desplazamiento para el cifrado César
     * @return Puntero a ListaDoble con los clientes restaurados
     * @details Descifra un archivo de texto y lo convierte a binario para restaurar clientes
     */
    static ListaDoble<Cliente*>* restaurarClientesDesdeTxt(const std::string& archivoTxt, int numCesar);
    
    /**
     * @brief Cifra un archivo binario a formato de texto hexadecimal
     * @param archivoBin Nombre del archivo binario a cifrar
     * @param numCesar Número de desplazamiento para el cifrado César
     * @details Convierte un archivo binario a representación hexadecimal
     *          y aplica cifrado César para proteger la información
     */
    static void cifrarArchivoABaseTxt(const std::string& archivoBin, int numCesar);
    
    /**
     * @brief Descifra un archivo de texto a formato binario
     * @param archivoTxt Nombre del archivo de texto cifrado
     * @param numCesar Número de desplazamiento para el descifrado César
     * @return String con el nombre del archivo binario descifrado
     * @details Aplica descifrado César y convierte de hexadecimal a binario
     */
    static std::string decifrarTxtABinario(const std::string& archivoTxt, int numCesar);
    
    /**
     * @brief Obtiene el último archivo de texto cifrado disponible
     * @return String con el nombre del archivo de texto cifrado más reciente
     * @details Busca archivos de texto cifrados y devuelve el más reciente
     */
    static std::string obtenerUltimoTxtCifrado();
    
    /**
     * @brief Lista todos los archivos de texto cifrados disponibles
     * @details Muestra en consola una lista numerada de todos los archivos
     *          de texto cifrados con sus fechas y tamaños
     */
    static void listarArchivosTxtCifrados();
    
    /**
     * @brief Permite seleccionar y descifrar un archivo de texto
     * @details Presenta una interfaz interactiva para seleccionar un archivo
     *          de texto cifrado y lo descifra usando el cifrado César
     */
    static void seleccionarYDescifrarTxt();
    
    /**
     * @brief Descifra únicamente un archivo de texto sin restaurar datos
     * @param archivoTxt Nombre del archivo de texto a descifrar
     * @param numCesar Número de desplazamiento para el descifrado César
     * @details Descifra un archivo de texto y lo guarda como binario
     *          sin cargar los datos en memoria
     */
    static void descifrarTxtSolamente(const std::string& archivoTxt, int numCesar);
    
private:
    /**
     * @brief Aplica cifrado César a una cadena de texto
     * @param cifradoCesar Referencia a la cadena a cifrar (se modifica)
     * @param numCesar Número de desplazamiento para el cifrado
     * @details Aplica cifrado César solo a caracteres hexadecimales (a-f, 0-9)
     */
    static void CifradoCesar(std::string& cifradoCesar, int numCesar);
    
    /**
     * @brief Aplica descifrado César a una cadena de texto
     * @param decifradoCesar Referencia a la cadena a descifrar (se modifica)
     * @param numCesar Número de desplazamiento para el descifrado
     * @details Aplica descifrado César solo a caracteres hexadecimales (a-f, 0-9)
     */
    static void DecifradoCesar(std::string& decifradoCesar, int numCesar);
};

#endif