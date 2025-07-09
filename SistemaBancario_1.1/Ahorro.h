#ifndef AHORRO_H_INCLUDED
#define AHORRO_H_INCLUDED

#include "Cuenta.h"
#include <string>
/**
 * @class Ahorro
 * @brief Clase que representa una cuenta de ahorro, derivada de Cuenta.
 *
 * La clase Ahorro extiende la funcionalidad de una cuenta bancaria básica,
 * añadiendo el atributo de tasa de interés.
 */
class Ahorro : public Cuenta {
    
public:
     /**
     * @brief Constructor por defecto de la clase Ahorro.
     */
    Ahorro();
    /**
     * @brief Constructor con parámetros.
     * @param id Identificador único de la cuenta.
     * @param saldo_inicial Saldo inicial de la cuenta.
     * @param fecha Fecha de creación o apertura de la cuenta.
     * @param tasa_interes Tasa de interés aplicada a la cuenta.
     */
    Ahorro(std::string id, double saldo_inicial, Fecha fecha, double tasa_interes);
     /**
     * @brief Obtiene el tipo de cuenta.
     * @return Una cadena que representa el tipo de cuenta (por ejemplo, "Ahorro").
     */
    std::string get_tipo() override;
    /**
     * @brief Obtiene la tasa de interés de la cuenta.
     * @return La tasa de interés como valor decimal.
     */
    double get_tasa_interes();
    /**
     * @brief Establece la tasa de interés de la cuenta.
     * @param tasa_interes Nuevo valor de la tasa de interés.
     */
    void set_tasa_interes(double);
    /**
     * @brief Convierte los datos de la cuenta a una cadena descriptiva.
     * @return Cadena con la información de la cuenta en formato legible.
     */
    std::string to_string() override;
    /**
     * @brief Guarda los datos de la cuenta en un archivo binario.
     * @param archivo Puntero al archivo donde se guardarán los datos.
     */
    void guardar_binario(FILE* archivo) override;
     /**
     * @brief Carga los datos de la cuenta desde un archivo binario.
     * @param archivo Puntero al archivo desde donde se leerán los datos.
     */
    void cargar_binario(FILE* archivo) override;
private:
    double tasa_interes;
};

#endif