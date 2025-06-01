#include "RespaldoDatos.h"
#include "Ahorro.h"
#include "Corriente.h"
#include "ListaDoble.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <vector>
namespace fs = std::filesystem;
using namespace std;

void RespaldoDatos::respaldoClientesBinario(const std::string& nombreArchivo, const ListaDoble<Cliente*>& clientes) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo) return;

    int totalClientes = 0;
    clientes.recorrer([&](Cliente*) { totalClientes++; });
    archivo.write(reinterpret_cast<char*>(&totalClientes), sizeof(int));

    clientes.recorrer([&](Cliente* cliente) {
        auto escribir_string = [&](const string& s) {
            size_t len = s.size();
            archivo.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
            archivo.write(s.c_str(), len);
        };

        escribir_string(cliente->get_dni());
        escribir_string(cliente->get_nombres());
        escribir_string(cliente->get_apellidos());
        escribir_string(cliente->get_direccion());
        escribir_string(cliente->get_telefono());
        escribir_string(cliente->get_email());
        escribir_string(cliente->get_contrasenia());
        Fecha fechaNac = cliente->get_fecha_nacimiento();
        archivo.write(reinterpret_cast<const char*>(&fechaNac), sizeof(Fecha));

        // Cuentas
        int totalCuentas = 0;
        cliente->get_cuentas()->recorrer([&](Cuenta*) { totalCuentas++; });
        archivo.write(reinterpret_cast<char*>(&totalCuentas), sizeof(int));

        cliente->get_cuentas()->recorrer([&](Cuenta* cuenta) {
            escribir_string(cuenta->get_tipo());
            escribir_string(cuenta->get_id_cuenta());
            double saldo = cuenta->get_saldo();
            archivo.write(reinterpret_cast<const char*>(&saldo), sizeof(double));
            Fecha fechaApertura = cuenta->get_fecha_apertura();
            archivo.write(reinterpret_cast<const char*>(&fechaApertura), sizeof(Fecha));

            if (cuenta->get_tipo() == "Ahorros") {
                double tasa = static_cast<Ahorro*>(cuenta)->get_tasa_interes();
                archivo.write(reinterpret_cast<const char*>(&tasa), sizeof(double));
            } else if (cuenta->get_tipo() == "Corriente") {
                double limite = static_cast<Corriente*>(cuenta)->get_limite_retiro_diario();
                archivo.write(reinterpret_cast<const char*>(&limite), sizeof(double));
            }

            // Movimientos
            int totalMovimientos = 0;
            cuenta->get_movimientos()->recorrer([&](Movimiento) { totalMovimientos++; });
            archivo.write(reinterpret_cast<char*>(&totalMovimientos), sizeof(int));
            cuenta->get_movimientos()->recorrer([&](Movimiento m) {
                std::string tipo = m.get_tipo();
                size_t len = tipo.size();
                archivo.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
                archivo.write(tipo.c_str(), len);

                double monto = m.get_monto();
                archivo.write(reinterpret_cast<const char*>(&monto), sizeof(double));
                Fecha fecha = m.get_fecha();
                archivo.write(reinterpret_cast<const char*>(&fecha), sizeof(Fecha));
                double saldo_post = m.get_saldo_post_movimiento();
                archivo.write(reinterpret_cast<const char*>(&saldo_post), sizeof(double));
            });
        });
    });

    archivo.close();
}
ListaDoble<Cliente*>* RespaldoDatos::restaurarClientesBinario(const std::string& nombreArchivo) {
    ListaDoble<Cliente*>* clientes = new ListaDoble<Cliente*>();
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) return clientes;

    int totalClientes = 0;
    archivo.read(reinterpret_cast<char*>(&totalClientes), sizeof(int));
    
    auto leer_string = [&](string& s) {
        size_t len = 0;
        archivo.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        s.resize(len);
        archivo.read(&s[0], len);
    };

    for (int i = 0; i < totalClientes; ++i) {
        string dni, nombre, apellido, direccion, telefono, email, contrasenia;
        leer_string(dni);
        leer_string(nombre);
        leer_string(apellido);
        leer_string(direccion);
        leer_string(telefono);
        leer_string(email);
        leer_string(contrasenia);

        Fecha fechaNacimiento;
        archivo.read(reinterpret_cast<char*>(&fechaNacimiento), sizeof(Fecha));

        Cliente* cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fechaNacimiento, contrasenia);

        int totalCuentas = 0;
        archivo.read(reinterpret_cast<char*>(&totalCuentas), sizeof(int));
        for (int j = 0; j < totalCuentas; ++j) {
            string tipo, id;
            leer_string(tipo);
            leer_string(id);
            double saldo;
            archivo.read(reinterpret_cast<char*>(&saldo), sizeof(double));
            Fecha fechaApertura;
            archivo.read(reinterpret_cast<char*>(&fechaApertura), sizeof(Fecha));

            Cuenta* cuenta = nullptr;
            if (tipo == "Ahorros") {
                double tasa;
                archivo.read(reinterpret_cast<char*>(&tasa), sizeof(double));
                cuenta = new Ahorro(id, saldo, fechaApertura, tasa);
            } else if (tipo == "Corriente") {
                double limite;
                archivo.read(reinterpret_cast<char*>(&limite), sizeof(double));
                cuenta = new Corriente(id, saldo, fechaApertura, limite);
            }

            // Restaurar movimientos
            int totalMovimientos = 0;
            archivo.read(reinterpret_cast<char*>(&totalMovimientos), sizeof(int));
            for (int k = 0; k < totalMovimientos; ++k) {
                string tipoMov;
                leer_string(tipoMov);
                double monto;
                archivo.read(reinterpret_cast<char*>(&monto), sizeof(double));
                Fecha fecha;
                archivo.read(reinterpret_cast<char*>(&fecha), sizeof(Fecha));
                double saldo_post;
                archivo.read(reinterpret_cast<char*>(&saldo_post), sizeof(double));

                Movimiento mov(tipoMov, monto, fecha, saldo_post);
                cuenta->get_movimientos()->insertar_cola(mov);
            }

            if (cuenta) cliente->agregar_cuenta(cuenta);
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
    std::string nombreArchivo = "backup_clientes_" + oss.str() + ".bak";
    
    // Llamar al método de respaldo binario
    respaldoClientesBinario(nombreArchivo, clientes);
    int numCesar = 3; // Puedes cambiar el número de desplazamiento
    cifrarArchivoABaseTxt(nombreArchivo, numCesar);
}
ListaDoble<Cliente*>* RespaldoDatos::restaurarClientesDesdeTxt(const std::string& archivoTxt, int numCesar) {
    // Decifrar el .txt cifrado y crear un .bin temporal
    decifrarTxtABinario(archivoTxt, numCesar);

    // El nombre del binario decifrado
    std::string archivoBin = archivoTxt.substr(0, archivoTxt.find("_cifrado.txt")) + "_decifrado.bin";

    // Restaurar normalmente desde el binario decifrado
    return restaurarClientesBinario(archivoBin);
}

std::string RespaldoDatos::obtenerUltimoRespaldo() {
    std::vector<std::pair<std::string, std::time_t>> backups;

    for (const auto& entry : fs::directory_iterator(".")) {
        std::string nombre = entry.path().filename().string();
        if (nombre.find("backup_clientes_") == 0 && nombre.substr(nombre.size() - 4) == ".bin") {
            std::string timestampStr = nombre.substr(16, nombre.size() - 20);
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
    if (fs::exists("respaldo_clientes.bin")) {
        return "respaldo_clientes.bin";
    }
    return "";
}

// Las funciones de cifrado y decifrado pueden quedarse igual:
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

void RespaldoDatos::cifrarArchivoABaseTxt(const std::string& archivoBin, int numCesar) {
    std::ifstream in(archivoBin, std::ios::binary);
    if (!in) return;
    std::string contenido((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // Cifrar el contenido
    CifradoCesar(contenido, numCesar);

    // Guardar como .txt
    std::string archivoTxt = archivoBin.substr(0, archivoBin.find_last_of('.')) + "_cifrado.txt";
    std::ofstream out(archivoTxt, std::ios::out | std::ios::trunc);
    out << contenido;
    out.close();
}
void RespaldoDatos::decifrarTxtABinario(const std::string& archivoTxt, int numCesar) {
    std::ifstream in(archivoTxt, std::ios::in);
    if (!in) return;
    std::string contenido((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // Decifrar el contenido
    DecifradoCesar(contenido, numCesar);

    // Guardar como .bin
    std::string archivoBin = archivoTxt.substr(0, archivoTxt.find("_cifrado.txt")) + "_decifrado.bin";
    std::ofstream out(archivoBin, std::ios::binary | std::ios::trunc);
    out.write(contenido.c_str(), contenido.size());
    out.close();
}