/**
 * @file Corriente.h
 * @brief Definición de la clase Corriente para cuentas corrientes bancarias.
 *
 * Esta clase representa una cuenta corriente con límite de retiro diario y operaciones específicas.
 */

#ifndef CORRIENTE_H_INCLUDED
#define CORRIENTE_H_INCLUDED

#include "Cuenta.h"
#include <string>

/**
 * @class Corriente
 * @brief Representa una cuenta corriente bancaria.
 *
 * Hereda de Cuenta e implementa operaciones con límite de retiro diario, serialización y
 * métodos para la gestión de retiros y datos binarios.
 */
class Corriente : public Cuenta {
public:
    /**
     * @brief Constructor por defecto de Corriente.
     */
    Corriente();
    /**
     * @brief Constructor parametrizado de Corriente.
     * @param id ID de la cuenta
     * @param saldo_inicial Saldo inicial de la cuenta
     * @param fecha Fecha de apertura
     * @param limite_retiro_diario Límite de retiro diario (por defecto 1000.0)
     */
    Corriente(std::string id, double saldo_inicial, Fecha fecha, double limite_retiro_diario = 1000.0);
    /**
     * @brief Obtiene el tipo de cuenta.
     * @return String "Corriente"
     */
    std::string get_tipo() override;
    /**
     * @brief Obtiene el límite de retiro diario.
     * @return Límite de retiro diario
     */
    double get_limite_retiro_diario();
    /**
     * @brief Establece el límite de retiro diario.
     * @param limite Nuevo límite de retiro diario
     */
    void set_limite_retiro_diario(double limite);
    /**
     * @brief Devuelve una representación en string de la cuenta corriente.
     * @return String con los datos principales de la cuenta
     */
    std::string to_string() override;
    /**
     * @brief Guarda los datos de la cuenta corriente en un archivo binario.
     * @param archivo Puntero al archivo binario abierto para escritura
     */
    void guardar_binario(FILE* archivo) override;
    /**
     * @brief Carga los datos de la cuenta corriente desde un archivo binario.
     * @param archivo Puntero al archivo binario abierto para lectura
     */
    void cargar_binario(FILE* archivo) override;
    /**
     * @brief Realiza un retiro de la cuenta corriente, validando límites y días hábiles.
     * @param monto Monto a retirar
     * @param fecha Fecha del retiro
     * @return true si el retiro fue exitoso, false en caso contrario
     */
    bool retirar(double monto, Fecha fecha) override;
private:
    double limite_retiro_diario; ///< Límite de retiro diario
    double monto_retirado_hoy;   ///< Monto retirado en el día actual
    Fecha ultimo_dia_retiro;     ///< Última fecha en la que se realizó un retiro
};

#endif