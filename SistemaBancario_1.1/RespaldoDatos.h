#ifndef RESPALDODATOS_H
#define RESPALDODATOS_H

#include "ListaDoble.h"
#include "Cliente.h"
#include "Ahorro.h"
#include "Corriente.h"
#include <string>

class RespaldoDatos {
public:
    static void guardarRespaldoClientesConFecha(const ListaDoble<Cliente*>& clientes);
    static std::string obtenerUltimoRespaldo();
    static ListaDoble<Cliente*>* restaurarClientesBinario(const std::string& nombreArchivo);
    static void respaldoClientesBinario(const std::string& nombreArchivo, const ListaDoble<Cliente*>& clientes);
    static ListaDoble<Cliente*>* restaurarClientesDesdeTxt(const std::string& archivoTxt, int numCesar);
    static void cifrarArchivoABaseTxt(const std::string& archivoBin, int numCesar);
    static void decifrarTxtABinario(const std::string& archivoTxt, int numCesar) ;
    private:
    static void CifradoCesar(std::string& cifradoCesar, int numCesar);
    static void DecifradoCesar(std::string& decifradoCesar, int numCesar);
};

#endif