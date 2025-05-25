#include "Banco.h"
#include <stdexcept>
#include <functional>
#include <stdio.h>

Banco::Banco() {
    clientes = new ListaDoble<Cliente*>();
}
Banco::~Banco() {
    clientes->recorrer([](Cliente* c) { delete c; });
    delete clientes;
}

void Banco::agregar_cliente(Cliente* cliente) {
    try {
        if (buscar_cliente(cliente->get_dni())) throw std::invalid_argument("Cliente ya existe");
        clientes->insertar_cola(cliente);
    } catch (const std::exception& e) {
        std::cerr << "Error en agregar_cliente: " << e.what() << std::endl;
    }
}

Cliente* Banco::buscar_cliente(std::string dni) {
    try {
        Cliente* resultado = nullptr;
        clientes->filtrar(
            [&](Cliente* c) { return c->get_dni() == dni; }, // Usar puntero
            [&](Cliente* c) { resultado = c; } // No crear copia
        );
        return resultado;
    } catch (const std::exception& e) {
        std::cerr << "Error en buscar_cliente: " << e.what() << std::endl;
        return nullptr;
    }
}

void Banco::consultar_cuentas_cliente(std::string dni, std::string nombre) {
    try {
        std::cout << "Cuentas para DNI=" << dni << " o Nombre=" << nombre << ":\n";
        clientes->filtrar(
            [&](Cliente* c) { return c->get_dni() == dni || c->get_nombre() == nombre; },
            [&](Cliente* c) {
                std::cout << c->to_string() << "\nCuentas:\n";
                c->get_cuentas()->recorrer([](Cuenta* cuenta) { std::cout << cuenta->to_string() << std::endl; });
            }
        );
    } catch (const std::exception& e) {
        std::cerr << "Error en consultar_cuentas_cliente: " << e.what() << std::endl;
    }
}

void Banco::consultar_movimientos_rango(std::string dni, Fecha inicio, Fecha fin) {
    try {
        if (inicio > fin) throw std::invalid_argument("Rango de fechas inválido");
        Cliente* cliente = buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");
        std::cout << "Movimientos para DNI=" << dni << " entre " << inicio.to_string() << " y " << fin.to_string() << ":\n";
        cliente->get_cuentas()->recorrer([&](Cuenta* cuenta) {
            cuenta->consultar_movimientos_rango(inicio, fin);
        });
        delete cliente;
    } catch (const std::exception& e) {
        std::cerr << "Error en consultar_movimientos_rango: " << e.what() << std::endl;
    }
}

void Banco::calcular_intereses_cuenta(int id_cuenta, Fecha hasta) {
    try {
        bool encontrada = false;
        clientes->recorrer([&](Cliente* c) {
            Cuenta* cuenta = c->buscar_cuenta(id_cuenta);
            if (cuenta) {
                encontrada = true;
                double intereses = cuenta->calcular_intereses(hasta);
                std::cout << "Intereses acumulados para cuenta " << id_cuenta << " (" << cuenta->get_tipo() << ") hasta "
                          << hasta.to_string() << ": " << intereses << std::endl;
            }
        });
        if (!encontrada) throw std::runtime_error("Cuenta no encontrada");
    } catch (const std::exception& e) {
        std::cerr << "Error en calcular_intereses_cuenta: " << e.what() << std::endl;
    }
}

void Banco::guardar_datos_binario(std::string archivo) {
    try {
        FILE* file = fopen(archivo.c_str(), "wb");
        if (!file) throw std::runtime_error("No se pudo abrir el archivo");
        int num_clientes = 0;
        clientes->recorrer([&](Cliente* c) { num_clientes++; });
        fwrite(&num_clientes, sizeof(int), 1, file);
        clientes->recorrer([&](Cliente* c) { c->guardar_binario(file); });
        fclose(file);
    } catch (const std::exception& e) {
        std::cerr << "Error en guardar_datos_binario: " << e.what() << std::endl;
    }
}

void Banco::cargar_datos_binario(std::string archivo) {
    try {
        FILE* file = fopen(archivo.c_str(), "rb");
        if (!file) throw std::runtime_error("No se pudo abrir el archivo");
        int num_clientes;
        fread(&num_clientes, sizeof(int), 1, file);
        clientes = new ListaDoble<Cliente*>();
        for (int i = 0; i < num_clientes; i++) {
            Cliente* cliente = new Cliente();
            cliente->cargar_binario(file);
            clientes->insertar_cola(cliente);
        }
        fclose(file);
    } catch (const std::exception& e) {
        std::cerr << "Error en cargar_datos_binario: " << e.what() << std::endl;
    }
}