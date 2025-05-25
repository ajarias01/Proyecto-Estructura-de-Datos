#include "Banco.h"
#include <stdexcept>
#include <functional>
#include <stdio.h>
#include <fstream>

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
        std::cerr << "Error al agregar cliente: " << e.what() << std::endl;
    }
}

Cliente* Banco::buscar_cliente(std::string dni) {
    try {
        Cliente* resultado = nullptr;
        clientes->filtrar(
            [&](Cliente* c) { return c->get_dni() == dni; },
            [&](Cliente* c) { resultado = c; }
        );
        return resultado;
    } catch (const std::exception& e) {
        std::cerr << "Error al buscar cliente: " << e.what() << std::endl;
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
        std::cerr << "Error al consultar cuentas: " << e.what() << std::endl;
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
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar movimientos: " << e.what() << std::endl;
    }
}

void Banco::calcular_intereses_cuenta(std::string id_cuenta, Fecha hasta) {
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
        std::cerr << "Error al calcular intereses: " << e.what() << std::endl;
    }
}

void Banco::guardar_datos_binario(std::string archivo) {
    try {
        FILE* file = fopen(archivo.c_str(), "wb");
        if (!file) throw std::runtime_error("No se pudo abrir/crear el archivo para escritura");
        int num_clientes = 0;
        clientes->recorrer([&](Cliente* c) { num_clientes++; });
        fwrite(&num_clientes, sizeof(int), 1, file);
        clientes->recorrer([&](Cliente* c) { c->guardar_binario(file); });
        fclose(file);
        std::cout << "Datos guardados exitosamente en " << archivo << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar datos: " << e.what() << std::endl;
    }
}

void Banco::cargar_datos_binario(std::string archivo) {
    try {
        std::ifstream check_file(archivo, std::ios::binary);
        if (!check_file.good()) {
            std::ofstream create_file(archivo, std::ios::binary);
            if (!create_file.good()) {
                throw std::runtime_error("No se pudo crear el archivo");
            }
            create_file.close();
            std::cout << "Archivo " << archivo << " no existía, se creó uno nuevo.\n";
            return;
        }
        check_file.close();

        FILE* file = fopen(archivo.c_str(), "rb");
        if (!file) throw std::runtime_error("No se pudo abrir el archivo para lectura");
        
        int num_clientes;
        if (fread(&num_clientes, sizeof(int), 1, file) != 1) {
            fclose(file);
            throw std::runtime_error("Error al leer el número de clientes");
        }
        
        if (clientes) {
            clientes->recorrer([](Cliente* c) { delete c; });
            delete clientes;
        }
        
        clientes = new ListaDoble<Cliente*>();
        for (int i = 0; i < num_clientes; i++) {
            Cliente* cliente = new Cliente();
            cliente->cargar_binario(file);
            clientes->insertar_cola(cliente);
        }
        fclose(file);
        std::cout << "Datos cargados exitosamente desde " << archivo << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
    }
}