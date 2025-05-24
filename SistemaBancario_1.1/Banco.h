#ifndef BANCO_H_INCLUDED
#define BANCO_H_INCLUDED

#include "Cliente.h"
#include "Cuenta.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>

class Banco {
public:
    Banco();
    void agregar_cliente(Cliente cliente);
    Cliente* buscar_cliente(std::string dni);
    void consultar_cuentas_cliente(std::string dni, std::string nombre);
    void consultar_movimientos_rango(std::string dni, Fecha inicio, Fecha fin);
    void calcular_intereses_cuenta(int id_cuenta, Fecha hasta);
    void guardar_datos_binario(std::string archivo);
    void cargar_datos_binario(std::string archivo);
private:
    ListaDoble<Cliente>* clientes;
};

#endif