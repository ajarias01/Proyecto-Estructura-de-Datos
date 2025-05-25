#include "Ahorro.h"
#include <stdexcept>
#include <cmath>

Ahorro::Ahorro() : Cuenta() {
    tasa_interes = 0;
}

Ahorro::Ahorro(std::string id, double saldo_inicial, Fecha fecha, double _tasa_interes) : Cuenta(id, saldo_inicial, fecha) {
    try {
        if (_tasa_interes < 0 || _tasa_interes > 100) throw std::invalid_argument("Tasa de interés inválida");
        tasa_interes = _tasa_interes;
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Ahorro: " << e.what() << std::endl;
        throw;
    }
}

std::string Ahorro::get_tipo() {
    return "Ahorros";
}

double Ahorro::get_tasa_interes() {
    return tasa_interes;
}

void Ahorro::set_tasa_interes(double _tasa) {
    tasa_interes = _tasa;
}

double Ahorro::calcular_intereses(Fecha hasta) {
    try {
        if (hasta < fecha_apertura) throw std::invalid_argument("Fecha inválida para cálculo de intereses");
        int meses_totales = (hasta.get_anuario() - fecha_apertura.get_anuario()) * 12 +
                           (hasta.get_mes() - fecha_apertura.get_mes());
        auto calcular_recursivo = [&](int meses_transcurridos, double saldo_actual, auto& self) -> double {
            if (meses_transcurridos >= meses_totales) return saldo_actual - saldo;
            double interes_mensual = saldo_actual * (tasa_interes / (100.0 * 12));
            saldo_actual += interes_mensual;
            return self(meses_transcurridos + 1, saldo_actual, self);
        };
        return calcular_recursivo(0, saldo, calcular_recursivo);
    } catch (const std::exception& e) {
        std::cerr << "Error en calcular_intereses: " << e.what() << std::endl;
        return 0;
    }
}

std::string Ahorro::to_string() {
    return "Cuenta Ahorros: ID=" + id_cuenta + ", Saldo=" + std::to_string(saldo) +
           ", Fecha Apertura=" + fecha_apertura.to_string() + ", Tasa Interés=" + std::to_string(tasa_interes) + "%";
}

void Ahorro::guardar_binario(FILE* archivo) {
    try {
        char tipo = 'A';
        fwrite(&tipo, sizeof(char), 1, archivo);
        fwrite(id_cuenta.c_str(), sizeof(char), id_cuenta.length() + 1, archivo);
        fwrite(&saldo, sizeof(double), 1, archivo);
        fwrite(&tasa_interes, sizeof(double), 1, archivo);
        fwrite(&fecha_apertura, sizeof(Fecha), 1, archivo);
        int num_movimientos = 0;
        movimientos->recorrer([&](Movimiento) { num_movimientos++; });
        fwrite(&num_movimientos, sizeof(int), 1, archivo);
        movimientos->recorrer([&](Movimiento m) { m.guardar_binario(archivo); });
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_binario: " << e.what() << std::endl;
    }
}

void Ahorro::cargar_binario(FILE* archivo) {
    try {
        char buffer[50];
        fread(buffer, sizeof(char), 50, archivo);
        id_cuenta = std::string(buffer);
        fread(&saldo, sizeof(double), 1, archivo);
        fread(&tasa_interes, sizeof(double), 1, archivo);
        fread(&fecha_apertura, sizeof(Fecha), 1, archivo);
        int num_movimientos;
        fread(&num_movimientos, sizeof(int), 1, archivo);
        movimientos = new ListaDoble<Movimiento>();
        for (int i = 0; i < num_movimientos; i++) {
            Movimiento movimiento;
            movimiento.cargar_binario(archivo);
            movimientos->insertar_cola(movimiento);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en cargar_binario: " << e.what() << std::endl;
    }
}