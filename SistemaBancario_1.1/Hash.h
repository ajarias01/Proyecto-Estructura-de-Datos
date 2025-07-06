#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>
#include "Cliente.h"
#include "Banco.h"

class Hash {
private:
    std::unordered_map<std::string, std::vector<Cliente*>> hashTable;

public:
    static std::string calculateMD5(const std::string& filePath);
    static bool verifyFileIntegrity(const std::string& filePath, const std::string& storedHash);
    
    // Nuevos métodos para tabla hash
    void buildHashTable(Banco& banco, int fieldIndex);
    std::vector<Cliente*> searchHash(const std::string& key);
    void clearHashTable();
    std::vector<std::pair<std::string, std::vector<Cliente*>>> getHashTableContents() const;
};

#endif