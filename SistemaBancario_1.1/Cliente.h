#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
#include "Fecha.h"
#include "ListaDoble.h"
#include "Cuenta.h"

class Cliente
{
public:
    Cliente();
    Cliente(std::string dni, std::string nombres, std::string apellidos, std::string direccion,
            std::string telefono, std::string email, Fecha fecha_nacimiento, std::string contrasenia);
    Cliente(const Cliente& otro); // Constructor de copia
    Cliente& operator=(const Cliente& otro); // Operador de asignación
    ~Cliente();
    
    std::string get_dni() const;
    std::string get_nombres() const;
    std::string get_apellidos() const;
    std::string get_direccion() const;
    std::string get_telefono() const;
    std::string get_email() const;
    Fecha get_fecha_nacimiento() const;
    std::string get_contrasenia() const;
    ListaDoble<Cuenta*>* get_cuentas() const;
    
    void set_dni(std::string dni);
    void set_nombres(std::string nombres);
    void set_apellidos(std::string apellidos);
    void set_direccion(std::string direccion);
    void set_telefono(std::string telefono);
    void set_email(std::string email);
    void set_fecha_nacimiento(Fecha fecha_nacimiento);
    void set_contrasenia(std::string contrasenia);
    
    void agregar_cuenta(Cuenta* cuenta);
    Cuenta* buscar_cuenta(std::string id_cuenta);
    
    std::string to_string() const; // Declaración de to_string
    
    void guardar_binario(FILE* file);
    void cargar_binario(FILE* file);
    
private:
    std::string dni;
    std::string nombres;
    std::string apellidos;
    std::string direccion;
    std::string telefono;
    std::string email;
    Fecha fecha_nacimiento;
    std::string contrasenia;
    ListaDoble<Cuenta*>* cuentas;
};

#endif