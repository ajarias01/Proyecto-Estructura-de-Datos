/*#include "MD5Util.h"
#include <fstream>
#include <iomanip>
#include <openssl/md5.h>
#include <sstream>

std::string MD5Util::calcularMD5(const std::string& filename) {
    unsigned char c[MD5_DIGEST_LENGTH];
    std::ifstream file(filename, std::ios::binary);
    if (!file) return "";

    MD5_CTX mdContext;
    MD5_Init(&mdContext);

    char buf[1024 * 16];
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&mdContext, buf, file.gcount());
    }
    MD5_Final(c, &mdContext);

    std::ostringstream oss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c[i];
    return oss.str();
}

void MD5Util::guardarHash(const std::string& hash, const std::string& hashFile) {
    std::ofstream out(hashFile);
    if (out) out << hash;
}

std::string MD5Util::leerHash(const std::string& hashFile) {
    std::ifstream in(hashFile);
    std::string hash;
    if (in) in >> hash;
    return hash;
}

bool MD5Util::verificarArchivo(const std::string& filename, const std::string& hashFile) {
    std::string hashActual = calcularMD5(filename);
    std::string hashGuardado = leerHash(hashFile);
    return hashActual == hashGuardado;
}

#include <iostream>
#include "MD5Util.h"

int main() {
    std::string archivo = "datos.txt";
    std::string archivoHash = "Hash";

    // Calcular y guardar hash
    std::string hash = MD5Util::calcularMD5(archivo);
    MD5Util::guardarHash(hash, archivoHash);

    // Verificar integridad
    if (MD5Util::verificarArchivo(archivo, archivoHash)) {
        std::cout << "El archivo NO ha sido manipulado." << std::endl;
    } else {
        std::cout << "El archivo ha sido manipulado!" << std::endl;
    }
    return 0;
}*/
#include "hash.h"
#include <fstream>
#include <iostream>
#include <openssl/evp.h>

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

bool Hash::verifyFileIntegrity(const std::string& filePath, const std::string& storedHash) {
    std::string currentHash = calculateMD5(filePath);
    if (currentHash.empty()) {
        return false;
    }
    return currentHash == storedHash;
}