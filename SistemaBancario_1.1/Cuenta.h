#ifndef CUENTA_H_INCLUDED
#define CUENTA_H_INCLUDED

#include "Movimiento.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>
#include <chrono> // Para time_point

class Cuenta {
public:
    Cuenta();
    Cuenta(std::string id, double saldo_inicial, Fecha fecha);
    virtual ~Cuenta();
    std::string get_id_cuenta();
    double get_saldo();
    Fecha get_fecha_apertura();
    ListaDoble<Movimiento>* get_movimientos();
    void set_id_cuenta(std::string);
    void set_saldo(double);
    void set_fecha_apertura(Fecha);
    virtual std::string get_tipo() = 0;
    virtual std::string to_string() = 0;
    virtual void guardar_binario(FILE* archivo) = 0;
    virtual void cargar_binario(FILE* archivo) = 0;
    virtual bool retirar(double monto, Fecha fecha);
    void depositar(double monto, Fecha fecha);
    double consultar_saldo();
    void consultar_movimientos_rango(Fecha inicio, Fecha fin);

    // Nuevos métodos para sucursales y citas
    int get_branchId() const { return branchId; }
    void set_branchId(int id) { branchId = id; }
    std::chrono::system_clock::time_point get_appointmentTime() const { return appointmentTime; }
    void set_appointmentTime(std::chrono::system_clock::time_point time) { appointmentTime = time; }

protected:
    std::string id_cuenta;
    double saldo;
    Fecha fecha_apertura;
    ListaDoble<Movimiento>* movimientos;
    int branchId; // 1 = Norte, 2 = Centro, 3 = Sur
    std::chrono::system_clock::time_point appointmentTime; // Hora de la cita
};

#endif