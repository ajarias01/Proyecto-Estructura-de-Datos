/**
 * @file Ahorro.cpp
 * @brief Implementación de la clase Ahorro para gestión de cuentas de ahorro bancarias
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo implementa la clase Ahorro que hereda de Cuenta y maneja
 *          cuentas de ahorro con tasas de interés. Incluye funcionalidades para
 *          crear, gestionar y persistir cuentas de ahorro en formato binario,
 *          así como validación de tasas de interés y manejo de excepciones.
 */

#include "Ahorro.h"
#include <stdexcept>
#include <cmath>

/**
 * @brief Constructor por defecto de la clase Ahorro
 * @details Inicializa una cuenta de ahorro con valores por defecto,
 *          estableciendo la tasa de interés en 0. Llama al constructor
 *          de la clase base Cuenta.
 */
Ahorro::Ahorro() : Cuenta() {
    tasa_interes = 0;
}

/**
 * @brief Constructor parametrizado de la clase Ahorro
 * @param id Identificador único de la cuenta de ahorro
 * @param saldo_inicial Saldo inicial de la cuenta (debe ser >= 0)
 * @param fecha Fecha de apertura de la cuenta
 * @param _tasa_interes Tasa de interés anual de la cuenta (debe estar entre 0 y 100)
 * @details Inicializa una cuenta de ahorro con los parámetros especificados.
 *          Valida que la tasa de interés esté dentro del rango válido (0-100%).
 *          Llama al constructor de la clase base Cuenta con los parámetros básicos.
 * @throw std::invalid_argument Si la tasa de interés está fuera del rango válido
 * @warning La tasa de interés debe estar entre 0 y 100, inclusive
 */
Ahorro::Ahorro(std::string id, double saldo_inicial, Fecha fecha, double _tasa_interes) : Cuenta(id, saldo_inicial, fecha) {
    try {
        // Validar que la tasa de interés esté en el rango válido (0-100%)
        if (_tasa_interes < 0 || _tasa_interes > 100) throw std::invalid_argument("Tasa de interés inválida");
        tasa_interes = _tasa_interes;
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Ahorro: " << e.what() << std::endl;
        throw;
    }
}

/**
 * @brief Obtiene el tipo de cuenta como string
 * @return String que identifica el tipo de cuenta como "Ahorros"
 * @details Método virtual que retorna el tipo específico de cuenta
 *          para identificación y clasificación en el sistema bancario.
 *          Implementa el método virtual de la clase base Cuenta.
 */
std::string Ahorro::get_tipo() {
    return "Ahorros";
}

/**
 * @brief Obtiene la tasa de interés actual de la cuenta de ahorro
 * @return Tasa de interés anual como porcentaje (double)
 * @details Método getter que retorna la tasa de interés configurada
 *          para esta cuenta de ahorro. La tasa se almacena como
 *          un porcentaje (ej: 5.5 para 5.5% anual).
 */
double Ahorro::get_tasa_interes() {
    return tasa_interes;
}

/**
 * @brief Establece una nueva tasa de interés para la cuenta de ahorro
 * @param _tasa Nueva tasa de interés a establecer (como porcentaje)
 * @details Método setter que permite modificar la tasa de interés
 *          de la cuenta de ahorro. La tasa debe proporcionarse como
 *          un porcentaje (ej: 5.5 para 5.5% anual).
 * @note Este método no valida el rango de la tasa. Se recomienda
 *       validar externamente que esté entre 0 y 100.
 */
void Ahorro::set_tasa_interes(double _tasa) {
    tasa_interes = _tasa;
}

/**
 * @brief Convierte la cuenta de ahorro a su representación en string
 * @return String con la información completa de la cuenta de ahorro
 * @details Genera una cadena formateada que incluye el tipo de cuenta,
 *          ID, saldo actual, fecha de apertura y tasa de interés.
 *          Útil para mostrar información de la cuenta al usuario.
 * @note La tasa de interés se muestra con el símbolo % al final
 */
std::string Ahorro::to_string() {
    return "Cuenta Ahorros: ID=" + id_cuenta + ", Saldo=" + std::to_string(saldo) +
           ", Fecha Apertura=" + fecha_apertura.to_string() + ", Tasa Interés=" + std::to_string(tasa_interes) + "%";
}

/**
 * @brief Guarda los datos de la cuenta de ahorro en formato binario
 * @param archivo Puntero al archivo donde se guardará la información
 * @details Serializa todos los datos de la cuenta de ahorro en formato binario,
 *          incluyendo ID, saldo, tasa de interés, fecha de apertura, branchId,
 *          tiempo de cita y lista de movimientos. Maneja excepciones durante
 *          el proceso de escritura.
 * @throw std::runtime_error Si el archivo no es válido o hay errores de escritura
 * @warning El archivo debe estar abierto en modo binario de escritura
 */
void Ahorro::guardar_binario(FILE* archivo) {
    try {
        // Validar que el archivo sea válido para escritura
        if (!archivo) throw std::runtime_error("Archivo no válido para escritura");
        
        // Guardar el ID de la cuenta (primero la longitud, luego el contenido)
        size_t len = id_cuenta.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(id_cuenta.c_str(), sizeof(char), len + 1, archivo);
        
        // Guardar datos básicos de la cuenta
        fwrite(&saldo, sizeof(double), 1, archivo);
        fwrite(&tasa_interes, sizeof(double), 1, archivo);
        fwrite(&fecha_apertura, sizeof(Fecha), 1, archivo);
        
        // Guardar información de sucursal y cita
        fwrite(&branchId, sizeof(int), 1, archivo);
        time_t tt = std::chrono::system_clock::to_time_t(appointmentTime);
        fwrite(&tt, sizeof(time_t), 1, archivo);
        
        // Guardar lista de movimientos
        int num_movimientos = 0;
        movimientos->recorrer([&](Movimiento) { num_movimientos++; });
        fwrite(&num_movimientos, sizeof(int), 1, archivo);
        movimientos->recorrer([&](Movimiento m) { m.guardar_binario(archivo); });
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_binario: " << e.what() << std::endl;
    }
}

/**
 * @brief Carga los datos de la cuenta de ahorro desde un archivo binario
 * @param archivo Puntero al archivo desde donde se cargarán los datos
 * @details Deserializa todos los datos de la cuenta de ahorro desde formato binario,
 *          incluyendo ID, saldo, tasa de interés, fecha de apertura, branchId,
 *          tiempo de cita y lista de movimientos. Maneja excepciones durante
 *          el proceso de lectura y recrea la estructura de datos completa.
 * @throw std::runtime_error Si el archivo no es válido o hay errores de lectura
 * @warning El archivo debe estar abierto en modo binario de lectura
 * @note Libera la memoria de movimientos existentes antes de cargar nuevos datos
 */
void Ahorro::cargar_binario(FILE* archivo) {
    try {
        // Validar que el archivo sea válido para lectura
        if (!archivo) throw std::runtime_error("Archivo no válido para lectura");
        
        // Cargar el ID de la cuenta
        size_t len;
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de id_cuenta");
        char* buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer id_cuenta");
        }
        id_cuenta = std::string(buffer);
        delete[] buffer;
        
        // Cargar datos básicos de la cuenta
        if (fread(&saldo, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer saldo");
        if (fread(&tasa_interes, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer tasa_interes");
        if (fread(&fecha_apertura, sizeof(Fecha), 1, archivo) != 1) throw std::runtime_error("Error al leer fecha_apertura");
        
        // Cargar información de sucursal y cita
        if (fread(&branchId, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer branchId");
        time_t tt;
        if (fread(&tt, sizeof(time_t), 1, archivo) != 1) throw std::runtime_error("Error al leer appointmentTime");
        appointmentTime = std::chrono::system_clock::from_time_t(tt);
        
        // Cargar lista de movimientos
        int num_movimientos;
        if (fread(&num_movimientos, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer número de movimientos");
        
        // Liberar memoria existente y crear nueva lista
        if (movimientos) {
            delete movimientos;
        }
        movimientos = new ListaDoble<Movimiento>();
        
        // Cargar cada movimiento individualmente
        for (int i = 0; i < num_movimientos; i++) {
            Movimiento movimiento;
            movimiento.cargar_binario(archivo);
            movimientos->insertar_cola(movimiento);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en cargar_binario: " << e.what() << std::endl;
        throw;
    }
}