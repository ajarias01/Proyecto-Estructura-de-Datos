/**
 * @file Movimiento.h
 * @brief Definición de la clase Movimiento para representar transacciones bancarias.
 * @author Sistema Bancario
 * @date 2024
 */

#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED

#include "Fecha.h"
#include <string>

/**
 * @class Movimiento
 * @brief Clase que representa un movimiento bancario (depósito o retiro).
 * @details Esta clase encapsula la información de las transacciones bancarias,
 * incluyendo el tipo de movimiento, monto, fecha y saldo posterior a la transacción.
 */
class Movimiento {
public:
    /**
     * @brief Constructor por defecto.
     * @details Inicializa un movimiento con valores por defecto.
     */
    Movimiento();
    
    /**
     * @brief Constructor parametrizado.
     * @param tipo Tipo de movimiento ("Depósito" o "Retiro").
     * @param monto Monto de la transacción.
     * @param fecha Fecha de la transacción.
     * @param saldo_post Saldo posterior a la transacción.
     * @throws std::invalid_argument Si el tipo no es válido o el monto es negativo.
     */
    Movimiento(std::string tipo, double monto, Fecha fecha, double saldo_post);
    
    /**
     * @brief Constructor de copia.
     * @param otro Objeto Movimiento a copiar.
     */
    Movimiento(const Movimiento& otro);
    
    /**
     * @brief Obtiene el tipo de movimiento.
     * @return String con el tipo de movimiento.
     */
    std::string get_tipo() const;
    
    /**
     * @brief Obtiene el monto del movimiento.
     * @return Double con el monto de la transacción.
     */
    double get_monto()const;
    
    /**
     * @brief Obtiene la fecha del movimiento.
     * @return Objeto Fecha con la fecha de la transacción.
     */
    Fecha get_fecha() const;
    
    /**
     * @brief Obtiene el saldo posterior al movimiento.
     * @return Double con el saldo después de la transacción.
     */
    double get_saldo_post_movimiento() const;
    
    /**
     * @brief Establece el tipo de movimiento.
     * @param tipo Nuevo tipo de movimiento.
     */
    void set_tipo(std::string);
    
    /**
     * @brief Establece el monto del movimiento.
     * @param monto Nuevo monto de la transacción.
     */
    void set_monto(double);
    
    /**
     * @brief Establece la fecha del movimiento.
     * @param fecha Nueva fecha de la transacción.
     */
    void set_fecha(Fecha);
    
    /**
     * @brief Establece el saldo posterior al movimiento.
     * @param saldo_post Nuevo saldo posterior a la transacción.
     */
    void set_saldo_post_movimiento(double);
    
    /**
     * @brief Convierte el movimiento a string.
     * @return String con la representación del movimiento.
     */
    std::string to_string() const; // Añadido const
    
    /**
     * @brief Guarda el movimiento en un archivo binario.
     * @param archivo Puntero al archivo donde se guardará el movimiento.
     * @details Serializa todos los datos del movimiento en formato binario.
     */
    void guardar_binario(FILE* archivo);
    
    /**
     * @brief Carga un movimiento desde un archivo binario.
     * @param archivo Puntero al archivo desde donde se cargará el movimiento.
     * @details Deserializa los datos del movimiento desde formato binario.
     * @throws std::runtime_error Si los datos están corruptos o hay errores de lectura.
     */
    void cargar_binario(FILE* archivo);
    
    /**
     * @brief Operador de salida para la clase Movimiento.
     * @param os Flujo de salida donde se escribirá el movimiento.
     * @param mov Objeto Movimiento a escribir.
     * @return Referencia al flujo de salida.
     */
    friend std::ostream& operator<<(std::ostream& os, const Movimiento& mov); // Añadido
    
private:
    /**
     * @brief Tipo de movimiento (Depósito o Retiro).
     */
    std::string tipo;
    
    /**
     * @brief Monto de la transacción.
     */
    double monto;
    
    /**
     * @brief Fecha en que se realizó la transacción.
     */
    Fecha fecha;
    
    /**
     * @brief Saldo de la cuenta después de realizar la transacción.
     */
    double saldo_post_movimiento;
};

#endif