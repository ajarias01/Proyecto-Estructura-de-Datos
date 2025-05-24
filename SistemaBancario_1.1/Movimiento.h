#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED

#include "Fecha.h"
#include <string>

class Movimiento {
public:
    Movimiento();
    Movimiento(std::string tipo, double monto, Fecha fecha, double saldo_post);
    std::string get_tipo();
    double get_monto();
    Fecha get_fecha();
    double get_saldo_post_movimiento();
    void set_tipo(std::string);
    void set_monto(double);
    void set_fecha(Fecha);
    void set_saldo_post_movimiento(double);
    std::string to_string() const; // Añadido const
    void guardar_binario(FILE* archivo);
    void cargar_binario(FILE* archivo);
    friend std::ostream& operator<<(std::ostream& os, const Movimiento& mov); // Añadido
private:
    std::string tipo;
    double monto;
    Fecha fecha;
    double saldo_post_movimiento;
};

#endif