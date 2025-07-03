#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include "Cuenta.h"
#include "ListaDoble.h"
#include "Fecha.h"
#include <string>

class Cliente {
public:
    Cliente();
    Cliente(std::string _dni, std::string _nombres, std::string _apellidos, std::string _direccion, 
            std::string _telefono, std::string _email, Fecha _fecha_nacimiento, std::string _contrasenia);
    Cliente(const Cliente& otro); // Declaración del constructor de copia
    ~Cliente();
    Cliente& operator=(const Cliente& otro); // Declaración del operador de asignación
    std::string get_dni() const; // Añadir const
    std::string get_nombres() const; // Añadir const
    std::string get_apellidos() const; // Añadir const
    std::string get_direccion() const; // Añadir const
    std::string get_telefono() const; // Añadir const
    std::string get_email() const; // Añadir const
    Fecha get_fecha_nacimiento() const; // Añadir const
    std::string get_contrasenia() const; // Añadir const
    ListaDoble<Cuenta*>* get_cuentas() const; // Añadir const
    void set_dni(std::string);
    void set_nombres(std::string);
    void set_apellidos(std::string);
    void set_direccion(std::string);
    void set_telefono(std::string);
    void set_email(std::string);
    void set_fecha_nacimiento(Fecha);
    void set_contrasenia(std::string);
    void agregar_cuenta(Cuenta* cuenta);
    void guardar_binario(FILE* archivo);
    void cargar_binario(FILE* archivo);
    Cuenta* buscar_cuenta(const std::string id_cuenta); // Nueva declaración
    std::string to_string() const; // Nueva declaración

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