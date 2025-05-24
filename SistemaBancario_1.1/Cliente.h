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
    Cliente(std::string dni, std::string nombre, std::string direccion, std::string telefono, 
            std::string email, Fecha fecha_nacimiento); //std::string estado_civil, std::string nif);
    ~Cliente();
    std::string get_dni();
    std::string get_nombre();
    std::string get_direccion();
    std::string get_telefono();
    std::string get_email();
    Fecha get_fecha_nacimiento();
    //std::string get_estado_civil();
    //std::string get_numero_identificacion_fiscal();
    ListaDoble<Cuenta*>* get_cuentas();
    void set_dni(std::string);
    void set_nombre(std::string);
    void set_direccion(std::string);
    void set_telefono(std::string);
    void set_email(std::string);
    void set_fecha_nacimiento(Fecha);
    //void set_estado_civil(std::string);
    //void set_numero_identificacion_fiscal(std::string);
    void agregar_cuenta(Cuenta* cuenta);
    Cuenta* buscar_cuenta(int id_cuenta);
    std::string to_string() const; // Añadido const
    void guardar_binario(FILE* archivo);
    void cargar_binario(FILE* archivo);
    friend std::ostream& operator<<(std::ostream& os, const Cliente& cliente); // Añadido
private:
    std::string dni;
    std::string nombre;
    std::string direccion;
    std::string telefono;
    std::string email;
    Fecha fecha_nacimiento;
    //std::string estado_civil;
    //std::string numero_identificacion_fiscal;
    ListaDoble<Cuenta*>* cuentas;
};

#endif