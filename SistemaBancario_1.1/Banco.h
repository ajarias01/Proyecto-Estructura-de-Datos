#ifndef BANCO_H_INCLUDED
#define BANCO_H_INCLUDED

#include "Cliente.h"
#include "Cuenta.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>
#include <chrono>
#include <functional>

class Banco {
public:
    Banco();
    ~Banco();
    void agregar_cliente(Cliente* cliente);
    void setClientes(ListaDoble<Cliente*>* nuevos_clientes);
    void limpiar_clientes();
    Cliente* buscar_cliente(std::string dni);
    void consultar_cuentas_cliente(std::string dni, std::string nombre, std::string apellido, int fila_actual,
                                  std::function<void(Cuenta*)> callback);
    void consultar_movimientos_rango(std::string dni, Fecha inicio, Fecha fin);
    void guardar_datos_binario(std::string archivo);
    void guardar_datos_binario_sin_backup(std::string archivo);
    void cargar_datos_binario(std::string archivo);
    void cargar_datos_recuperados_binario(std::string archivo);
    void restaurar_desde_respaldo(const std::string& nombreArchivo);
    std::chrono::system_clock::time_point findNextAvailableSlot(int branchId);
    bool isWorkingHour(const std::chrono::system_clock::time_point& tp);
    void addAppointment(int branchId, std::chrono::system_clock::time_point time);

    // Declaración del método getClientes (sin implementación inline)
    ListaDoble<Cliente*>* getClientes() const;

private:
    ListaDoble<Cliente*>* clientes;
    bool datos_cargados;
    std::vector<std::vector<std::chrono::system_clock::time_point>> appointments;
    // ... resto de miembros ...
};

#endif