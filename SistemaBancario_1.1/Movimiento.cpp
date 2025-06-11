#include "Movimiento.h"
#include <stdexcept>

Movimiento::Movimiento() {
    monto = 0;
    saldo_post_movimiento = 0;
}

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

std::string Movimiento::get_tipo() const { return tipo; }
double Movimiento::get_monto() const { return monto; }
Fecha Movimiento::get_fecha() const { return fecha; }
double Movimiento::get_saldo_post_movimiento() const{ return saldo_post_movimiento; }

void Movimiento::set_tipo(std::string _tipo) { tipo = _tipo; }
void Movimiento::set_monto(double _monto) { monto = _monto; }
void Movimiento::set_fecha(Fecha _fecha) { fecha = _fecha; }
void Movimiento::set_saldo_post_movimiento(double _saldo) { saldo_post_movimiento = _saldo; }

std::string Movimiento::to_string() const {
    return "Movimiento: Tipo=" + tipo + ", Monto=" + std::to_string(monto) +
           ", Fecha=" + fecha.to_string() + ", Saldo posterior=" + std::to_string(saldo_post_movimiento);
}

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
std::ostream& operator<<(std::ostream& os, const Movimiento& mov) {
    os << mov.to_string();
    return os;
}
Movimiento::Movimiento(const Movimiento& otro)
    : tipo(otro.tipo),
      monto(otro.monto),
      fecha(otro.fecha),
      saldo_post_movimiento(otro.saldo_post_movimiento) {}