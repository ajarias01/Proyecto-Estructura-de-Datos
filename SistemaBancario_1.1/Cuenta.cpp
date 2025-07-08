/**
 * @file Cuenta.cpp
 * @brief Implementación de la clase Cuenta y sus métodos para la gestión de cuentas bancarias.
 *
 * Este archivo contiene la implementación de la clase Cuenta, que administra los datos de una cuenta bancaria,
 * operaciones de depósito, retiro, serialización y consulta de movimientos.
 */

#include "Cuenta.h"
#include "GestorClientes.h"
#include <stdexcept>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>

/**
 * @brief Constructor por defecto de Cuenta.
 * Inicializa los atributos básicos y la lista de movimientos vacía.
 */
Cuenta::Cuenta() {
    id_cuenta = "";
    saldo = 0;
    movimientos = new ListaDoble<Movimiento>();
    branchId = 0;
    appointmentTime = std::chrono::system_clock::time_point();
}

/**
 * @brief Constructor parametrizado de Cuenta.
 * @param id ID de la cuenta
 * @param saldo_inicial Saldo inicial de la cuenta
 * @param fecha Fecha de apertura
 */
Cuenta::Cuenta(std::string id, double saldo_inicial, Fecha fecha) {
    try {
        if (id.empty()) throw std::invalid_argument("ID de cuenta inválido");
        if (saldo_inicial < 0) throw std::invalid_argument("Saldo inicial no puede ser negativo");
        id_cuenta = id;
        saldo = saldo_inicial;
        fecha_apertura = fecha;
        movimientos = new ListaDoble<Movimiento>();
        branchId = 0;
        appointmentTime = std::chrono::system_clock::time_point();
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Cuenta: " << e.what() << std::endl;
        throw;
    }
}

/**
 * @brief Destructor de Cuenta.
 * Libera la memoria de la lista de movimientos.
 */
Cuenta::~Cuenta() {
    delete movimientos;
}

/**
 * @brief Guarda los datos de la cuenta en un archivo binario.
 * @param archivo Puntero al archivo binario abierto para escritura
 */
void Cuenta::guardar_binario(FILE* archivo) {
    try {
        if (!archivo) throw std::runtime_error("Archivo no válido para escritura");
        size_t len = id_cuenta.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(id_cuenta.c_str(), sizeof(char), len + 1, archivo);
        fwrite(&saldo, sizeof(double), 1, archivo);
        fwrite(&fecha_apertura, sizeof(Fecha), 1, archivo);
        fwrite(&branchId, sizeof(int), 1, archivo); // Guardar branchId
        // Convertir time_point a time_t para guardarlo (simplificación)
        time_t tt = std::chrono::system_clock::to_time_t(appointmentTime);
        fwrite(&tt, sizeof(time_t), 1, archivo); // Guardar appointmentTime
        movimientos->recorrer([&](Movimiento m) { m.guardar_binario(archivo); });
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_binario: " << e.what() << std::endl;
        throw;
    }
}

/**
 * @brief Carga los datos de la cuenta desde un archivo binario.
 * @param archivo Puntero al archivo binario abierto para lectura
 */
void Cuenta::cargar_binario(FILE* archivo) {
    try {
        if (!archivo) throw std::runtime_error("Archivo no válido para lectura");
        size_t len;
        char* buffer = nullptr;
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de id_cuenta");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer id_cuenta");
        }
        id_cuenta = std::string(buffer);
        delete[] buffer;

        if (fread(&saldo, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer saldo");
        if (fread(&fecha_apertura, sizeof(Fecha), 1, archivo) != 1) throw std::runtime_error("Error al leer fecha_apertura");
        if (fread(&branchId, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer branchId");
        time_t tt;
        if (fread(&tt, sizeof(time_t), 1, archivo) != 1) throw std::runtime_error("Error al leer appointmentTime");
        appointmentTime = std::chrono::system_clock::from_time_t(tt);
        delete movimientos;
        movimientos = new ListaDoble<Movimiento>();
        while (true) {
            Movimiento m;
            if (fread(&m, sizeof(Movimiento), 1, archivo) != 1) break;
            movimientos->insertar_cola(m);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en cargar_binario: " << e.what() << std::endl;
        throw;
    }
}

/**
 * @brief Obtiene el ID de la cuenta.
 * @return ID de la cuenta como string
 */
std::string Cuenta::get_id_cuenta() { return id_cuenta; }
/**
 * @brief Obtiene el saldo de la cuenta.
 * @return Saldo actual
 */
double Cuenta::get_saldo() { return saldo; }
/**
 * @brief Obtiene la fecha de apertura de la cuenta.
 * @return Fecha de apertura
 */
Fecha Cuenta::get_fecha_apertura() { return fecha_apertura; }
/**
 * @brief Obtiene la lista de movimientos de la cuenta.
 * @return Puntero a la lista doble de movimientos
 */
ListaDoble<Movimiento>* Cuenta::get_movimientos() { return movimientos; }

/**
 * @brief Establece el ID de la cuenta.
 * @param id Nuevo ID de la cuenta
 */
void Cuenta::set_id_cuenta(std::string id) { id_cuenta = id; }
/**
 * @brief Establece el saldo de la cuenta.
 * @param _saldo Nuevo saldo
 */
void Cuenta::set_saldo(double _saldo) { saldo = _saldo; }
/**
 * @brief Establece la fecha de apertura de la cuenta.
 * @param fecha Nueva fecha de apertura
 */
void Cuenta::set_fecha_apertura(Fecha fecha) { fecha_apertura = fecha; }

/**
 * @brief Realiza un depósito en la cuenta.
 * @param monto Monto a depositar
 * @param fecha Fecha del depósito
 */
void Cuenta::depositar(double monto, Fecha fecha) {
    try {
        if (monto <= 0) throw std::invalid_argument("Monto de depósito debe ser mayor a 0");
        if (!fecha.es_dia_habil()) throw std::invalid_argument("Depósito no permitido en día no hábil");
        saldo += monto;
        Movimiento movimiento("Depósito", monto, fecha, saldo);
        movimientos->insertar_cola(movimiento);
    } catch (const std::exception& e) {
        std::cerr << "Error en depositar: " << e.what() << std::endl;
    }
}

/**
 * @brief Realiza un retiro de la cuenta.
 * @param monto Monto a retirar
 * @param fecha Fecha del retiro
 * @return true si el retiro fue exitoso, false en caso contrario
 */
bool Cuenta::retirar(double monto, Fecha fecha) {
    try {
        if (monto <= 0) throw std::invalid_argument("Monto de retiro debe ser mayor a 0");
        if (saldo < monto) throw std::invalid_argument("Saldo insuficiente");
        //if (!fecha.es_dia_habil()) throw std::invalid_argument("Retiro no permitido en día no hábil");
        saldo -= monto;
        Movimiento movimiento("Retiro", monto, fecha, saldo);
        movimientos->insertar_cola(movimiento);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error en retirar: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Consulta el saldo actual de la cuenta.
 * @return Saldo actual
 */
double Cuenta::consultar_saldo() {
    return saldo;
}

/**
 * @brief Consulta e imprime los movimientos de la cuenta en un rango de fechas.
 * @param inicio Fecha de inicio
 * @param fin Fecha de fin
 */
void Cuenta::consultar_movimientos_rango(Fecha inicio, Fecha fin) {
    try {
        if (inicio > fin) throw std::invalid_argument("Rango de fechas inválido");
        std::cout << "  ==============================================\n";
        std::cout << "     MOVIMIENTOS DE LA CUENTA " << id_cuenta << "\n";
        std::cout << "     Desde: " << inicio.to_string() << "  Hasta: " << fin.to_string() << "\n";
        std::cout << "  ==============================================\n";

        std::vector<Movimiento> movs;
        movimientos->recorrer([&](Movimiento m) {
            movs.push_back(m);
        });

        if (movs.empty()) {
            std::cout << "  No hay movimientos registrados.\n";
            return;
        }

        GestorClientes::radixSortFecha(movs);

        int keyInicio = GestorClientes::dateKey(inicio);
        int keyFin = GestorClientes::dateKey(fin);

        int startIdx = GestorClientes::lowerBound(movs, keyInicio);
        int endIdx = GestorClientes::upperBound(movs, keyFin) - 1;

        if (startIdx > endIdx || startIdx >= (int)movs.size()) {
            std::cout << "  No hay movimientos en este rango de fechas.\n";
            return;
        }

        std::cout << "  Movimientos encontrados:\n";
        for (int i = startIdx; i <= endIdx; ++i) {
            const Movimiento& m = movs[i];
            std::cout << "    • Tipo: " << m.get_tipo()
                      << " | Monto: " << m.get_monto()
                      << " | Fecha: " << m.get_fecha().to_string()
                      << " | Saldo posterior: " << m.get_saldo_post_movimiento()
                      << std::endl;
        }
        std::cout << "  ==============================================\n";
    } catch (const std::exception& e) {
        std::cerr << "Error en consultar_movimientos_rango: " << e.what() << std::endl;
    }
}


