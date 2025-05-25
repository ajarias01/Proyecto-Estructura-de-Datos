#ifndef RESPALDO_DATOS_H
#define RESPALDO_DATOS_H

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "Cliente.h"
#include "Banco.h"
#include "Fecha.h"

using json = nlohmann::json;
using namespace std;
class RespaldoDatos {
private:
    string backup;
    int numero;
public:
    RespaldoDatos() : backup(""), numero(0) {}
    // Constructor con parámetros
    RespaldoDatos(const string& backup, int numero) : backup(backup), numero(numero) {}
    // Guardar respaldo completo del banco en JSON
    static void respaldoClientes(const string& nombreArchivo, const ListaDoble<Cliente*>& clientes)
    // Restaurar respaldo completo del banco desde JSON
    static ListaDoble<Cliente*> restaurarClientes(const string& nombreArchivo);
    // Guardar respaldo con fecha automática
    static void guardarRespaldoClientesConFecha(const ListaDoble<Cliente*>& clientes);
    //Cifrado Cesar
    void CifradoCesar(string backup,int numero);
    //Decifrado Cesar
    void DecifradoCesar(string backup,int numero);
    
    // Cargar respaldo específico por fecha
    //static Banco cargarRespaldoConFecha(const std::string& fecha);
    
    // Listar archivos de respaldo disponibles
    //static std::vector<std::string> listarRespaldos();
private:
    string backup;
    int numero;
    
};
#endif
