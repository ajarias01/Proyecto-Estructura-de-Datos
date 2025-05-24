#include "Cliente.h"
#include <stdexcept>
#include <functional>

Cliente::Cliente() {
    cuentas = new ListaDoble<Cuenta*>();
}

Cliente::Cliente(std::string _dni, std::string _nombre, std::string _direccion, std::string _telefono,
                std::string _email, Fecha _fecha_nacimiento ){ //std::string _estado_civil, std::string _nif) {
    try {
        if (_dni.empty()) throw std::invalid_argument("DNI no puede estar vacío");
        if (_nombre.empty()) throw std::invalid_argument("Nombre no puede estar vacío");
        dni = _dni;
        nombre = _nombre;
        direccion = _direccion;
        telefono = _telefono;
        email = _email;
        fecha_nacimiento = _fecha_nacimiento;
        //estado_civil = _estado_civil;
        //numero_identificacion_fiscal = _nif;
        cuentas = new ListaDoble<Cuenta*>();
    } catch (const std::exception& e) {
        std::cerr << "Error al crear Cliente: " << e.what() << std::endl;
        throw;
    }
}

Cliente::~Cliente() {
    if (cuentas) {
        cuentas->recorrer([](Cuenta* c) { delete c; });
        delete cuentas;
    }
}

std::string Cliente::get_dni() { return dni; }
std::string Cliente::get_nombre() { return nombre; }
std::string Cliente::get_direccion() { return direccion; }
std::string Cliente::get_telefono() { return telefono; }
std::string Cliente::get_email() { return email; }
Fecha Cliente::get_fecha_nacimiento() { return fecha_nacimiento; }
//std::string Cliente::get_estado_civil() { return estado_civil; }
//std::string Cliente::get_numero_identificacion_fiscal() { return numero_identificacion_fiscal; }
ListaDoble<Cuenta*>* Cliente::get_cuentas() { return cuentas; }

void Cliente::set_dni(std::string _dni) { dni = _dni; }
void Cliente::set_nombre(std::string _nombre) { nombre = _nombre; }
void Cliente::set_direccion(std::string _direccion) { direccion = _direccion; }
void Cliente::set_telefono(std::string _telefono) { telefono = _telefono; }
void Cliente::set_email(std::string _email) { email = _email; }
void Cliente::set_fecha_nacimiento(Fecha _fecha) { fecha_nacimiento = _fecha; }
//void Cliente::set_estado_civil(std::string _estado) { estado_civil = _estado; }
//void Cliente::set_numero_identificacion_fiscal(std::string _nif) { numero_identificacion_fiscal = _nif; }

void Cliente::agregar_cuenta(Cuenta* cuenta) {
    try {
        if (cuenta->get_id_cuenta() <= 0) throw std::invalid_argument("ID de cuenta inválido");
        cuentas->insertar_cola(cuenta);
    } catch (const std::exception& e) {
        std::cerr << "Error en agregar_cuenta: " << e.what() << std::endl;
    }
}

Cuenta* Cliente::buscar_cuenta(int id_cuenta) {
    try {
        Cuenta* resultaat = nullptr;
        cuentas->filtrar(
            [&](Cuenta* c) { return c->get_id_cuenta() == id_cuenta; },
            [&](Cuenta* c) { resultaat = c; }
        );
        if (!resultaat) throw std::runtime_error("Cuenta no encontrada");
        return resultaat;
    } catch (const std::exception& e) {
        std::cerr << "Error en buscar_cuenta: " << e.what() << std::endl;
        return nullptr;
    }
}

std::string Cliente::to_string() const {
    std::string result = "Cliente: DNI=" + dni + ", Nombre=" + nombre + ", Dirección=" + direccion +
                         ", Teléfono=" + telefono + ", Email=" + email + ", Fecha Nacimiento=" +
                         fecha_nacimiento.to_string(); //+ ", Estado Civil=" + estado_civil +
                         //", NIF=" + numero_identificacion_fiscal;
    return result;
}

void Cliente::guardar_binario(FILE* archivo) {
    try {
        fwrite(dni.c_str(), sizeof(char), dni.length() + 1, archivo);
        fwrite(nombre.c_str(), sizeof(char), nombre.length() + 1, archivo);
        fwrite(direccion.c_str(), sizeof(char), direccion.length() + 1, archivo);
        fwrite(telefono.c_str(), sizeof(char), telefono.length() + 1, archivo);
        fwrite(email.c_str(), sizeof(char), email.length() + 1, archivo);
        fwrite(&fecha_nacimiento, sizeof(Fecha), 1, archivo);
        //fwrite(estado_civil.c_str(), sizeof(char), estado_civil.length() + 1, archivo);
        //fwrite(numero_identificacion_fiscal.c_str(), sizeof(char), numero_identificacion_fiscal.length() + 1, archivo);
        int num_cuentas = 0;
        cuentas->recorrer([&](Cuenta*) { num_cuentas++; });
        fwrite(&num_cuentas, sizeof(int), 1, archivo);
        cuentas->recorrer([&](Cuenta* c) { c->guardar_binario(archivo); });
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_binario: " << e.what() << std::endl;
    }
}

void Cliente::cargar_binario(FILE* archivo) {
    try {
        char buffer[100];
        fread(buffer, sizeof(char), 100, archivo);
        dni = std::string(buffer);
        fread(buffer, sizeof(char), 100, archivo);
        nombre = std::string(buffer);
        fread(buffer, sizeof(char), 100, archivo);
        direccion = std::string(buffer);
        fread(buffer, sizeof(char), 100, archivo);
        telefono = std::string(buffer);
        fread(buffer, sizeof(char), 100, archivo);
        email = std::string(buffer);
        fread(&fecha_nacimiento, sizeof(Fecha), 1, archivo);
        //fread(buffer, sizeof(char), 100, archivo);
        //estado_civil = std::string(buffer);
        //fread(buffer, sizeof(char), 100, archivo);
        //numero_identificacion_fiscal = std::string(buffer);
        int num_cuentas;
        fread(&num_cuentas, sizeof(int), 1, archivo);
        cuentas = new ListaDoble<Cuenta*>();
        for (int i = 0; i < num_cuentas; i++) {
            char tipo;
            fread(&tipo, sizeof(char), 1, archivo);
            Cuenta* cuenta;
            if (tipo == 'A') {
                cuenta = new Ahorro();
            } else if (tipo == 'C') {
                cuenta = new Corriente();
            } else {
                throw std::runtime_error("Tipo de cuenta desconocido");
            }
            cuenta->cargar_binario(archivo);
            cuentas->insertar_cola(cuenta);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en cargar_binario: " << e.what() << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const Cliente& cliente) {
    os << cliente.to_string();
    return os;
}