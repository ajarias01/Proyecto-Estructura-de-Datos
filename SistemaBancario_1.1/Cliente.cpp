#include "Cliente.h"
#include "Ahorro.h"
#include "Corriente.h"
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <conio.h>

Cliente::Cliente() {
    cuentas = new ListaDoble<Cuenta*>();
}

Cliente::Cliente(std::string _dni, std::string _nombre, std::string _apellido, std::string _direccion, std::string _telefono,
                std::string _email, Fecha _fecha_nacimiento, std::string _contrasenia) {
    try {
        if (_dni.empty()) throw std::invalid_argument("DNI no puede estar vacío");
        if (_nombre.empty()) throw std::invalid_argument("Nombre no puede estar vacío");
        if (_apellido.empty()) throw std::invalid_argument("Apellido no puede estar vacío");
        if (_contrasenia.empty()) throw std::invalid_argument("Contraseña no puede estar vacía");
        dni = _dni;
        nombres = _nombre;
        apellidos = _apellido;
        direccion = _direccion;
        telefono = _telefono;
        email = _email;
        fecha_nacimiento = _fecha_nacimiento;
        contrasenia = _contrasenia;
        cuentas = new ListaDoble<Cuenta*>();
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Cliente: " << e.what() << std::endl;
        throw;
    }
}

Cliente::Cliente(const Cliente& otro) {
    dni = otro.dni;
    nombres = otro.nombres;
    apellidos = otro.apellidos;
    direccion = otro.direccion;
    telefono = otro.telefono;
    email = otro.email;
    fecha_nacimiento = otro.fecha_nacimiento;
    contrasenia = otro.contrasenia;
    cuentas = new ListaDoble<Cuenta*>();
    otro.cuentas->recorrer([this](Cuenta* c) {
        if (auto ahorro = dynamic_cast<Ahorro*>(c)) {
            cuentas->insertar_cola(new Ahorro(*ahorro));
        } else if (auto corriente = dynamic_cast<Corriente*>(c)) {
            cuentas->insertar_cola(new Corriente(*corriente));
        }
    });
}

Cliente& Cliente::operator=(const Cliente& otro) {
    if (this != &otro) {
        delete cuentas;
        dni = otro.dni;
        nombres = otro.nombres;
        apellidos = otro.apellidos;
        direccion = otro.direccion;
        telefono = otro.telefono;
        email = otro.email;
        fecha_nacimiento = otro.fecha_nacimiento;
        contrasenia = otro.contrasenia;
        cuentas = new ListaDoble<Cuenta*>();
        otro.cuentas->recorrer([this](Cuenta* c) {
            if (auto ahorro = dynamic_cast<Ahorro*>(c)) {
                cuentas->insertar_cola(new Ahorro(*ahorro));
            } else if (auto corriente = dynamic_cast<Corriente*>(c)) {
                cuentas->insertar_cola(new Corriente(*corriente));
            }
        });
    }
    return *this;
}

Cliente::~Cliente() {
    if (cuentas) {
        delete cuentas;
    }
}

std::string Cliente::get_dni() const { return dni; }
std::string Cliente::get_nombres() const { return nombres; }
std::string Cliente::get_apellidos() const { return apellidos; }
std::string Cliente::get_direccion() const { return direccion; }
std::string Cliente::get_telefono() const { return telefono; }
std::string Cliente::get_email() const { return email; }
Fecha Cliente::get_fecha_nacimiento() const { return fecha_nacimiento; }
std::string Cliente::get_contrasenia() const { return contrasenia; }
ListaDoble<Cuenta*>* Cliente::get_cuentas() const { return cuentas; }

void Cliente::set_dni(std::string _dni) { dni = _dni; }
void Cliente::set_nombres(std::string _nombres) { nombres = _nombres; }
void Cliente::set_apellidos(std::string _apellidos) { apellidos = _apellidos; }
void Cliente::set_direccion(std::string _direccion) { direccion = _direccion; }
void Cliente::set_telefono(std::string _telefono) { telefono = _telefono; }
void Cliente::set_email(std::string _email) { email = _email; }
void Cliente::set_fecha_nacimiento(Fecha _fecha) { fecha_nacimiento = _fecha; }
void Cliente::set_contrasenia(std::string _contrasenia) { contrasenia = _contrasenia; }

void Cliente::agregar_cuenta(Cuenta* cuenta) {
    try {
        if (cuenta->get_id_cuenta().empty()) throw std::invalid_argument("ID de cuenta inválido");
        cuentas->insertar_cola(cuenta);
    } catch (const std::exception& e) {
        std::cerr << "Error en agregar_cuenta: " << e.what() << std::endl;
    }
}

Cuenta* Cliente::buscar_cuenta(const std::string id_cuenta) {
    try {
        Cuenta* resultado = nullptr;
        std::string id_buscado = id_cuenta;
        // Elimina espacios y convierte a mayúsculas
        id_buscado.erase(0, id_buscado.find_first_not_of(" \t\n\r\f\v"));
        id_buscado.erase(id_buscado.find_last_not_of(" \t\n\r\f\v") + 1);
        std::transform(id_buscado.begin(), id_buscado.end(), id_buscado.begin(), ::toupper);

        cuentas->filtrar(
            [&](Cuenta* c) {
                std::string id_actual = c->get_id_cuenta();
                id_actual.erase(0, id_actual.find_first_not_of(" \t\n\r\f\v"));
                id_actual.erase(id_actual.find_last_not_of(" \t\n\r\f\v") + 1);
                std::transform(id_actual.begin(), id_actual.end(), id_actual.begin(), ::toupper);
                return id_actual == id_buscado;
            },
            [&](Cuenta* c) { resultado = c; }
        );
        if (!resultado) throw std::runtime_error("Cuenta no encontrada");
        return resultado;
    } catch (const std::exception& e) {
        std::cerr << "Error en buscar_cuenta: " << e.what() << std::endl;
        return nullptr;
    }
}

std::string Cliente::to_string() const {
    std::string result = "Cliente: DNI=" + dni + ", Nombre=" + nombres + ", Apellido=" + apellidos +
                         ", Dirección=" + direccion + ", Teléfono=" + telefono + ", Email=" + email +
                         ", Fecha Nacimiento=" + fecha_nacimiento.to_string();
    return result;
}

void Cliente::guardar_binario(FILE* archivo) {
    try {
        if (!archivo) throw std::runtime_error("Archivo no válido para escritura");
        // Escribir longitud de las cadenas seguido de las cadenas
        size_t len;
        len = dni.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(dni.c_str(), sizeof(char), len + 1, archivo);
        len = nombres.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(nombres.c_str(), sizeof(char), len + 1, archivo);
        len = apellidos.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(apellidos.c_str(), sizeof(char), len + 1, archivo);
        len = direccion.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(direccion.c_str(), sizeof(char), len + 1, archivo);
        len = telefono.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(telefono.c_str(), sizeof(char), len + 1, archivo);
        len = email.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(email.c_str(), sizeof(char), len + 1, archivo);
        len = contrasenia.length();
        fwrite(&len, sizeof(size_t), 1, archivo);
        fwrite(contrasenia.c_str(), sizeof(char), len + 1, archivo);
        fwrite(&fecha_nacimiento, sizeof(Fecha), 1, archivo);
        // Guardar número de cuentas como int
        int num_cuentas = 0;
        cuentas->recorrer([&](Cuenta* c) { num_cuentas++; });
        fwrite(&num_cuentas, sizeof(int), 1, archivo);
        cuentas->recorrer([&](Cuenta* c) {
            char tipo = dynamic_cast<Ahorro*>(c) ? 'A' : 'C';
            fwrite(&tipo, sizeof(char), 1, archivo);
            c->guardar_binario(archivo);
        });
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_binario: " << e.what() << std::endl;
    }
}


void Cliente::cargar_binario(FILE* archivo) {
    try {
        if (!archivo) throw std::runtime_error("Archivo no válido para lectura");
        size_t len;
        char* buffer;

        // Leer DNI
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de DNI");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer DNI");
        }
        dni = std::string(buffer);
        delete[] buffer;

        // Leer nombres
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de nombres");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer nombres");
        }
        nombres = std::string(buffer);
        delete[] buffer;

        // Leer apellidos
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de apellidos");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer apellidos");
        }
        apellidos = std::string(buffer);
        delete[] buffer;

        // Leer dirección
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de dirección");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer dirección");
        }
        direccion = std::string(buffer);
        delete[] buffer;

        // Leer teléfono
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de teléfono");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer teléfono");
        }
        telefono = std::string(buffer);
        delete[] buffer;

        // Leer email
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de email");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer email");
        }
        email = std::string(buffer);
        delete[] buffer;

        // Leer contraseña
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de contraseña");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer contraseña");
        }
        contrasenia = std::string(buffer);
        delete[] buffer;

        // Leer fecha de nacimiento
        if (fread(&fecha_nacimiento, sizeof(Fecha), 1, archivo) != 1) throw std::runtime_error("Error al leer fecha de nacimiento");

        // Leer número de cuentas como int
        int num_cuentas;
        fread(&num_cuentas, sizeof(int), 1, archivo);
        for (int i = 0; i < num_cuentas; i++) {
            char tipo;
            fread(&tipo, sizeof(char), 1, archivo);
            Cuenta* cuenta = nullptr;
            if (tipo == 'A') cuenta = new Ahorro();
            else if (tipo == 'C') cuenta = new Corriente();
            else throw std::runtime_error("Tipo de cuenta desconocido");
            cuenta->cargar_binario(archivo);
            cuentas->insertar_cola(cuenta);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en cargar_binario: " << e.what() << std::endl;
        throw;
    }
}

std::ostream& operator<<(std::ostream& os, const Cliente& cliente) {
    os << cliente.to_string();
    return os;
}