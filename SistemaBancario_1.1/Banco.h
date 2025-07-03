#ifndef BANCO_H_INCLUDED
#define BANCO_H_INCLUDED

#include "Cliente.h"
#include "Cuenta.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>
#include <chrono>
#include <functional>

class Banco
{
public:
    Banco();
    ~Banco();
    ListaDoble<Cliente *> *getClientes() const { return clientes; }
    void agregar_cliente(Cliente *cliente);
    Cliente *buscar_cliente(std::string dni);
    void consultar_cuentas_cliente(std::string dni, std::string nombre, std::string apellido, int fila_actual,
                                   std::function<void(Cuenta *)> callback = nullptr);
    void consultar_movimientos_rango(std::string dni, Fecha inicio, Fecha fin);
    void guardar_datos_binario(std::string archivo);
    void cargar_datos_binario(std::string archivo);
    void guardar_datos_binario_sin_backup(std::string archivo);
    void cargar_datos_recuperados_binario(std::string archivo);
    void restaurar_desde_respaldo(const std::string &nombreArchivo);
    void setClientes(ListaDoble<Cliente *> *nuevos_clientes);
    void limpiar_clientes();
    std::chrono::system_clock::time_point findNextAvailableSlot(int branchId);
    bool isWorkingHour(const std::chrono::system_clock::time_point &tp);
    void addAppointment(int branchId, std::chrono::system_clock::time_point time);

    ListaDoble<Cliente *> *clientes;
    bool datos_cargados;

private:
    std::vector<std::vector<std::chrono::system_clock::time_point>> appointments; // Añadido para consistencia
};

#endif