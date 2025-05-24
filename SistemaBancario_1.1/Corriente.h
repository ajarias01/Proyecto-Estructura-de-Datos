#ifndef CORRIENTE_H_INCLUDED
#define CORRIENTE_H_INCLUDED

#include "Cuenta.h"
#include <string>

class Corriente : public Cuenta {
public:
    Corriente();
    Corriente(int id, double saldo_inicial, Fecha fecha, double limite_retiro_diario = 1000.0);
    std::string get_tipo() override;
    double get_limite_retiro_diario();
    void set_limite_retiro_diario(double);
    double calcular_intereses(Fecha hasta) override;
    std::string to_string() override;
    void guardar_binario(FILE* archivo) override;
    void cargar_binario(FILE* archivo) override;
    bool retirar(double monto, Fecha fecha) override;
private:
    double limite_retiro_diario;
    double monto_retirado_hoy;
    Fecha ultimo_dia_retiro;
};

#endif