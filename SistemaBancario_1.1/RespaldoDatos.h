#ifndef RESPALDODATOS_H
#define RESPALDODATOS_H

#include "ListaDoble.h"
#include "Cliente.h"
#include "Ahorro.h"
#include "Corriente.h"
#include <nlohmann/json.hpp>
#include <string>

class RespaldoDatos {
public:
    static void respaldoClientes(const std::string& nombreArchivo, const ListaDoble<Cliente*>& clientes);
    static ListaDoble<Cliente*>* restaurarClientes(const std::string& nombreArchivo);
    static void guardarRespaldoClientesConFecha(const ListaDoble<Cliente*>& clientes);
    static std::string obtenerUltimoRespaldo();
    
private:
    static void CifradoCesar(std::string& cifradoCesar, int numCesar);
    static void DecifradoCesar(std::string& decifradoCesar, int numCesar);
};

#endif