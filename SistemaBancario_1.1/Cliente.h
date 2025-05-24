#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include "Cuenta.h"
#include "Ahorro.h"
#include "Corriente.h"
#include "Fecha.h"
#include "ListaDoble.h"
#include <string>

class Cliente {
public:
    Cliente();
    Cliente(std::string dni, std::string nombre, std::string apellido, std::string direccion, std::string telefono, 
            std::string email, Fecha fecha_nacimiento);
    ~Cliente();
    std::string get_dni();
    std::string get_nombre();
    std::string get_apellido();
    std::string get_direccion();
    std::string get_telefono();
    std::string get_email();
    Fecha get_fecha_nacimiento();
    ListaDoble<Cuenta*>* get_cuentas();
    void set_dni(std::string);
    void set_nombre(std::string);
    void set_apellido(std::string);
    void set_direccion(std::string);
    void set_telefono(std::string);
    void set_email(std::string);
    void set_fecha_nacimiento(Fecha);
    void agregar_cuenta(Cuenta* cuenta);
    Cuenta* buscar_cuenta(int id_cuenta);
    std::string to_string() const;
    void guardar_binario(FILE* archivo);
    void cargar_binario(FILE* archivo);
    friend std::ostream& operator<<(std::ostream& os, const Cliente& cliente);
private:
    std::string dni;
    std::string nombre;
    std::string apellido;
    std::string direccion;
    std::string telefono;
    std::string email;
    Fecha fecha_nacimiento;
    ListaDoble<Cuenta*>* cuentas;
};

#endif