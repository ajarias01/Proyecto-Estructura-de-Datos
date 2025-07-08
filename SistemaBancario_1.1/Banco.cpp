#include "Banco.h"
#include "RespaldoDatos.h"
#include "Menus.h"
#include <stdexcept>
#include <functional>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <algorithm>

Banco::Banco() {
    clientes = new ListaDoble<Cliente*>();
    datos_cargados = false; // Inicializar bandera
    appointments.resize(4);

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

void Banco::setClientes(ListaDoble<Cliente*>* nuevos_clientes) {
    clientes = nuevos_clientes;
}

void Banco::limpiar_clientes() {
    if (clientes) {
        clientes->recorrer([](Cliente* c) { delete c; });
        delete clientes;
        clientes = new ListaDoble<Cliente*>(); // Crear nueva lista vacía
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

void Banco::consultar_cuentas_cliente(std::string dni, std::string nombre, std::string apellido, int fila_actual,
                                      std::function<void(Cuenta*)> callback) {
    try {
        // Verificar que al menos un criterio de búsqueda esté presente
        if (dni.empty() && nombre.empty() && apellido.empty()) {
            throw std::invalid_argument("Debe proporcionar al menos un criterio de búsqueda (DNI, nombre o apellido)");
        }

        bool encontrado = false;
        clientes->filtrar(
            [&](Cliente* c) {
                bool dni_match = !dni.empty() && c->get_dni() == dni;
                bool nombre_match = !nombre.empty();
                if (nombre_match) {
                    std::string nombre_c = c->get_nombres();
                    std::transform(nombre_c.begin(), nombre_c.end(), nombre_c.begin(), ::tolower);
                    std::string nombre_search = nombre;
                    std::transform(nombre_search.begin(), nombre_search.end(), nombre_search.begin(), ::tolower);
                    nombre_match = nombre_c.find(nombre_search) != std::string::npos;
                }
                bool apellido_match = !apellido.empty();
                if (apellido_match) {
                    std::string apellido_c = c->get_apellidos();
                    std::transform(apellido_c.begin(), apellido_c.end(), apellido_c.begin(), ::tolower);
                    std::string apellido_search = apellido;
                    std::transform(apellido_search.begin(), apellido_search.end(), apellido_search.begin(), ::tolower);
                    apellido_match = apellido_c.find(apellido_search) != std::string::npos;
                }
                return dni_match || nombre_match || apellido_match;
            },
            [&](Cliente* c) {
                encontrado = true;
                const int ancho_consola = 80;
                auto centrar = [&](const std::string& texto) {
                    int espacios = (ancho_consola - static_cast<int>(texto.length())) / 2;
                    if (espacios < 0) espacios = 0;
                    std::cout << std::string(espacios, ' ') << texto << std::endl;
                };

                mover_cursor(1, fila_actual++);
                centrar("╔══════════════════════════════════════════════════════════════════════╗");
                mover_cursor(1, fila_actual++);
                centrar("        === DATOS DEL CLIENTE ===        ");
                mover_cursor(1, fila_actual++);
                centrar("╚══════════════════════════════════════════════════════════════════════╝");
                mover_cursor(1, fila_actual++);
                centrar("DNI: " + c->get_dni());
                mover_cursor(1, fila_actual++);
                centrar("Nombre: " + c->get_nombres());
                mover_cursor(1, fila_actual++);
                centrar("Apellido: " + c->get_apellidos());
                mover_cursor(1, fila_actual++);
                centrar("Dirección: " + c->get_direccion());
                mover_cursor(1, fila_actual++);
                centrar("Teléfono: " + c->get_telefono());
                mover_cursor(1, fila_actual++);
                centrar("Email: " + c->get_email());
                mover_cursor(1, fila_actual++);
                centrar("Fecha de Nacimiento: " + c->get_fecha_nacimiento().to_string());
                mover_cursor(1, fila_actual++);
                centrar("──────────────────────────────────────────────────────────────────────");
                mover_cursor(1, fila_actual++);
                centrar("Cuentas asociadas:");
                mover_cursor(1, fila_actual++);

                // Definir arreglo de sucursales
                const char* sucursales[] = {
                    "Sucursal Norte - Av. 10 de Agosto y Mariana de Jesús",
                    "Sucursal Centro - Av. 12 de Octubre y Veintimilla",
                    "Sucursal Sur - Av. Morán Valverde y Rumichaca"
                };

                // Imprimir tabla con Tipo y Sucursal
                std::cout << "┌───────────────┬───────────────┬───────────────┬───────────────┐" << std::endl;
                std::cout << "│   Tipo        │   ID Cuenta   │   Saldo       │   Sucursal    │" << std::endl;
                std::cout << "├───────────────┼───────────────┼───────────────┼───────────────┤" << std::endl;
                c->get_cuentas()->recorrer([&](Cuenta* cuenta) {
                    int branchId = cuenta->get_branchId();
                    // Asignar Sucursal Sur por defecto si branchId es inválido
                    if (branchId < 1 || branchId > 3) {
                        cuenta->set_branchId(3); // Asignar Sucursal Sur (índice 2, ID 3)
                        branchId = 3;
                    }
                    std::string sucursal = sucursales[branchId - 1];
                    std::cout << "│ " << std::left << std::setw(13) << cuenta->get_tipo() << " │ "
                              << std::left << std::setw(13) << cuenta->get_id_cuenta() << " │ $"
                              << std::left << std::setw(13) << cuenta->get_saldo() << " │ "
                              << std::left << std::setw(13) << sucursal.substr(0, 13) << " │" << std::endl;
                });
                std::cout << "└───────────────┴───────────────┴───────────────┴───────────────┘" << std::endl;
                std::cout << std::endl; // Espaciado adicional
            }
        );
        if (!encontrado) {
            mover_cursor(1, fila_actual++);
            std::cout << "=== NO SE ENCONTRARON CLIENTES ===" << std::endl;
            mover_cursor(1, fila_actual++);
            std::cout << "No se encontraron clientes con DNI=" << dni;
            if (!nombre.empty()) std::cout << ", Nombre=" << nombre;
            if (!apellido.empty()) std::cout << ", Apellido=" << apellido;
            std::cout << std::endl;
        }
        
    } catch (const std::exception& e) {
        mover_cursor(1, fila_actual++);
        std::cerr << "Error al consultar cuentas: " << e.what() << std::endl;
    }
}

void Banco::consultar_movimientos_rango(std::string dni, Fecha inicio, Fecha fin) {
    try {
        if (dni.empty()) throw std::invalid_argument("El DNI no puede estar vacío");
        if (inicio > fin) throw std::invalid_argument("Rango de fechas inválido");
        Cliente* cliente = buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente con DNI " + dni + " no encontrado");
        
        std::cout << "\nMovimientos para el cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos()
                  << " (DNI: " << dni << ") entre " << inicio.to_string() << " y " << fin.to_string() << ":\n";
        cliente->get_cuentas()->recorrer([&](Cuenta* cuenta) {
            cuenta->consultar_movimientos_rango(inicio, fin);
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar movimientos: " << e.what() << std::endl;
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
        RespaldoDatos::guardarRespaldoClientesConFecha(*clientes);
        fclose(file);
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar datos: " << e.what() << std::endl;
    }
}

void Banco::guardar_datos_binario_sin_backup(std::string archivo) {
    FILE* file = fopen(archivo.c_str(), "wb");
    if (!file) throw std::runtime_error("No se pudo abrir/crear el archivo para escritura");
    int num_clientes = 0;
    clientes->recorrer([&](Cliente* c) { num_clientes++; });
    fwrite(&num_clientes, sizeof(int), 1, file);
    clientes->recorrer([&](Cliente* c) { c->guardar_binario(file); });
    fclose(file);
}

void Banco::cargar_datos_binario(std::string archivo) {
    try {
        if (datos_cargados) return;

        std::ifstream check_file(archivo, std::ios::binary);
        if (!check_file.good()) {
            std::string backupFile = RespaldoDatos::obtenerUltimoRespaldo();
            if (!backupFile.empty()) {
                ListaDoble<Cliente*>* clientesRestaurados = RespaldoDatos::restaurarClientesBinario(backupFile);
                
                if (clientes) {
                    clientes->recorrer([](Cliente* c) { delete c; });
                    delete clientes;
                }
                clientes = clientesRestaurados;
                
                guardar_datos_binario_sin_backup(archivo);
                datos_cargados = true;
                std::cout << "Datos restaurados desde el respaldo: " << backupFile << std::endl;
                getch();
                return;
            } else {
                std::ofstream create_file(archivo, std::ios::binary);
                if (!create_file.good()) throw std::runtime_error("No se pudo crear el archivo");
                create_file.close();
                std::cout << "Archivo " << archivo << " creado. No hay respaldos BIN disponibles.\n";
                datos_cargados = true;
                return;
            }
        }

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
            try {
                cliente->cargar_binario(file);
                clientes->insertar_cola(cliente);
            } catch (...) {
                delete cliente;
                fclose(file);
                throw std::runtime_error("Error al cargar cliente desde el archivo");
            }
        }
        fclose(file);
        datos_cargados = true; // Marcar datos como cargados
        std::cout << "Datos cargados exitosamente desde " << archivo << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
        throw;
    }
}

void Banco::cargar_datos_recuperados_binario(std::string archivo) {
    // NO uses la bandera datos_cargados aquí
    try {
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
            try {
                cliente->cargar_binario(file);
                clientes->insertar_cola(cliente);
            } catch (...) {
                delete cliente;
                fclose(file);
                throw std::runtime_error("Error al cargar cliente desde el archivo");
            }
        }
        fclose(file);
        std::cout << "Datos cargados exitosamente desde " << archivo << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
        throw;
    }
}
void Banco::restaurar_desde_respaldo(const std::string& nombreArchivo) {
    try {
        std::cout << "Abriendo backup: " << nombreArchivo << std::endl;
        RespaldoDatos respaldo;
        ListaDoble<Cliente*>* nuevos_clientes = respaldo.restaurarClientesBinario(nombreArchivo);
        std::cout << "Backup abierto, reemplazando clientes..." << std::endl;
        if (clientes) {
            std::cout << "Eliminando clientes actuales..." << std::endl;
            clientes->recorrer([](Cliente* c) { 
                std::cout << "Eliminando cliente..." << std::endl;
                delete c; 
            });
            delete clientes;
        }
        if (!nuevos_clientes) {
            std::cout << "La lista de nuevos clientes es nula." << std::endl;
        }
        clientes = nuevos_clientes;
        std::cout << "Clientes reemplazados correctamente." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error en restaurar_desde_respaldo: " << e.what() << std::endl;
        throw;
    }
}

std::chrono::system_clock::time_point Banco::findNextAvailableSlot(int branchId) {
    if (branchId < 1 || branchId > 3) throw std::invalid_argument("Sucursal inválida");
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&tt);
    local_tm.tm_hour = 9; // Comenzar a las 9:00 del día actual o siguiente
    local_tm.tm_min = 0;
    local_tm.tm_sec = 0;
    auto start = std::chrono::system_clock::from_time_t(mktime(&local_tm));

    // Si estamos fuera del horario laboral (17:00-9:00) o es fin de semana, mover al próximo lunes 9:00
    if (local_tm.tm_hour >= 17 || local_tm.tm_wday == 0 || local_tm.tm_wday == 6) {
        start += std::chrono::hours(24 * (8 - local_tm.tm_wday % 7)); // Avanzar al lunes
    }

    auto& branchAppointments = appointments[branchId];
    std::sort(branchAppointments.begin(), branchAppointments.end());

    auto nextSlot = start;
    while (true) {
        bool available = true;
        for (const auto& apt : branchAppointments) {
            if (std::abs(std::chrono::duration_cast<std::chrono::minutes>(apt - nextSlot).count()) < 30) {
                available = false;
                break;
            }
        }
        if (available && isWorkingHour(nextSlot)) return nextSlot;
        nextSlot += std::chrono::minutes(30);
    }
}

bool Banco::isWorkingHour(const std::chrono::system_clock::time_point& tp) {
    time_t tt = std::chrono::system_clock::to_time_t(tp);
    tm local_tm = *localtime(&tt);
    int hour = local_tm.tm_hour;
    int wday = local_tm.tm_wday; // 0 = domingo, 1 = lunes, etc.
    return (wday >= 1 && wday <= 5 && hour >= 9 && hour < 17);
}

void Banco::addAppointment(int branchId, std::chrono::system_clock::time_point time) {
    if (branchId < 1 || branchId > 3) throw std::invalid_argument("Sucursal inválida");
    appointments[branchId].push_back(time);
}

// Banco.cpp
ListaDoble<Cliente*>* Banco::getClientes() const {
    if (!clientes) {
        throw std::runtime_error("La lista de clientes no ha sido inicializada");
    }
    return clientes;
}