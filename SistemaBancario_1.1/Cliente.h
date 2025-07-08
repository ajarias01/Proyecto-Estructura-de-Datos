/**
 * @file Cliente.h
 * @brief Definición de la clase Cliente para la gestión de datos personales y cuentas bancarias.
 *
 * Esta clase representa a un cliente del sistema bancario, almacenando sus datos personales,
 * credenciales y la lista de cuentas asociadas. Permite la serialización y deserialización
 * de la información, así como operaciones de consulta y modificación.
 */

#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include "Cuenta.h"
#include "ListaDoble.h"
#include "Fecha.h"
#include <string>

/**
 * @class Cliente
 * @brief Representa a un cliente del sistema bancario.
 *
 * Almacena los datos personales, credenciales y la lista de cuentas asociadas.
 * Permite agregar cuentas, serializar/deserializar datos y consultar información relevante.
 */
class Cliente {
public:
    /**
     * @brief Constructor por defecto de Cliente.
     */
    Cliente();
    /**
     * @brief Constructor parametrizado de Cliente.
     * @param _dni DNI del cliente
     * @param _nombres Nombres del cliente
     * @param _apellidos Apellidos del cliente
     * @param _direccion Dirección del cliente
     * @param _telefono Teléfono del cliente
     * @param _email Email del cliente
     * @param _fecha_nacimiento Fecha de nacimiento
     * @param _contrasenia Contraseña del cliente
     */
    Cliente(std::string _dni, std::string _nombres, std::string _apellidos, std::string _direccion, 
            std::string _telefono, std::string _email, Fecha _fecha_nacimiento, std::string _contrasenia);
    /**
     * @brief Constructor de copia de Cliente.
     * @param otro Cliente a copiar
     */
    Cliente(const Cliente& otro);
    /**
     * @brief Destructor de Cliente.
     */
    ~Cliente();
    /**
     * @brief Operador de asignación para Cliente.
     * @param otro Cliente a asignar
     * @return Referencia a este objeto
     */
    Cliente& operator=(const Cliente& otro);
    /**
     * @brief Obtiene el DNI del cliente.
     * @return DNI como string
     */
    std::string get_dni() const;
    /**
     * @brief Obtiene los nombres del cliente.
     * @return Nombres como string
     */
    std::string get_nombres() const;
    /**
     * @brief Obtiene los apellidos del cliente.
     * @return Apellidos como string
     */
    std::string get_apellidos() const;
    /**
     * @brief Obtiene la dirección del cliente.
     * @return Dirección como string
     */
    std::string get_direccion() const;
    /**
     * @brief Obtiene el teléfono del cliente.
     * @return Teléfono como string
     */
    std::string get_telefono() const;
    /**
     * @brief Obtiene el email del cliente.
     * @return Email como string
     */
    std::string get_email() const;
    /**
     * @brief Obtiene la fecha de nacimiento del cliente.
     * @return Fecha de nacimiento
     */
    Fecha get_fecha_nacimiento() const;
    /**
     * @brief Obtiene la contraseña del cliente.
     * @return Contraseña como string
     */
    std::string get_contrasenia() const;
    /**
     * @brief Obtiene la lista de cuentas asociadas al cliente.
     * @return Puntero a la lista doble de cuentas
     */
    ListaDoble<Cuenta*>* get_cuentas() const;
    /**
     * @brief Establece el DNI del cliente.
     * @param _dni Nuevo DNI
     */
    void set_dni(std::string _dni);
    /**
     * @brief Establece los nombres del cliente.
     * @param _nombres Nuevos nombres
     */
    void set_nombres(std::string _nombres);
    /**
     * @brief Establece los apellidos del cliente.
     * @param _apellidos Nuevos apellidos
     */
    void set_apellidos(std::string _apellidos);
    /**
     * @brief Establece la dirección del cliente.
     * @param _direccion Nueva dirección
     */
    void set_direccion(std::string _direccion);
    /**
     * @brief Establece el teléfono del cliente.
     * @param _telefono Nuevo teléfono
     */
    void set_telefono(std::string _telefono);
    /**
     * @brief Establece el email del cliente.
     * @param _email Nuevo email
     */
    void set_email(std::string _email);
    /**
     * @brief Establece la fecha de nacimiento del cliente.
     * @param _fecha Nueva fecha de nacimiento
     */
    void set_fecha_nacimiento(Fecha _fecha);
    /**
     * @brief Establece la contraseña del cliente.
     * @param _contrasenia Nueva contraseña
     */
    void set_contrasenia(std::string _contrasenia);
    /**
     * @brief Agrega una cuenta a la lista de cuentas del cliente.
     * @param cuenta Puntero a la cuenta a agregar
     */
    void agregar_cuenta(Cuenta* cuenta);
    /**
     * @brief Guarda los datos del cliente y sus cuentas en un archivo binario.
     * @param archivo Puntero al archivo binario abierto para escritura
     */
    void guardar_binario(FILE* archivo);
    /**
     * @brief Carga los datos del cliente y sus cuentas desde un archivo binario.
     * @param archivo Puntero al archivo binario abierto para lectura
     */
    void cargar_binario(FILE* archivo);
    /**
     * @brief Busca una cuenta del cliente por su ID.
     * @param id_cuenta ID de la cuenta a buscar
     * @return Puntero a la cuenta si se encuentra, nullptr en caso contrario
     */
    Cuenta* buscar_cuenta(const std::string id_cuenta);
    /**
     * @brief Devuelve una representación en string de los datos principales del cliente.
     * @return String con DNI, nombre y apellido
     */
    std::string to_string() const;

private:
    std::string dni; ///< DNI del cliente
    std::string nombres; ///< Nombres del cliente
    std::string apellidos; ///< Apellidos del cliente
    std::string direccion; ///< Dirección del cliente
    std::string telefono; ///< Teléfono del cliente
    std::string email; ///< Email del cliente
    Fecha fecha_nacimiento; ///< Fecha de nacimiento
    std::string contrasenia; ///< Contraseña del cliente
    ListaDoble<Cuenta*>* cuentas; ///< Lista de cuentas asociadas
};

#endif