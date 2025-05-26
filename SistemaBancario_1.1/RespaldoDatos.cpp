#include "RespaldoDatos.h"
#include "Ahorro.h"
#include "Corriente.h"
#include "ListaDoble.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <vector>
namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

void RespaldoDatos::respaldoClientes(const string& nombreArchivo, const ListaDoble<Cliente*>& clientes) {
    json j;
    clientes.recorrer([&j](Cliente* cliente) {
        json clienteJson;
        // Serializar datos básicos del cliente
        clienteJson["dni"] = cliente->get_dni();
        clienteJson["nombre"] = cliente->get_nombres();
        clienteJson["apellido"] = cliente->get_apellidos();
        clienteJson["direccion"] = cliente->get_direccion();
        clienteJson["telefono"] = cliente->get_telefono();
        clienteJson["email"] = cliente->get_email();
        clienteJson["contrasenia"] = cliente->get_contrasenia();
        std::string contraseniaCifrada = cliente->get_contrasenia();
        CifradoCesar(contraseniaCifrada, 3); // Usa el desplazamiento que prefieras
        clienteJson["Cifrado Cesar contrasenia"] = contraseniaCifrada;

        // Serializar fecha de nacimiento
        Fecha fechaNacimiento = cliente->get_fecha_nacimiento();
        clienteJson["fecha_nacimiento"] = {
            {"anio", fechaNacimiento.get_anuario()},
            {"mes", fechaNacimiento.get_mes()},
            {"dia", fechaNacimiento.get_dia()},
            {"hora", fechaNacimiento.get_hora()},
            {"minutos", fechaNacimiento.get_minutos()},
            {"segundos", fechaNacimiento.get_segundos()}
        };

        // Serializar cuentas con todos sus parámetros
        json cuentasJson;
        cliente->get_cuentas()->recorrer([&cuentasJson](Cuenta* cuenta) { 
            json cuentaJson;
            cuentaJson["id"] = cuenta->get_id_cuenta();
            cuentaJson["saldo"] = cuenta->get_saldo();
            
            // Serializar tipo específico y sus parámetros
            if (Ahorro* ahorro = dynamic_cast<Ahorro*>(cuenta)) {
                cuentaJson["tipo"] = "Ahorro";
                cuentaJson["tasa_interes"] = ahorro->get_tasa_interes();
                cuentaJson["fecha_apertura"] = {
                    {"anio", ahorro->get_fecha_apertura().get_anuario()},
                    {"mes", ahorro->get_fecha_apertura().get_mes()},
                    {"dia", ahorro->get_fecha_apertura().get_dia()}
                };
            } 
            else if (Corriente* corriente = dynamic_cast<Corriente*>(cuenta)) {
                cuentaJson["tipo"] = "Corriente";
                cuentaJson["limite_retiro"] = corriente->get_limite_retiro_diario();
                cuentaJson["fecha_apertura"] = {
                    {"anio", corriente->get_fecha_apertura().get_anuario()},
                    {"mes", corriente->get_fecha_apertura().get_mes()},
                    {"dia", corriente->get_fecha_apertura().get_dia()}
                };
            }
            cuentasJson.push_back(cuentaJson);
        });
        
        clienteJson["cuentas"] = cuentasJson;
        j.push_back(clienteJson);
    });

    // Guardar en archivo con formato bonito
    std::ofstream archivo(nombreArchivo);
    archivo << j.dump(4);
    archivo.close();
}

ListaDoble<Cliente*>* RespaldoDatos::restaurarClientes(const std::string& nombreArchivo) {
    ListaDoble<Cliente*>* clientes = new ListaDoble<Cliente*>();
    std::ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir archivo: " << nombreArchivo << std::endl;
        return clientes;
    }

    json j;
    archivo >> j;

    for (const auto& item : j) {
        // Reconstruir fecha de nacimiento
        Fecha fechaNacimiento(
            item["fecha_nacimiento"]["anio"],
            item["fecha_nacimiento"]["mes"],
            item["fecha_nacimiento"]["dia"],
            item["fecha_nacimiento"]["hora"],
            item["fecha_nacimiento"]["minutos"],
            item["fecha_nacimiento"]["segundos"]
        );

        // Crear cliente
        
        Cliente* cliente = new Cliente(
            item["dni"].get<std::string>(),
            item["nombre"].get<std::string>(),
            item["apellido"].get<std::string>(),
            item["direccion"].get<std::string>(),
            item["telefono"].get<std::string>(),
            item["email"].get<std::string>(),
            fechaNacimiento,
            item["contrasenia"].get<std::string>()
            
        );

        // Restaurar cuentas con todos sus parámetros
        for (const auto& cuentaItem : item["cuentas"]) {
            Cuenta* cuenta = nullptr;
            std::string tipo = cuentaItem["tipo"];
            
            // Reconstruir fecha de apertura
            Fecha fechaApertura(
                cuentaItem["fecha_apertura"]["anio"],
                cuentaItem["fecha_apertura"]["mes"],
                cuentaItem["fecha_apertura"]["dia"]
            );

            if (tipo == "Ahorro") {
                cuenta = new Ahorro(
                    cuentaItem["id"].get<std::string>(),
                    cuentaItem["saldo"].get<double>(),
                    fechaApertura,
                    cuentaItem.value("tasa_interes", 5.0)  // Valor por defecto si no existe
                );
            }
            else if (tipo == "Corriente") {
                cuenta = new Corriente(
                    cuentaItem["id"].get<std::string>(),
                    cuentaItem["saldo"].get<double>(),
                    fechaApertura,
                    cuentaItem.value("limite_retiro", 1000.0)  // Valor por defecto si no existe
                );
            }
            
            if (cuenta) {
                cliente->agregar_cuenta(cuenta);
            }
        }
        clientes->insertar_cola(cliente);
    }
    
    archivo.close();
    return clientes;
}

void RespaldoDatos::guardarRespaldoClientesConFecha(const ListaDoble<Cliente*>& clientes) {
    // Generar nombre de archivo con timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    std::string nombreArchivo = "backup_clientes_" + oss.str() + ".json";
    
    // Llamar al método de respaldo
    respaldoClientes(nombreArchivo, clientes);
}
std::string RespaldoDatos::obtenerUltimoRespaldo() {
    std::vector<std::pair<std::string, std::time_t>> backups;

    for (const auto& entry : fs::directory_iterator(".")) {
        std::string nombre = entry.path().filename().string();
        if (nombre.find("backup_clientes_") == 0 && nombre.substr(nombre.size() - 5) == ".json") {
            // ¡Corregir aquí!
            std::string timestampStr = nombre.substr(16, nombre.size() - 21);
            
            std::tm tm = {};
            std::istringstream ss(timestampStr);
            ss >> std::get_time(&tm, "%Y%m%d_%H%M%S");
            if (ss.fail()) continue;
            std::time_t tiempo = std::mktime(&tm);
            
            backups.emplace_back(nombre, tiempo);
        }
    }

    if (!backups.empty()) {
        std::sort(backups.begin(), backups.end(), 
            [](const auto& a, const auto& b) { return a.second > b.second; });
        return backups[0].first;
    }
    if (fs::exists("respaldo_clientes.json")) {
        return "respaldo_clientes.json";
    }
    return "";
}
void RespaldoDatos::CifradoCesar(std::string& cifradoCesar, int numCesar) {
    for (int i = 0; i < cifradoCesar.length(); i++) {
        char currentChar = cifradoCesar[i];
        if (isdigit(currentChar)) {
            numCesar = currentChar - '0';
        } else if (isalpha(currentChar)) {
            char base = islower(currentChar) ? 'a' : 'A';
            cifradoCesar[i] = base + (currentChar - base + numCesar) % 26;
        }
    }
}

void RespaldoDatos::DecifradoCesar(std::string& decifradoCesar, int numCesar) {
    for (int i = 0; i < decifradoCesar.length(); i++) {
        char currentChar = decifradoCesar[i];
        if (isdigit(currentChar)) {
            numCesar = currentChar - '0';
        } else if (isalpha(currentChar)) {
            char base = islower(currentChar) ? 'a' : 'A';
            decifradoCesar[i] = base + (currentChar - base - numCesar + 26) % 26;
        }
    }
}