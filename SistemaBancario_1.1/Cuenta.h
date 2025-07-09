/**
 * @file Cuenta.h
 * @brief Definición de la clase abstracta Cuenta para la gestión de cuentas bancarias.
 *
 * Esta clase representa una cuenta bancaria genérica, con operaciones comunes y métodos virtuales
 * para especialización en tipos de cuenta como Corriente o Ahorro.
 */

#ifndef CUENTA_H_INCLUDED
#define CUENTA_H_INCLUDED

#include "Movimiento.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>
#include <chrono> // Para time_point

/**
 * @class Cuenta
 * @brief Clase abstracta para la gestión de cuentas bancarias.
 *
 * Define la interfaz y atributos comunes para cuentas bancarias, incluyendo operaciones de depósito,
 * retiro, serialización y consulta de movimientos. Debe ser heredada por tipos de cuenta específicos.
 */
class Cuenta {
public:
    /**
     * @brief Constructor por defecto de Cuenta.
     */
    Cuenta();
    /**
     * @brief Constructor parametrizado de Cuenta.
     * @param id ID de la cuenta
     * @param saldo_inicial Saldo inicial de la cuenta
     * @param fecha Fecha de apertura
     */
    Cuenta(std::string id, double saldo_inicial, Fecha fecha);
    /**
     * @brief Destructor virtual de Cuenta.
     */
    virtual ~Cuenta();
    /**
     * @brief Obtiene el ID de la cuenta.
     * @return ID de la cuenta como string
     */
    std::string get_id_cuenta();
    /**
     * @brief Obtiene el saldo de la cuenta.
     * @return Saldo actual
     */
    double get_saldo();
    /**
     * @brief Obtiene la fecha de apertura de la cuenta.
     * @return Fecha de apertura
     */
    Fecha get_fecha_apertura();
    /**
     * @brief Obtiene la lista de movimientos de la cuenta.
     * @return Puntero a la lista doble de movimientos
     */
    ListaDoble<Movimiento>* get_movimientos();
    /**
     * @brief Establece el ID de la cuenta.
     * @param id Nuevo ID de la cuenta
     */
    void set_id_cuenta(std::string id);
    /**
     * @brief Establece el saldo de la cuenta.
     * @param _saldo Nuevo saldo
     */
    void set_saldo(double _saldo);
    /**
     * @brief Establece la fecha de apertura de la cuenta.
     * @param fecha Nueva fecha de apertura
     */
    void set_fecha_apertura(Fecha fecha);
    /**
     * @brief Obtiene el tipo de cuenta (debe ser implementado por las subclases).
     * @return Tipo de cuenta como string
     */
    virtual std::string get_tipo() = 0;
    /**
     * @brief Devuelve una representación en string de la cuenta (debe ser implementado por las subclases).
     * @return String con los datos principales de la cuenta
     */
    virtual std::string to_string() = 0;
    /**
     * @brief Guarda los datos de la cuenta en un archivo binario (debe ser implementado por las subclases).
     * @param archivo Puntero al archivo binario abierto para escritura
     */
    virtual void guardar_binario(FILE* archivo) = 0;
    /**
     * @brief Carga los datos de la cuenta desde un archivo binario (debe ser implementado por las subclases).
     * @param archivo Puntero al archivo binario abierto para lectura
     */
    virtual void cargar_binario(FILE* archivo) = 0;
    /**
     * @brief Realiza un retiro de la cuenta.
     * @param monto Monto a retirar
     * @param fecha Fecha del retiro
     * @return true si el retiro fue exitoso, false en caso contrario
     */
    virtual bool retirar(double monto, Fecha fecha);
    /**
     * @brief Realiza un depósito en la cuenta.
     * @param monto Monto a depositar
     * @param fecha Fecha del depósito
     */
    void depositar(double monto, Fecha fecha);
    /**
     * @brief Consulta el saldo actual de la cuenta.
     * @return Saldo actual
     */
    double consultar_saldo();
    /**
     * @brief Consulta e imprime los movimientos de la cuenta en un rango de fechas.
     * @param inicio Fecha de inicio
     * @param fin Fecha de fin
     */
    void consultar_movimientos_rango(Fecha inicio, Fecha fin);

    /**
     * @brief Obtiene el ID de la sucursal asociada a la cuenta.
     * @return ID de la sucursal
     */
    int get_branchId() const { return branchId; }
    /**
     * @brief Establece el ID de la sucursal asociada a la cuenta.
     * @param id Nuevo ID de sucursal
     */
    void set_branchId(int id) { branchId = id; }
    /**
     * @brief Obtiene la hora de la cita asociada a la cuenta.
     * @return Hora de la cita (time_point)
     */
    std::chrono::system_clock::time_point get_appointmentTime() const { return appointmentTime; }
    /**
     * @brief Establece la hora de la cita asociada a la cuenta.
     * @param time Nueva hora de la cita (time_point)
     */
    void set_appointmentTime(std::chrono::system_clock::time_point time) { appointmentTime = time; }

protected:
    std::string id_cuenta; ///< ID de la cuenta
    double saldo; ///< Saldo actual de la cuenta
    Fecha fecha_apertura; ///< Fecha de apertura de la cuenta
    ListaDoble<Movimiento>* movimientos; ///< Lista de movimientos de la cuenta
    int branchId; ///< ID de la sucursal asociada (1 = Norte, 2 = Centro, 3 = Sur)
    std::chrono::system_clock::time_point appointmentTime; ///< Hora de la cita asociada
};

#endif