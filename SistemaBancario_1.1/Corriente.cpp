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

std::string Corriente::to_string() {
    return "Cuenta Corriente: ID=" + id_cuenta + ", Saldo=" + std::to_string(saldo) +
           ", Fecha Apertura=" + fecha_apertura.to_string() + ", Límite Retiro Diario=" + std::to_string(limite_retiro_diario);
}

void Corriente::guardar_binario(FILE* archivo) {
    try {
        if (!archivo) throw std::runtime_error("Archivo no válido para escritura");
        char tipo = 'C';
        fwrite(&tipo, sizeof(char), 1, archivo);
        size_t len = id_cuenta.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(id_cuenta.c_str(), sizeof(char), len + 1, archivo);
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
        if (!archivo) throw std::runtime_error("Archivo no válido para lectura");
        size_t len;
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de id_cuenta");
        char* buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer id_cuenta");
        }
        id_cuenta = std::string(buffer);
        delete[] buffer;
        if (fread(&saldo, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer saldo");
        if (fread(&limite_retiro_diario, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer limite_retiro_diario");
        if (fread(&monto_retirado_hoy, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer monto_retirado_hoy");
        if (fread(&ultimo_dia_retiro, sizeof(Fecha), 1, archivo) != 1) throw std::runtime_error("Error al leer ultimo_dia_retiro");
        if (fread(&fecha_apertura, sizeof(Fecha), 1, archivo) != 1) throw std::runtime_error("Error al leer fecha_apertura");
        int num_movimientos;
        if (fread(&num_movimientos, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer número de movimientos");
        if (movimientos) {
            movimientos->recorrer([](Movimiento) {});
            delete movimientos;
        }
        movimientos = new ListaDoble<Movimiento>();
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