/**
 * @file RespaldoDatos.cpp
 * @brief Implementación de la clase RespaldoDatos para gestión de respaldos y recuperación de datos
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la implementación completa de la clase RespaldoDatos
 *          que proporciona funcionalidades para respaldar, restaurar y cifrar datos
 *          de clientes del sistema bancario. Incluye serialización binaria,
 *          cifrado César, y gestión de archivos con marcas de tiempo.
 */

#include "RespaldoDatos.h"
#include "Ahorro.h"
#include "Corriente.h"
#include "ListaDoble.h"
#include "Menus.h"
namespace fs = std::filesystem;
using namespace std;

/**
 * @brief Realiza respaldo binario de clientes
 * @param nombreArchivo Nombre del archivo donde guardar el respaldo
 * @param clientes Lista de clientes a respaldar
 * @details Serializa toda la información de clientes en formato binario incluyendo:
 *          - Datos personales de cada cliente
 *          - Todas las cuentas asociadas (Ahorro/Corriente)
 *          - Historial completo de movimientos
 *          - Manejo de strings con longitud variable
 */
void RespaldoDatos::respaldoClientesBinario(const std::string& nombreArchivo, const ListaDoble<Cliente*>& clientes) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo) return;

    // Contar total de clientes para escribir en el archivo
    int totalClientes = 0;
    clientes.recorrer([&](Cliente*) { totalClientes++; });
    archivo.write(reinterpret_cast<char*>(&totalClientes), sizeof(int));

    // Recorrer y serializar cada cliente
    clientes.recorrer([&](Cliente* cliente) {
        // Lambda para escribir strings con longitud variable
        auto escribir_string = [&](const string& s) {
            size_t len = s.size();
            archivo.write(reinterpret_cast<const char*>(&len), sizeof(size_t));
            archivo.write(s.c_str(), len);
        };

        // Escribir datos personales del cliente
        escribir_string(cliente->get_dni());
        escribir_string(cliente->get_nombres());
        escribir_string(cliente->get_apellidos());
        escribir_string(cliente->get_direccion());
        escribir_string(cliente->get_telefono());
        escribir_string(cliente->get_email());
        escribir_string(cliente->get_contrasenia());
        Fecha fechaNac = cliente->get_fecha_nacimiento();
        archivo.write(reinterpret_cast<const char*>(&fechaNac), sizeof(Fecha));

        // Serializar cuentas del cliente
        int totalCuentas = 0;
        cliente->get_cuentas()->recorrer([&](Cuenta*) { totalCuentas++; });
        archivo.write(reinterpret_cast<char*>(&totalCuentas), sizeof(int));

        cliente->get_cuentas()->recorrer([&](Cuenta* cuenta) {
            // Escribir información básica de la cuenta
            escribir_string(cuenta->get_tipo());
            escribir_string(cuenta->get_id_cuenta());
            double saldo = cuenta->get_saldo();
            archivo.write(reinterpret_cast<const char*>(&saldo), sizeof(double));
            Fecha fechaApertura = cuenta->get_fecha_apertura();
            archivo.write(reinterpret_cast<const char*>(&fechaApertura), sizeof(Fecha));

            // Escribir datos específicos según el tipo de cuenta
            if (cuenta->get_tipo() == "Ahorros") {
                double tasa = static_cast<Ahorro*>(cuenta)->get_tasa_interes();
                archivo.write(reinterpret_cast<const char*>(&tasa), sizeof(double));
            } else if (cuenta->get_tipo() == "Corriente") {
                double limite = static_cast<Corriente*>(cuenta)->get_limite_retiro_diario();
                archivo.write(reinterpret_cast<const char*>(&limite), sizeof(double));
            }

            // Serializar movimientos de la cuenta
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

/**
 * @brief Restaura clientes desde un archivo binario
 * @param nombreArchivo Nombre del archivo binario a restaurar
 * @return Puntero a ListaDoble con los clientes restaurados
 * @details Deserializa un archivo binario y reconstruye la estructura completa:
 *          - Crea objetos Cliente con todos sus datos
 *          - Reconstruye cuentas Ahorro y Corriente
 *          - Restaura el historial completo de movimientos
 *          - Maneja strings de longitud variable correctamente
 */
ListaDoble<Cliente*>* RespaldoDatos::restaurarClientesBinario(const std::string& nombreArchivo) {
    ListaDoble<Cliente*>* clientes = new ListaDoble<Cliente*>();
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) return clientes;

    // Leer total de clientes
    int totalClientes = 0;
    archivo.read(reinterpret_cast<char*>(&totalClientes), sizeof(int));
    
    // Lambda para leer strings con longitud variable
    auto leer_string = [&](string& s) {
        size_t len = 0;
        archivo.read(reinterpret_cast<char*>(&len), sizeof(size_t));
        s.resize(len);
        archivo.read(&s[0], len);
    };

    // Restaurar cada cliente
    for (int i = 0; i < totalClientes; ++i) {
        // Leer datos personales del cliente
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

        // Crear objeto Cliente
        Cliente* cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fechaNacimiento, contrasenia);

        // Restaurar cuentas del cliente
        int totalCuentas = 0;
        archivo.read(reinterpret_cast<char*>(&totalCuentas), sizeof(int));
        for (int j = 0; j < totalCuentas; ++j) {
            // Leer información básica de la cuenta
            string tipo, id;
            leer_string(tipo);
            leer_string(id);
            double saldo;
            archivo.read(reinterpret_cast<char*>(&saldo), sizeof(double));
            Fecha fechaApertura;
            archivo.read(reinterpret_cast<char*>(&fechaApertura), sizeof(Fecha));

            // Crear cuenta específica según el tipo
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

            // Restaurar movimientos de la cuenta
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

            // Agregar cuenta al cliente si fue creada exitosamente
            if (cuenta) cliente->agregar_cuenta(cuenta);
        }
        // Agregar cliente a la lista
        clientes->insertar_cola(cliente);
    }
    archivo.close();
    return clientes;
}

/**
 * @brief Guarda un respaldo de clientes con marca de tiempo automática
 * @param clientes Lista de clientes a respaldar
 * @details Genera un archivo de respaldo binario con nombre que incluye fecha y hora
 *          en formato YYYYMMDD_HHMMSS para identificación única. Opcionalmente
 *          puede cifrar el archivo usando cifrado César.
 */
void RespaldoDatos::guardarRespaldoClientesConFecha(const ListaDoble<Cliente*>& clientes) {
    // Generar nombre de archivo con timestamp
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    std::string nombreArchivo = "backup_clientes_" + oss.str() + ".bin";
    
    // Llamar al método de respaldo binario
    respaldoClientesBinario(nombreArchivo, clientes);
    
    // Opcional: cifrar el archivo (deshabilitado por defecto)
    //int numCesar = 3; // Puedes cambiar el número de desplazamiento
    //cifrarArchivoABaseTxt(nombreArchivo, numCesar);
}

/**
 * @brief Restaura clientes desde un archivo de texto cifrado
 * @param archivoTxt Nombre del archivo de texto cifrado
 * @param numCesar Número de desplazamiento para el cifrado César
 * @return Puntero a ListaDoble con los clientes restaurados
 * @details Proceso completo de restauración desde texto cifrado:
 *          1. Descifra el archivo de texto a formato binario
 *          2. Restaura los clientes desde el archivo binario descifrado
 */
ListaDoble<Cliente*>* RespaldoDatos::restaurarClientesDesdeTxt(const std::string& archivoTxt, int numCesar) {
    // Decifrar el .txt cifrado y obtener el nombre del archivo descifrado
    std::string archivoBin = decifrarTxtABinario(archivoTxt, numCesar);
    // Restaurar normalmente desde el binario descifrado
    return restaurarClientesBinario(archivoBin);
}

/**
 * @brief Obtiene el nombre del último archivo de respaldo disponible
 * @return String con el nombre del archivo de respaldo más reciente
 * @details Busca en el directorio actual archivos de respaldo con formato
 *          "backup_clientes_YYYYMMDD_HHMMSS.bin" y devuelve el más reciente
 *          basándose en la marca de tiempo. Si no encuentra ninguno, busca
 *          el archivo legacy "respaldo_clientes.bin".
 */
string RespaldoDatos::obtenerUltimoRespaldo() {
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

/**
 * @brief Aplica cifrado César a una cadena de texto
 * @param cifradoCesar Referencia a la cadena a cifrar (se modifica)
 * @param numCesar Número de desplazamiento para el cifrado
 * @details Aplica cifrado César solo a caracteres hexadecimales:
 *          - Caracteres 'a'-'f': se desplazan dentro del rango hexadecimal
 *          - Caracteres '0'-'9': se desplazan dentro del rango numérico
 *          - Otros caracteres permanecen sin cambios
 */
void RespaldoDatos::CifradoCesar(std::string& cifradoCesar, int numCesar) {
    for (int i = 0; i < cifradoCesar.length(); i++) {
        char c = cifradoCesar[i];
        if (c >= 'a' && c <= 'f') {
            cifradoCesar[i] = 'a' + (c - 'a' + numCesar) % 6;
        } else if (c >= '0' && c <= '9') {
            cifradoCesar[i] = '0' + (c - '0' + numCesar) % 10;
        }
    }
}

/**
 * @brief Aplica descifrado César a una cadena de texto
 * @param decifradoCesar Referencia a la cadena a descifrar (se modifica)
 * @param numCesar Número de desplazamiento para el descifrado
 * @details Aplica descifrado César (operación inversa del cifrado):
 *          - Caracteres 'a'-'f': se desplazan hacia atrás dentro del rango hexadecimal
 *          - Caracteres '0'-'9': se desplazan hacia atrás dentro del rango numérico
 *          - Usa aritmética modular para evitar desbordamientos
 */
void RespaldoDatos::DecifradoCesar(std::string& decifradoCesar, int numCesar) {
    for (int i = 0; i < decifradoCesar.length(); i++) {
        char c = decifradoCesar[i];
        if (c >= 'a' && c <= 'f') {
            decifradoCesar[i] = 'a' + (c - 'a' - numCesar + 6) % 6;
        } else if (c >= '0' && c <= '9') {
            decifradoCesar[i] = '0' + (c - '0' - numCesar + 10) % 10;
        }
    }
}

/**
 * @brief Cifra un archivo binario a formato de texto hexadecimal
 * @param archivoBin Nombre del archivo binario a cifrar
 * @param numCesar Número de desplazamiento para el cifrado César
 * @details Proceso completo de cifrado:
 *          1. Lee el archivo binario byte por byte
 *          2. Convierte cada byte a representación hexadecimal
 *          3. Aplica cifrado César a la cadena hexadecimal
 *          4. Guarda el resultado en archivo de texto con formato legible
 *          5. Extrae timestamp del nombre del archivo para naming consistente
 */
void RespaldoDatos::cifrarArchivoABaseTxt(const std::string& archivoBin, int numCesar) {
    std::ifstream in(archivoBin, std::ios::binary);
    if (!in) return;

    // Leer los datos binarios y convertirlos a representación hexadecimal
    std::ostringstream oss;
    char c;
    while (in.get(c)) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(static_cast<unsigned char>(c)));
    }
    std::string contenido = oss.str();
    in.close();

    // Aplicar cifrado César
    CifradoCesar(contenido, numCesar);

    // Extraer la marca de tiempo del nombre del archivo
    size_t pos1 = archivoBin.find("backup_clientes_");
    size_t pos2 = archivoBin.find(".bin");
    std::string timestamp;
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        timestamp = archivoBin.substr(16, pos2 - 16); // 16 es la posición después de "backup_clientes_"
    } else {
        timestamp = "error_timestamp";
    }

    // Construir el nombre del archivo cifrado
    std::string archivoTxt = "cifrado_" + timestamp + ".txt";

    // Escribir el contenido cifrado con formato legible (64 caracteres por línea)
    std::ofstream out(archivoTxt, std::ios::out | std::ios::trunc);
    if (out) {
        const size_t ancho = 64; // 64 caracteres por línea (32 bytes)
        for (size_t i = 0; i < contenido.length(); i += ancho) {
            out << contenido.substr(i, ancho) << '\n';
        }
        out.close();
    }
}

/**
 * @brief Descifra un archivo de texto a formato binario
 * @param archivoTxt Nombre del archivo de texto cifrado
 * @param numCesar Número de desplazamiento para el descifrado César
 * @return String con el nombre del archivo binario descifrado
 * @details Proceso completo de descifrado:
 *          1. Lee el archivo de texto cifrado línea por línea
 *          2. Aplica descifrado César a la cadena hexadecimal
 *          3. Convierte de hexadecimal a binario byte por byte
 *          4. Guarda el resultado en archivo binario
 *          5. Extrae timestamp para naming consistente
 */
string RespaldoDatos::decifrarTxtABinario(const std::string& archivoTxt, int numCesar) {
    std::ifstream in(archivoTxt, std::ios::in);
    if (!in) {
        throw std::runtime_error("No se pudo abrir el archivo: " + archivoTxt);
    }
    
    // Leer el contenido cifrado línea por línea
    std::string contenido;
    char c;
    while (in.get(c)) {
        contenido += c;
    }
    in.close();

    // Aplicar descifrado César con desplazamiento fijo de 3
    DecifradoCesar(contenido, 3);

    // Convertir la representación hexadecimal de vuelta a binario
    std::string timestamp = archivoTxt.substr(16, 15); // Extraer YYYYMMDD_HHMMSS
    std::string archivoDescifrado = "decifrado_" + timestamp + ".bin"; // Nuevo nombre
    std::ofstream out(archivoDescifrado, std::ios::binary | std::ios::trunc);
    if (out) {
        // Convertir cada par de caracteres hexadecimales a byte
        for (size_t i = 0; i < contenido.length(); i += 2) {
            std::string byteStr = contenido.substr(i, 2);
            unsigned int byte;
            std::stringstream ss;
            ss << std::hex << byteStr;
            ss >> byte;
            out.put(static_cast<char>(byte));
        }
        out.close();
    }
    return archivoDescifrado; // Retornar el nombre para uso en otras funciones
}

/**
 * @brief Obtiene el último archivo de texto cifrado disponible
 * @return String con el nombre del archivo de texto cifrado más reciente
 * @details Busca archivos con patrón "backup_clientes_*_cifrado.txt" y devuelve
 *          el más reciente basándose en la marca de tiempo del nombre
 */
string RespaldoDatos::obtenerUltimoTxtCifrado() {
    std::vector<std::pair<std::string, std::time_t>> backupsTxt;

    for (const auto& entry : fs::directory_iterator(".")) {
        std::string nombre = entry.path().filename().string();
        if (nombre.find("backup_clientes_") == 0 && nombre.find("_cifrado.txt") != std::string::npos) {
            std::string timestampStr = nombre.substr(16, 15); // 15 caracteres para YYYYMMDD_HHMMSS
            std::tm tm = {};
            std::istringstream ss(timestampStr);
            ss >> std::get_time(&tm, "%Y%m%d_%H%M%S");
            if (!ss.fail()) {
                std::time_t tiempo = std::mktime(&tm);
                backupsTxt.emplace_back(nombre, tiempo);
            }
        }
    }

    if (!backupsTxt.empty()) {
        std::sort(backupsTxt.begin(), backupsTxt.end(), 
            [](const auto& a, const auto& b) { return a.second > b.second; });
        return backupsTxt[0].first;
    }
    return "";
}

/**
 * @brief Lista todos los archivos de texto cifrados disponibles
 * @details Muestra en consola una lista numerada y formateada de todos los archivos
 *          de texto cifrados encontrados en el directorio, incluyendo:
 *          - Nombre del archivo
 *          - Fecha y hora de creación
 *          - Tamaño en bytes
 *          - Ordenados por fecha (más reciente primero)
 */
void RespaldoDatos::listarArchivosTxtCifrados() {
    std::vector<std::pair<std::string, std::time_t>> backupsTxt;
    int contador = 1;

    std::cout << "\n=== ARCHIVOS .TXT CIFRADOS DISPONIBLES ===" << std::endl;
    std::cout << "===========================================" << std::endl;

    for (const auto& entry : fs::directory_iterator(".")) {
        std::string nombre = entry.path().filename().string();
        if (nombre.find("cifrado_") == 0 && nombre.find(".txt") == nombre.size() - 4) {
            std::string timestampStr = nombre.substr(8, 15);
            std::tm tm = {};
            std::istringstream ss(timestampStr);
            ss >> std::get_time(&tm, "%Y%m%d_%H%M%S");
            if (!ss.fail()) {
                std::time_t tiempo = std::mktime(&tm);
                backupsTxt.emplace_back(nombre, tiempo);
            }
        }
    }

    if (backupsTxt.empty()) {
        std::cout << "No se encontraron archivos .txt cifrados." << std::endl;
        return;
    }

    std::sort(backupsTxt.begin(), backupsTxt.end(), 
        [](const auto& a, const auto& b) { return a.second > b.second; });

    for (const auto& backup : backupsTxt) {
        std::tm* tm = std::localtime(&backup.second);
        std::cout << contador << ". " << backup.first << std::endl;
        std::cout << "• Fecha: " << std::put_time(tm, "%d/%m/%Y %H:%M:%S") << "       • Tamaño: ";
        try {
            auto tamanio = fs::file_size(backup.first);
            std::cout << tamanio << " bytes" << std::endl;
        } catch (...) {
            std::cout << "N/A" << std::endl;
        }
        contador++;
    }
}

/**
 * @brief Permite seleccionar y descifrar un archivo de texto
 * @details Interfaz interactiva que:
 *          1. Lista todos los archivos de texto cifrados disponibles
 *          2. Permite al usuario seleccionar uno por número
 *          3. Descifra el archivo seleccionado usando cifrado César (clave fija: 3)
 *          4. Maneja errores y validaciones de entrada
 *          5. Proporciona feedback al usuario sobre el resultado
 */
void RespaldoDatos::seleccionarYDescifrarTxt() {
    std::vector<std::pair<std::string, std::time_t>> backupsTxt;
    for (const auto& entry : fs::directory_iterator(".")) {
        std::string nombre = entry.path().filename().string();
        if (nombre.find("cifrado_") == 0 && nombre.find(".txt") == nombre.size() - 4) {
            std::string timestampStr = nombre.substr(8, 15);
            std::tm tm = {};
            std::istringstream ss(timestampStr);
            ss >> std::get_time(&tm, "%Y%m%d_%H%M%S");
            if (!ss.fail()) {
                std::time_t tiempo = std::mktime(&tm);
                backupsTxt.emplace_back(nombre, tiempo);
            }
        }
    }

    if (backupsTxt.empty()) {
        std::cout << "\nNo se encontraron archivos .txt cifrados para descifrar." << std::endl;
        pausar_consola();
        return;
    }

    std::sort(backupsTxt.begin(), backupsTxt.end(), 
        [](const auto& a, const auto& b) { return a.second > b.second; });
    listarArchivosTxtCifrados();

    int seleccion;
    std::string numero;
    do {
        limpiar_linea("Seleccione el archivo a descifrar (1-" + std::to_string(backupsTxt.size()) + "): ");
        numero = ingresar_dni("");
        if (numero == "__ESC__") return;
        if (!numero.empty()) {
            try {
                seleccion = std::stoi(numero);
            } catch (...) {
                seleccion = 0;
            }
        }
    } while (seleccion < 1 || seleccion > static_cast<int>(backupsTxt.size()));
    cout << endl;

    std::string archivoSeleccionado = backupsTxt[seleccion - 1].first;
    // No se pide numCesar por teclado, se usa el valor fijo de 3
    int numCesar = 3;

    std::cout << "\nDescifrando archivo: " << archivoSeleccionado << std::endl;
    
    try {
        descifrarTxtSolamente(archivoSeleccionado, numCesar);
        std::cout << "Archivo descifrado exitosamente." << std::endl;
        std::cout << "Archivo descifrado guardado como: " << archivoSeleccionado << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Error al descifrar el archivo: " << e.what() << std::endl;
    }
    
    pausar_consola();
}

/**
 * @brief Descifra únicamente un archivo de texto sin restaurar datos
 * @param archivoTxt Nombre del archivo de texto a descifrar
 * @param numCesar Número de desplazamiento para el descifrado César
 * @details Función especializada que solo descifra un archivo de texto y lo guarda
 *          como binario sin cargar los datos en memoria. Útil para recuperar
 *          archivos cifrados sin necesidad de restaurar toda la estructura de datos.
 *          Maneja saltos de línea en el archivo de texto correctamente.
 */
void RespaldoDatos::descifrarTxtSolamente(const std::string& archivoTxt, int numCesar) {
    std::ifstream in(archivoTxt, std::ios::in);
    if (!in) {
        throw std::runtime_error("No se pudo abrir el archivo: " + archivoTxt);
    }

    // Leer el contenido cifrado línea por línea, ignorando saltos de línea
    std::string contenido, linea;
    while (std::getline(in, linea)) {
        contenido += linea;
    }
    in.close();

    // Aplicar descifrado César con desplazamiento fijo de 3
    DecifradoCesar(contenido, numCesar);

    // Extraer timestamp de forma segura del nombre del archivo
    std::string timestamp;
    size_t pos1 = archivoTxt.find("cifrado_");
    size_t pos2 = archivoTxt.rfind(".txt");
    if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1 + 8) {
        timestamp = archivoTxt.substr(pos1 + 8, pos2 - (pos1 + 8));
    } else {
        timestamp = "error_timestamp";
    }

    // Crear archivo binario descifrado
    std::string archivoDescifrado = "decifrado_" + timestamp + ".bin";
    std::ofstream out(archivoDescifrado, std::ios::binary | std::ios::trunc);
    if (out) {
        // Convertir cada par de caracteres hexadecimales a byte binario
        for (size_t i = 0; i < contenido.length(); i += 2) {
            std::string byteStr = contenido.substr(i, 2);
            unsigned int byte;
            std::stringstream ss;
            ss << std::hex << byteStr;
            ss >> byte;
            out.put(static_cast<char>(byte));
        }
        out.close();
    }
}