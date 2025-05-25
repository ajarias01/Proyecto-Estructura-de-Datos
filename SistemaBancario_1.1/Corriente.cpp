#include "Corriente.h"
#include <stdexcept>

Corriente::Corriente() : Cuenta() {
    limite_retiro_diario = 1000.0;
    monto_retirado_hoy = 0;
}

Corriente::Corriente(std::string id, double saldo_inicial, Fecha fecha, double _limite_retiro_diario) : Cuenta(id, saldo_inicial, fecha) {
    try {
        if (_limite_retiro_diario <= 0) throw std::invalid_argument("Límite de retiro diario inválido");
        limite_retiro_diario = _limite_retiro_diario;
        monto_retirado_hoy = 0;
        ultimo_dia_retiro = fecha;
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Corriente: " << e.what() << std::endl;
        throw;
    }
}

std::string Corriente::get_tipo() {
    return "Corriente";
}

double Corriente::get_limite_retiro_diario() {
    return limite_retiro_diario;
}

void Corriente::set_limite_retiro_diario(double _limite) {
    limite_retiro_diario = _limite;
}

double Corriente::calcular_intereses(Fecha hasta) {
    return 0;
}

std::string Corriente::to_string() {
    return "Cuenta Corriente: ID=" + id_cuenta + ", Saldo=" + std::to_string(saldo) +
           ", Fecha Apertura=" + fecha_apertura.to_string() + ", Límite Retiro Diario=" + std::to_string(limite_retiro_diario);
}

void Corriente::guardar_binario(FILE* archivo) {
    try {
        char tipo = 'C';
        fwrite(&tipo, sizeof(char), 1, archivo);
        fwrite(id_cuenta.c_str(), sizeof(char), id_cuenta.length() + 1, archivo);
        fwrite(&saldo, sizeof(double), 1, archivo);
        fwrite(&limite_retiro_diario, sizeof(double), 1, archivo);
        fwrite(&monto_retirado_hoy, sizeof(double), 1, archivo);
        fwrite(&ultimo_dia_retiro, sizeof(Fecha), 1, archivo);
        fwrite(&fecha_apertura, sizeof(Fecha), 1, archivo);
        int num_movimientos = 0;
        movimientos->recorrer([&](Movimiento) { num_movimientos++; });
        fwrite(&num_movimientos, sizeof(int), 1, archivo);
        movimientos->recorrer([&](Movimiento m) { m.guardar_binario(archivo); });
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_binario: " << e.what() << std::endl;
    }
}

void Corriente::cargar_binario(FILE* archivo) {
    try {
        char buffer[50];
        fread(buffer, sizeof(char), 50, archivo);
        id_cuenta = std::string(buffer);
        fread(&saldo, sizeof(double), 1, archivo);
        fread(&limite_retiro_diario, sizeof(double), 1, archivo);
        fread(&monto_retirado_hoy, sizeof(double), 1, archivo);
        fread(&ultimo_dia_retiro, sizeof(Fecha), 1, archivo);
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

bool Corriente::retirar(double monto, Fecha fecha) {
    try {
        if (monto <= 0) throw std::invalid_argument("Monto de retiro debe ser mayor a 0");
        if (saldo < monto) throw std::invalid_argument("Saldo insuficiente");
        if (!fecha.es_dia_habil()) throw std::invalid_argument("Retiro no permitido en día no hábil");
        if (fecha.get_anuario() != ultimo_dia_retiro.get_anuario() ||
            fecha.get_mes() != ultimo_dia_retiro.get_mes() ||
            fecha.get_dia() != ultimo_dia_retiro.get_dia()) {
            monto_retirado_hoy = 0;
            ultimo_dia_retiro = fecha;
        }
        if (monto_retirado_hoy + monto > limite_retiro_diario) {
            throw std::invalid_argument("Excede el límite de retiro diario");
        }
        saldo -= monto;
        monto_retirado_hoy += monto;
        Movimiento movimiento("Retiro", monto, fecha, saldo);
        movimientos->insertar_cola(movimiento);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error en retirar: " << e.what() << std::endl;
        return false;
    }
}