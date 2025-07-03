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

Cliente::Cliente(std::string _dni, std::string _nombres, std::string _apellidos, std::string _direccion,
                std::string _telefono, std::string _email, Fecha _fecha_nacimiento, std::string _contrasenia)
                : dni(_dni), nombres(_nombres), apellidos(_apellidos), direccion(_direccion),
                  telefono(_telefono), email(_email), fecha_nacimiento(_fecha_nacimiento), contrasenia(_contrasenia) {
    cuentas = new ListaDoble<Cuenta*>();
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
    if (otro.cuentas) {
        otro.cuentas->recorrer([this](Cuenta* c) {
            if (Ahorro* ahorro = dynamic_cast<Ahorro*>(c)) {
                this->agregar_cuenta(new Ahorro(*ahorro)); // Copia de Ahorro
            } else if (Corriente* corriente = dynamic_cast<Corriente*>(c)) {
                this->agregar_cuenta(new Corriente(*corriente)); // Copia de Corriente
            }
        });
    }
}

Cliente::~Cliente() {
    if (cuentas) {
        cuentas->recorrer([](Cuenta* c) { delete c; });
        delete cuentas;
    }
}

Cliente& Cliente::operator=(const Cliente& otro) {
    if (this != &otro) {
        dni = otro.dni;
        nombres = otro.nombres;
        apellidos = otro.apellidos;
        direccion = otro.direccion;
        telefono = otro.telefono;
        email = otro.email;
        fecha_nacimiento = otro.fecha_nacimiento;
        contrasenia = otro.contrasenia;
        if (cuentas) {
            cuentas->recorrer([](Cuenta* c) { delete c; });
            delete cuentas;
        }
        cuentas = new ListaDoble<Cuenta*>();
        if (otro.cuentas) {
            otro.cuentas->recorrer([this](Cuenta* c) {
                if (Ahorro* ahorro = dynamic_cast<Ahorro*>(c)) {
                    this->agregar_cuenta(new Ahorro(*ahorro)); // Copia de Ahorro
                } else if (Corriente* corriente = dynamic_cast<Corriente*>(c)) {
                    this->agregar_cuenta(new Corriente(*corriente)); // Copia de Corriente
                }
            });
        }
    }
    return *this;
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
    if (cuentas) cuentas->insertar_cola(cuenta);
}

void Cliente::guardar_binario(FILE* archivo) {
    try {
        if (!archivo) throw std::runtime_error("Archivo no válido para escritura");

        size_t len = dni.length();
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

        int num_cuentas = 0;
        cuentas->recorrer([&](Cuenta* c) { num_cuentas++; });
        fwrite(&num_cuentas, sizeof(int), 1, archivo);

        cuentas->recorrer([&](Cuenta* cuenta) {
            int accountType = (dynamic_cast<Ahorro*>(cuenta) != nullptr) ? 1 : 2; // 1 para Ahorro, 2 para Corriente
            fwrite(&accountType, sizeof(int), 1, archivo);
            std::string id_cuenta = cuenta->get_id_cuenta();
            size_t id_len = id_cuenta.length();
            fwrite(&id_len, sizeof(size_t), 1, archivo);
            fwrite(id_cuenta.c_str(), sizeof(char), id_len + 1, archivo);
            double saldo = cuenta->get_saldo();
            fwrite(&saldo, sizeof(double), 1, archivo);
            Fecha fecha_apertura = cuenta->get_fecha_apertura();
            fwrite(&fecha_apertura, sizeof(Fecha), 1, archivo);
            int branchId = cuenta->get_branchId();
            fwrite(&branchId, sizeof(int), 1, archivo);
            time_t tt = std::chrono::system_clock::to_time_t(cuenta->get_appointmentTime());
            fwrite(&tt, sizeof(time_t), 1, archivo);

            if (accountType == 1) {
                Ahorro* ahorro = dynamic_cast<Ahorro*>(cuenta);
                if (ahorro) {
                    double tasa_interes = ahorro->get_tasa_interes();
                    fwrite(&tasa_interes, sizeof(double), 1, archivo);
                }
            } else if (accountType == 2) {
                Corriente* corriente = dynamic_cast<Corriente*>(cuenta);
                if (corriente) {
                    double limite_retiro_diario = corriente->get_limite_retiro_diario();
                    fwrite(&limite_retiro_diario, sizeof(double), 1, archivo);
                }
            }

            int num_movimientos = 0;
            cuenta->get_movimientos()->recorrer([&](Movimiento) { num_movimientos++; });
            fwrite(&num_movimientos, sizeof(int), 1, archivo);
            cuenta->get_movimientos()->recorrer([&](Movimiento m) { m.guardar_binario(archivo); });
        });
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_binario: " << e.what() << std::endl;
    }
}

void Cliente::cargar_binario(FILE* archivo) {
    try {
        if (!archivo) throw std::runtime_error("Archivo no válido para lectura");

        size_t len;
        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de dni");
        char* buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer dni");
        }
        dni = std::string(buffer);
        delete[] buffer;

        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de nombres");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer nombres");
        }
        nombres = std::string(buffer);
        delete[] buffer;

        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de apellidos");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer apellidos");
        }
        apellidos = std::string(buffer);
        delete[] buffer;

        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de direccion");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer direccion");
        }
        direccion = std::string(buffer);
        delete[] buffer;

        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de telefono");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer telefono");
        }
        telefono = std::string(buffer);
        delete[] buffer;

        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de email");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer email");
        }
        email = std::string(buffer);
        delete[] buffer;

        if (fread(&len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de contrasenia");
        buffer = new char[len + 1];
        if (fread(buffer, sizeof(char), len + 1, archivo) != len + 1) {
            delete[] buffer;
            throw std::runtime_error("Error al leer contrasenia");
        }
        contrasenia = std::string(buffer);
        delete[] buffer;

        if (fread(&fecha_nacimiento, sizeof(Fecha), 1, archivo) != 1) throw std::runtime_error("Error al leer fecha_nacimiento");

        int num_cuentas;
        if (fread(&num_cuentas, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer número de cuentas");

        if (cuentas) {
            cuentas->recorrer([](Cuenta* c) { delete c; });
            delete cuentas;
        }
        cuentas = new ListaDoble<Cuenta*>();

        for (int i = 0; i < num_cuentas; i++) {
            Cuenta* cuenta = nullptr;
            int accountType;
            if (fread(&accountType, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer tipo de cuenta");
            std::string id_cuenta;
            size_t id_len;
            if (fread(&id_len, sizeof(size_t), 1, archivo) != 1) throw std::runtime_error("Error al leer longitud de id_cuenta");
            char* id_buffer = new char[id_len + 1];
            if (fread(id_buffer, sizeof(char), id_len + 1, archivo) != id_len + 1) {
                delete[] id_buffer;
                throw std::runtime_error("Error al leer id_cuenta");
            }
            id_cuenta = std::string(id_buffer);
            delete[] id_buffer;

            double saldo;
            if (fread(&saldo, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer saldo");

            Fecha fecha_apertura;
            if (fread(&fecha_apertura, sizeof(Fecha), 1, archivo) != 1) throw std::runtime_error("Error al leer fecha_apertura");

            int branchId;
            if (fread(&branchId, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer branchId");

            time_t tt;
            if (fread(&tt, sizeof(time_t), 1, archivo) != 1) throw std::runtime_error("Error al leer appointmentTime");
            auto appointmentTime = std::chrono::system_clock::from_time_t(tt);

            if (accountType == 1) {
                double tasa_interes;
                if (fread(&tasa_interes, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer tasa_interes");
                cuenta = new Ahorro(id_cuenta, saldo, fecha_apertura, tasa_interes);
            } else if (accountType == 2) {
                double limite_retiro_diario;
                if (fread(&limite_retiro_diario, sizeof(double), 1, archivo) != 1) throw std::runtime_error("Error al leer limite_retiro_diario");
                cuenta = new Corriente(id_cuenta, saldo, fecha_apertura, limite_retiro_diario);
            } else {
                throw std::runtime_error("Tipo de cuenta desconocido");
            }

            cuenta->set_branchId(branchId);
            cuenta->set_appointmentTime(appointmentTime);

            int num_movimientos;
            if (fread(&num_movimientos, sizeof(int), 1, archivo) != 1) throw std::runtime_error("Error al leer número de movimientos");
            for (int j = 0; j < num_movimientos; j++) {
                Movimiento movimiento;
                movimiento.cargar_binario(archivo);
                cuenta->get_movimientos()->insertar_cola(movimiento);
            }
            cuentas->insertar_cola(cuenta);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en cargar_binario: " << e.what() << std::endl;
        throw;
    }
}

Cuenta* Cliente::buscar_cuenta(const std::string id_cuenta) {
    if (!cuentas) return nullptr;
    Cuenta* resultado = nullptr;
    cuentas->filtrar(
        [&](Cuenta* c) { return c->get_id_cuenta() == id_cuenta; },
        [&](Cuenta* c) { resultado = c; }
    );
    return resultado;
}

std::string Cliente::to_string() const {
    return "DNI: " + dni + ", Nombre: " + nombres + ", Apellido: " + apellidos;
}