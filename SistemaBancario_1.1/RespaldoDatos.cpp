#include "RespaldoDatos.h"
#include <fstream>
#include <iomanip>
#include <ctime>
using namespace std;

void RespaldoDatos::respaldoClientes(const string& nombreArchivo, const ListaDoble<Cliente*>& clientes) {
    json j;
    clientes.recorrer([&j](Cliente* cliente) {
        json clienteJson;
        // Serializar datos básicos del cliente
        clienteJson["dni"] = cliente->get_dni();
        clienteJson["nombre"] = cliente->get_nombre();
        clienteJson["apellido"] = cliente->get_apellido();
        clienteJson["direccion"] = cliente->get_direccion();
        clienteJson["telefono"] = cliente->get_telefono();
        clienteJson["email"] = cliente->get_email();
        clienteJson["contrasenia"] = cliente->get_contrasenia();

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
        cliente->get_cuentas().recorrer([&cuentasJson](Cuenta* cuenta) {
            json cuentaJson;
            cuentaJson["id"] = cuenta->get_id_cuenta();
            cuentaJson["saldo"] = cuenta->get_saldo();
            
            // Serializar tipo específico y sus parámetros
            if (auto ahorro = dynamic_cast<Ahorro*>(cuenta)) {
                cuentaJson["tipo"] = "Ahorro";
                cuentaJson["tasa_interes"] = ahorro->get_tasa_interes();
                cuentaJson["fecha_apertura"] = {
                    {"anio", ahorro->get_fecha_apertura().get_anuario()},
                    {"mes", ahorro->get_fecha_apertura().get_mes()},
                    {"dia", ahorro->get_fecha_apertura().get_dia()}
                };
            } 
            else if (auto corriente = dynamic_cast<Corriente*>(cuenta)) {
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

ListaDoble<Cliente*> RespaldoDatos::restaurarClientes(const string& nombreArchivo) {
    ListaDoble<Cliente*> clientes;
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
        clientes.insertar_cola(cliente);
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

 void RespaldoDatos::CifradoCesar(string CifradoCesar,int numCesar){
    for (int i = 0; i < cifradoCesar.length(); i++) {
    char currentChar = cifradoCesar[i];
    if (isdigit(currentChar)) { // Verifica si es un dígito
        numCesar = currentChar - '0'; // Convierte el carácter a número
        cifradoCesar[i] = currentChar; // Mantén el carácter numérico
    } else if (currentChar == ' ') { // Si es un espacio
        cifradoCesar[i] = ' ';
    } else { // Para otros caracteres
        nuevoCesar = currentChar + index;
        if (isalpha(currentChar)) { // Asegúrate de que sea una letra
            char base = islower(currentChar) ? 'a' : 'A';
            cifradoCesar[i] = base + (nuevoCesar - base) % 26; // Aplica el cifrado César
        }
    }
    }
}
void RespaldoDatos::DecifradoCesar(string decifradoCesar,int numCesar){
for (int i = 0; i < decifradoCesar.length(); i++){
    char currentChar = decifradoCesar[i];
    if (isdigit(currentChar)) { // Verifica si es un dígito
        numCesar = currentChar - '0'; // Convierte el carácter a número
        decifradoCesar[i] = currentChar; // Mantén el carácter numérico
    } else if (currentChar == ' ') { // Si es un espacio
        decifradoCesar[i] = ' ';
    } else { // Para otros caracteres
        if (isalpha(currentChar)) { // Asegúrate de que sea una letra
            char base = islower(currentChar) ? 'a' : 'A';
            decifradoCesar[i] = base + (currentChar - base - index + 26) % 26; // Aplica el cifrado César
        }
    }
    }
}
