/**
 * @file Movimiento.cpp
 * @brief Implementación de la clase Movimiento para representar transacciones bancarias.
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 */

#include "Movimiento.h"
#include <stdexcept>

/**
 * @brief Constructor por defecto de la clase Movimiento.
 * @details Inicializa un movimiento con valores por defecto (monto y saldo en 0).
 */
Movimiento::Movimiento() {
    monto = 0;
    saldo_post_movimiento = 0;
}

/**
 * @brief Constructor parametrizado de la clase Movimiento.
 * @param _tipo Tipo de movimiento ("Depósito" o "Retiro").
 * @param _monto Monto de la transacción.
 * @param _fecha Fecha de la transacción.
 * @param _saldo_post Saldo posterior a la transacción.
 * @throws std::invalid_argument Si el tipo no es válido o el monto es negativo.
 */
Movimiento::Movimiento(std::string _tipo, double _monto, Fecha _fecha, double _saldo_post) {
    try {
        if (_tipo != "Depósito" && _tipo != "Retiro") throw std::invalid_argument("Tipo de movimiento inválido");
        if (_monto <= 0) throw std::invalid_argument("Monto debe ser mayor a 0");
        tipo = _tipo;
        monto = _monto;
        fecha = _fecha;
        saldo_post_movimiento = _saldo_post;
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Movimiento: " << e.what() << std::endl;
        throw;
    }
}

/**
 * @brief Obtiene el tipo de movimiento.
 * @return String con el tipo de movimiento.
 */
std::string Movimiento::get_tipo() const { return tipo; }

/**
 * @brief Obtiene el monto del movimiento.
 * @return Double con el monto de la transacción.
 */
double Movimiento::get_monto() const { return monto; }

/**
 * @brief Obtiene la fecha del movimiento.
 * @return Objeto Fecha con la fecha de la transacción.
 */
Fecha Movimiento::get_fecha() const { return fecha; }

/**
 * @brief Obtiene el saldo posterior al movimiento.
 * @return Double con el saldo después de la transacción.
 */
double Movimiento::get_saldo_post_movimiento() const{ return saldo_post_movimiento; }

/**
 * @brief Establece el tipo de movimiento.
 * @param _tipo Nuevo tipo de movimiento.
 */
void Movimiento::set_tipo(std::string _tipo) { tipo = _tipo; }

/**
 * @brief Establece el monto del movimiento.
 * @param _monto Nuevo monto de la transacción.
 */
void Movimiento::set_monto(double _monto) { monto = _monto; }

/**
 * @brief Establece la fecha del movimiento.
 * @param _fecha Nueva fecha de la transacción.
 */
void Movimiento::set_fecha(Fecha _fecha) { fecha = _fecha; }

/**
 * @brief Establece el saldo posterior al movimiento.
 * @param _saldo Nuevo saldo posterior a la transacción.
 */
void Movimiento::set_saldo_post_movimiento(double _saldo) { saldo_post_movimiento = _saldo; }

/**
 * @brief Convierte el movimiento a una representación en cadena.
 * @return String con toda la información del movimiento formateada.
 * @details Genera una cadena que incluye el tipo de movimiento, monto, fecha y saldo posterior.
 */
std::string Movimiento::to_string() const {
    return "Movimiento: Tipo=" + tipo + ", Monto=" + std::to_string(monto) +
           ", Fecha=" + fecha.to_string() + ", Saldo posterior=" + std::to_string(saldo_post_movimiento);
}

/**
 * @brief Guarda el movimiento en un archivo binario.
 * @param archivo Puntero al archivo donde se guardará el movimiento.
 * @details Serializa todos los datos del movimiento en formato binario,
 * incluyendo la longitud del tipo, el tipo, monto, fecha y saldo posterior.
 * @throws std::exception Si ocurre un error durante la escritura.
 */
void Movimiento::guardar_binario(FILE* archivo) {
    if (!archivo) {
        std::cerr << "Error: Archivo no válido para escritura" << std::endl;
        return;
    }
    try {
        size_t len = tipo.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(tipo.c_str(), sizeof(char), len + 1, archivo);
        fwrite(&monto, sizeof(double), 1, archivo);
        fwrite(&fecha, sizeof(Fecha), 1, archivo);
        fwrite(&saldo_post_movimiento, sizeof(double), 1, archivo);
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar movimiento en archivo binario: " << e.what() << std::endl;
    }
}

/**
 * @brief Carga un movimiento desde un archivo binario.
 * @param archivo Puntero al archivo desde donde se cargará el movimiento.
 * @details Deserializa los datos del movimiento desde formato binario,
 * leyendo la longitud del tipo, el tipo, monto, fecha y saldo posterior.
 * Incluye validación de longitud del tipo para evitar lecturas corruptas.
 * @throws std::runtime_error Si ocurre un error durante la lectura o los datos están corruptos.
 */
void Movimiento::cargar_binario(FILE* archivo) {
    if (!archivo) {
        std::cerr << "Error: Archivo no válido para lectura" << std::endl;
        return;
    }
    try {
        size_t len;
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de tipo");
        if (len > 100) throw std::runtime_error("Longitud de tipo de movimiento inválida");
        char* buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer tipo de movimiento");
        }
        tipo = std::string(buffer);
        delete[] buffer;

        if (fread(&monto, sizeof(double), 1, archivo) != 1) {
            throw std::runtime_error("Error al leer el monto");
        }
        if (fread(&fecha, sizeof(Fecha), 1, archivo) != 1) {
            throw std::runtime_error("Error al leer la fecha");
        }
        if (fread(&saldo_post_movimiento, sizeof(double), 1, archivo) != 1) {
            throw std::runtime_error("Error al leer el saldo posterior");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar movimiento desde archivo binario: " << e.what() << std::endl;
    }
}

/**
 * @brief Operador de salida para la clase Movimiento.
 * @param os Flujo de salida donde se escribirá el movimiento.
 * @param mov Objeto Movimiento a escribir.
 * @return Referencia al flujo de salida.
 * @details Permite usar el operador << para escribir un movimiento a cualquier flujo de salida.
 */
std::ostream& operator<<(std::ostream& os, const Movimiento& mov) {
    os << mov.to_string();
    return os;
}

/**
 * @brief Constructor de copia de la clase Movimiento.
 * @param otro Objeto Movimiento del cual se copiará la información.
 * @details Crea una copia exacta del movimiento proporcionado, copiando
 * todos los atributos: tipo, monto, fecha y saldo posterior.
 */
Movimiento::Movimiento(const Movimiento& otro)
    : tipo(otro.tipo),
      monto(otro.monto),
      fecha(otro.fecha),
      saldo_post_movimiento(otro.saldo_post_movimiento) {}