#include "Cuenta.h"
#include <stdexcept>
#include <functional>

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
        if (!fecha.es_dia_habil()) throw std::invalid_argument("Retiro no permitido en día no hábil");
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
        std::cout << "Movimientos de la cuenta " << id_cuenta << " entre " << inicio.to_string() << " y " << fin.to_string() << ":\n";
        movimientos->filtrar(
            [&inicio, &fin](Movimiento m) { return m.get_fecha() >= inicio && m.get_fecha() <= fin; },
            [](Movimiento m) { std::cout << m.to_string() << std::endl; }
        );
    } catch (const std::exception& e) {
        std::cerr << "Error en consultar_movimientos_rango: " << e.what() << std::endl;
    }
}