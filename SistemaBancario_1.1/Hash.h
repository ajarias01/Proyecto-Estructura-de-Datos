/*#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#pragma once
#include <string>

class MD5Util {
public:
    static std::string calcularMD5(const std::string& filename);
    static void guardarHash(const std::string& hash, const std::string& hashFile);
    static std::string leerHash(const std::string& hashFile);
    static bool verificarArchivo(const std::string& filename, const std::string& hashFile);
};

#endif*/
#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <string>

class Hash {
public:
    static std::string calculateMD5(const std::string& filePath);
    static bool verifyFileIntegrity(const std::string& filePath, const std::string& storedHash);
};

#endif