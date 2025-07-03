#include "Cuenta.h"
#include "GestorClientes.h"
#include <stdexcept>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>


Cuenta::Cuenta() {
    id_cuenta = "";
    saldo = 0;
    movimientos = new ListaDoble<Movimiento>();
    branchId = 0;
    appointmentTime = std::chrono::system_clock::time_point();
}
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

Cuenta::~Cuenta() {
    delete movimientos;
}

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

std::string Cuenta::get_id_cuenta() { return id_cuenta; }
double Cuenta::get_saldo() { return saldo; }
Fecha Cuenta::get_fecha_apertura() { return fecha_apertura; }
ListaDoble<Movimiento>* Cuenta::get_movimientos() { return movimientos; }

void Cuenta::set_id_cuenta(std::string id) { id_cuenta = id; }
void Cuenta::set_saldo(double _saldo) { saldo = _saldo; }
void Cuenta::set_fecha_apertura(Fecha fecha) { fecha_apertura = fecha; }

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

double Cuenta::consultar_saldo() {
    return saldo;
}
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


