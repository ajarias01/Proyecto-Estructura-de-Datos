#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED

#include "Fecha.h"
#include <string>

class Movimiento {
public:
    Movimiento();
    Movimiento(std::string tipo, double monto, Fecha fecha, double saldo_post);
    Movimiento(const Movimiento& otro);
    std::string get_tipo() const;
    double get_monto()const;
    Fecha get_fecha() const;
    double get_saldo_post_movimiento() const;
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