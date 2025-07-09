/**
 * @file Hash.h
 * @brief Definición de la clase Hash para cálculo y verificación de integridad de archivos y gestión de tablas hash de clientes.
 *
 * Esta clase permite calcular el hash MD5 de archivos, verificar integridad y gestionar una tabla hash de clientes por diferentes campos.
 */

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>
#include "Cliente.h"
#include "Banco.h"

/**
 * @class Hash
 * @brief Utilidades para cálculo de hash MD5 y gestión de tabla hash de clientes.
 *
 * Permite calcular y verificar el hash MD5 de archivos, así como construir, buscar y limpiar una tabla hash de clientes por campo.
 */
class Hash {
private:
    std::unordered_map<std::string, std::vector<Cliente*>> hashTable; ///< Tabla hash de clientes

public:
    /**
     * @brief Calcula el hash MD5 de un archivo dado.
     * @param filePath Ruta del archivo a calcular el hash
     * @return Hash MD5 en formato hexadecimal (string), o vacío si hay error
     */
    static std::string calculateMD5(const std::string& filePath);
    /**
     * @brief Verifica la integridad de un archivo comparando su hash MD5 con uno almacenado.
     * @param filePath Ruta del archivo a verificar
     * @param storedHash Hash MD5 esperado
     * @return true si el hash coincide, false en caso contrario
     */
    static bool verifyFileIntegrity(const std::string& filePath, const std::string& storedHash);
    /**
     * @brief Construye la tabla hash de clientes según el campo seleccionado.
     * @param banco Referencia al banco con la lista de clientes
     * @param fieldIndex Índice del campo (0: DNI, 1: Nombres, 2: Apellidos, 3: Teléfono, 4: Email)
     */
    void buildHashTable(Banco& banco, int fieldIndex);
    /**
     * @brief Busca clientes en la tabla hash por clave.
     * @param key Clave de búsqueda
     * @return Vector de punteros a clientes encontrados (puede estar vacío)
     */
    std::vector<Cliente*> searchHash(const std::string& key);
    /**
     * @brief Limpia la tabla hash de clientes.
     */
    void clearHashTable();
    /**
     * @brief Obtiene el contenido completo de la tabla hash.
     * @return Vector de pares (clave, vector de clientes)
     */
    std::vector<std::pair<std::string, std::vector<Cliente*>>> getHashTableContents() const;
};

#endif