#include "Cuenta.h"
#include "Sorting.h"
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
}
Cuenta::Cuenta(std::string id, double saldo_inicial, Fecha fecha) {
    try {
        if (id.empty()) throw std::invalid_argument("ID de cuenta inválido");
        if (saldo_inicial < 0) throw std::invalid_argument("Saldo inicial no puede ser negativo");
        id_cuenta = id;
        saldo = saldo_inicial;
        fecha_apertura = fecha;
        movimientos = new ListaDoble<Movimiento>();
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Cuenta: " << e.what() << std::endl;
        throw;
    }
}

Cuenta::~Cuenta() {
    delete movimientos;
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

        radixSortMovements(movs);

        int keyInicio = dateKey(inicio);
        int keyFin = dateKey(fin);

        int startIdx = lowerBound(movs, keyInicio);
        int endIdx = upperBound(movs, keyFin) - 1;

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
