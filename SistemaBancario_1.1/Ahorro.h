#ifndef AHORRO_H_INCLUDED
#define AHORRO_H_INCLUDED

#include "Cuenta.h"
#include <string>

class Ahorro : public Cuenta {
public:
    Ahorro();
    Ahorro(int id, double saldo_inicial, Fecha fecha, double tasa_interes);
    std::string get_tipo() override;
    double get_tasa_interes();
    void set_tasa_interes(double);
    double calcular_intereses(Fecha hasta) override;
    std::string to_string() override;
    void guardar_binario(FILE* archivo) override;
    void cargar_binario(FILE* archivo) override;
private:
    double tasa_interes;
};

#endif