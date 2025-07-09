/**
 * @file Banco.h
 * @brief Definición de la clase Banco para la gestión de clientes, cuentas y turnos.
 *
 * Esta clase administra la lista de clientes, operaciones de búsqueda, inserción, respaldo,
 * restauración y gestión de turnos en sucursales bancarias.
 */

#ifndef BANCO_H_INCLUDED
#define BANCO_H_INCLUDED

#include "Cliente.h"
#include "Cuenta.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>
#include <chrono>
#include <functional>

/**
 * @class Banco
 * @brief Clase principal para la gestión de clientes y operaciones bancarias.
 *
 * Permite agregar, buscar, respaldar, restaurar y consultar clientes y cuentas,
 * así como gestionar turnos en sucursales.
 */
class Banco {
public:
    /**
     * @brief Constructor de Banco.
     */
    Banco();
    /**
     * @brief Destructor de Banco.
     */
    ~Banco();
    /**
     * @brief Agrega un cliente a la lista, validando duplicados por DNI.
     * @param cliente Puntero al cliente a agregar
     */
    void agregar_cliente(Cliente* cliente);
    /**
     * @brief Reemplaza la lista de clientes por una nueva.
     * @param nuevos_clientes Puntero a la nueva lista de clientes
     */
    void setClientes(ListaDoble<Cliente*>* nuevos_clientes);
    /**
     * @brief Elimina todos los clientes y reinicia la lista.
     */
    void limpiar_clientes();
    /**
     * @brief Busca un cliente por DNI.
     * @param dni DNI del cliente a buscar
     * @return Puntero al cliente si se encuentra, nullptr en caso contrario
     */
    Cliente* buscar_cliente(std::string dni);
    /**
     * @brief Consulta y muestra las cuentas de un cliente según criterios de búsqueda.
     * @param dni DNI del cliente
     * @param nombre Nombre del cliente
     * @param apellido Apellido del cliente
     * @param fila_actual Fila de inicio para impresión en consola
     * @param callback Función callback para procesar cada cuenta encontrada
     */
    void consultar_cuentas_cliente(std::string dni, std::string nombre, std::string apellido, int fila_actual,
                                  std::function<void(Cuenta*)> callback);
    /**
     * @brief Consulta los movimientos de un cliente en un rango de fechas.
     * @param dni DNI del cliente
     * @param inicio Fecha de inicio
     * @param fin Fecha de fin
     */
    void consultar_movimientos_rango(std::string dni, Fecha inicio, Fecha fin);
    /**
     * @brief Guarda los datos de clientes en un archivo binario y realiza respaldo.
     * @param archivo Nombre del archivo binario
     */
    void guardar_datos_binario(std::string archivo);
    /**
     * @brief Guarda los datos de clientes en un archivo binario sin crear respaldo.
     * @param archivo Nombre del archivo binario
     */
    void guardar_datos_binario_sin_backup(std::string archivo);
    /**
     * @brief Carga los datos de clientes desde un archivo binario, restaurando desde respaldo si es necesario.
     * @param archivo Nombre del archivo binario
     */
    void cargar_datos_binario(std::string archivo);
    /**
     * @brief Carga los datos de clientes desde un archivo binario recuperado (sin usar bandera de datos cargados).
     * @param archivo Nombre del archivo binario
     */
    void cargar_datos_recuperados_binario(std::string archivo);
    /**
     * @brief Restaura la lista de clientes desde un archivo de respaldo binario.
     * @param nombreArchivo Nombre del archivo de respaldo
     */
    void restaurar_desde_respaldo(const std::string& nombreArchivo);
    /**
     * @brief Busca el siguiente turno disponible en una sucursal.
     * @param branchId ID de la sucursal (1, 2 o 3)
     * @return Fecha y hora del siguiente turno disponible
     */
    std::chrono::system_clock::time_point findNextAvailableSlot(int branchId);
    /**
     * @brief Verifica si un horario es laboral (lunes a viernes, 9:00-17:00).
     * @param tp Punto en el tiempo a verificar
     * @return true si es horario laboral, false en caso contrario
     */
    bool isWorkingHour(const std::chrono::system_clock::time_point& tp);
    /**
     * @brief Agrega un turno (cita) a la sucursal indicada.
     * @param branchId ID de la sucursal
     * @param time Fecha y hora de la cita
     */
    void addAppointment(int branchId, std::chrono::system_clock::time_point time);
    /**
     * @brief Obtiene la lista de clientes del banco.
     * @return Puntero a la lista doble de clientes
     */
    ListaDoble<Cliente*>* getClientes() const;

private:
    ListaDoble<Cliente*>* clientes; ///< Puntero a la lista doble de clientes
    bool datos_cargados; ///< Bandera para indicar si los datos han sido cargados
    std::vector<std::vector<std::chrono::system_clock::time_point>> appointments; ///< Turnos por sucursal
    // ... resto de miembros ...
};

#endif