/**
 * @file Hash.cpp
 * @brief Implementación de la clase Hash para cálculo y verificación de integridad de archivos y gestión de tablas hash de clientes.
 *
 * Este archivo contiene métodos para calcular el hash MD5 de archivos, verificar integridad, y construir/buscar en una tabla hash de clientes.
 */

#include "hash.h"
#include <fstream>
#include <iostream>
#include <openssl/evp.h>

/**
 * @brief Calcula el hash MD5 de un archivo dado.
 * @param filePath Ruta del archivo a calcular el hash
 * @return Hash MD5 en formato hexadecimal (string), o vacío si hay error
 */
std::string Hash::calculateMD5(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return "";
    }

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        std::cerr << "Error al crear el contexto MD5." << std::endl;
        return "";
    }

    if (EVP_DigestInit_ex(mdctx, EVP_md5(), NULL) != 1) {
        std::cerr << "Error al inicializar el contexto MD5." << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
            std::cerr << "Error al actualizar el hash." << std::endl;
            EVP_MD_CTX_free(mdctx);
            return "";
        }
    }
    if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
        std::cerr << "Error al finalizar el hash." << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;
    if (EVP_DigestFinal_ex(mdctx, digest, &digest_len) != 1) {
        std::cerr << "Error al obtener el hash final." << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    char md5string[33];
    for (int i = 0; i < (int)digest_len; i++) {
        sprintf(&md5string[i * 2], "%02x", (unsigned int)digest[i]);
    }
    md5string[32] = '\0';

    EVP_MD_CTX_free(mdctx);
    return std::string(md5string);
}

/**
 * @brief Verifica la integridad de un archivo comparando su hash MD5 con uno almacenado.
 * @param filePath Ruta del archivo a verificar
 * @param storedHash Hash MD5 esperado
 * @return true si el hash coincide, false en caso contrario
 */
bool Hash::verifyFileIntegrity(const std::string& filePath, const std::string& storedHash) {
    std::string currentHash = calculateMD5(filePath);
    if (currentHash.empty()) {
        return false;
    }
    return currentHash == storedHash;
}

/**
 * @brief Construye la tabla hash de clientes según el campo seleccionado.
 * @param banco Referencia al banco con la lista de clientes
 * @param fieldIndex Índice del campo (0: DNI, 1: Nombres, 2: Apellidos, 3: Teléfono, 4: Email)
 */
void Hash::buildHashTable(Banco& banco, int fieldIndex) {
    clearHashTable();
    if (!banco.getClientes()) return;

    banco.getClientes()->recorrer([this, fieldIndex](Cliente* client) {
        std::string key;
        switch (fieldIndex) {
            case 0: key = client->get_dni(); break;
            case 1: key = client->get_nombres(); break;
            case 2: key = client->get_apellidos(); break;
            case 3: key = client->get_telefono(); break;
            case 4: key = client->get_email(); break;
            default: return;
        }
        hashTable[key].push_back(client);
    });
}

/**
 * @brief Busca clientes en la tabla hash por clave.
 * @param key Clave de búsqueda
 * @return Vector de punteros a clientes encontrados (puede estar vacío)
 */
std::vector<Cliente*> Hash::searchHash(const std::string& key) {
    auto it = hashTable.find(key);
    return (it != hashTable.end()) ? it->second : std::vector<Cliente*>();
}

/**
 * @brief Limpia la tabla hash de clientes.
 */
void Hash::clearHashTable() {
    for (auto& pair : hashTable) {
        pair.second.clear(); // No liberamos memoria, los clientes son manejados por Banco
    }
    hashTable.clear();
}

/**
 * @brief Obtiene el contenido completo de la tabla hash.
 * @return Vector de pares (clave, vector de clientes)
 */
std::vector<std::pair<std::string, std::vector<Cliente*>>> Hash::getHashTableContents() const {
    std::vector<std::pair<std::string, std::vector<Cliente*>>> contents;
    for (const auto& pair : hashTable) {
        contents.push_back(pair);
    }
    return contents;
}