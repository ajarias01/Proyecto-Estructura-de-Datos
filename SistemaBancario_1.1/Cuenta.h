#ifndef CUENTA_H_INCLUDED
#define CUENTA_H_INCLUDED

#include "Movimiento.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>

class Cuenta {
public:
    Cuenta();
    Cuenta(int id, double saldo_inicial, Fecha fecha);
    virtual ~Cuenta();
    int get_id_cuenta();
    double get_saldo();
    Fecha get_fecha_apertura();
    ListaDoble<Movimiento>* get_movimientos();
    void set_id_cuenta(int);
    void set_saldo(double);
    void set_fecha_apertura(Fecha);
    virtual std::string get_tipo() = 0;
    virtual double calcular_intereses(Fecha hasta) = 0;
    virtual std::string to_string() = 0;
    virtual void guardar_binario(FILE* archivo) = 0;
    virtual void cargar_binario(FILE* archivo) = 0;
    virtual bool retirar(double monto, Fecha fecha); // Cambiado a virtual
    void depositar(double monto, Fecha fecha);
    double consultar_saldo();
    void consultar_movimientos_rango(Fecha inicio, Fecha fin);
protected:
    int id_cuenta;
    double saldo;
    Fecha fecha_apertura;
    ListaDoble<Movimiento>* movimientos;
};

#endif