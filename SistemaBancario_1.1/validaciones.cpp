#include "Validaciones.h"
#include "Fecha.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <random>
#include <conio.h>
#include <algorithm>

using namespace std;
void limpiar_linea(const string& mensaje) {
    cout << "\r\033[K" << mensaje;
}
string ingresar_alfabetico(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            palabra += c;
            printf("%c", c);
        } else if (c == 13) { 
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            printf("\b \b");
        }
    }
}
string ingresar_dni(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        if (c>='0'&& c<='9') {
            palabra += c;
            printf("%c", c);
        } else if (c == 13) { 
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            printf("\b \b");
        }
    }
}
string validarOpcion(string& opcion) {
    for (int i = 1; i < opcion.length(); i++) {
        opcion[0] = tolower(opcion[0]);
        opcion[i] = toupper(opcion[i]);
    }
    return opcion;
}
string normalizarDireccion(string direccion) {
    bool nuevaPalabra = true;
    for (size_t i = 0; i < direccion.length(); i++) {
        if (nuevaPalabra && (isalpha(direccion[i]) || isdigit(direccion[i]))) {
            direccion[i] = toupper(direccion[i]);
            nuevaPalabra = false;
        } else {
            direccion[i] = tolower(direccion[i]);
        }
        if (direccion[i] == ' ' || direccion[i] == '-' || direccion[i] == ',' || direccion[i] == '.' || direccion[i] == '#' || direccion[i] == '/') {
            nuevaPalabra = true;
        }
    }
    return direccion;
}
string ingresar_direccion(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        // Permitir letras, números, tildes, ñ, espacios, guiones, puntos, comas, #, /
        if ((isalpha(c) || isdigit(c) || c == ' ' || c == '-' || c == ',' || c == '.' || c == '#' || c == '/' || c == 164 || c == 165 || c == 160 || c == 161 || c == 162 || c == 163 || c == 181 ||  c == 130 || c == 144 || c == 214 || c == 224 || c == 233) &&  palabra.length() < 100) { // Límite máximo de 100 caracteres
            palabra += c;
            cout << c;
        } else if (c == 13) { // Enter, mínimo 5 caracteres
            // Verificar que no sean solo espacios o símbolos
            bool tieneLetraONumero = false;
            for (char ch : palabra) {
                if (isalpha(ch) || isdigit(ch) || ch == 164 || ch == 165 || ch == 160 || ch == 161 || ch == 162 || ch == 163 || ch == 181 || ch == 130 || ch == 144 || ch == 214 || ch == 224 || ch == 233) {
                    tieneLetraONumero = true;
                    break;
                }
            }
            if (!tieneLetraONumero) {
                palabra.clear();
                return palabra;
            }
            // Verificar que no sean solo espacios
            bool soloEspacios = true;
            for (char ch : palabra) {
                if (ch != ' ') {
                    soloEspacios = false;
                    break;
                }
            }
            if (soloEspacios) {
                palabra.clear();
                return palabra;
            }
            return normalizarDireccion(palabra); // Normalizar antes de devolver
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            cout << "\b \b";
        }
    }
}
Fecha validarFecha(const string& mensaje) {
    Fecha fecha;
    char buffer[11] = {0};
    char* palabra = buffer;
    char c,dia=0,mes=0,anio=0;
    int index = 0;
    cout << mensaje;
    while (true) {
        c = getch();
        if ((c >= '0' && c <= '9') && index < 8) {
            *(palabra + index) = c;
            printf("%c", c);
            index++;
            if (index % 2 == 0 && index < 5) {
                cout << "/";
            }
        } else if (c == 13) { // Enter
            if (index != 8) {
                return Fecha(0, 0, 0); // Fecha inválida
            }
            try {
                int dia = stoi(string(buffer, 2));
                int mes = stoi(string(buffer + 2, 2));
                int anio = stoi(string(buffer + 4, 4));
                if (Fecha::es_fecha_valida(anio, mes, dia)) {
                    return Fecha(anio, mes, dia); 
                } else {
                    return Fecha(0, 0, 0); // Fecha inválida
                }
            } catch (...) {
                return Fecha(0, 0, 0);
            }
        } else if (c == 8 && index > 0) { // BACKSPACE
            *(palabra + index) = '\0'; // borrar último carácter
            printf("\b \b");
            if (index % 2 == 0 && index < 5) {
                printf("\b \b");
            }
            index--;
        }
    }
}
string ingresar_email(const std::string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    int arroba_count = 0;
    int punto_count = 0;
    bool arroba_encontrado = false;
    while (true) {
        c = getch();
        if (isalnum(c) || c == '-' || c == '_' || c == '+') {
            palabra += c;
            cout << c;
        } else if (c == '@' && arroba_count == 0) {
            palabra += c;
            cout << c;
            arroba_count++;
            arroba_encontrado = true;
        } else if (c == '.' && arroba_encontrado && punto_count == 0) {
            palabra += c;
            cout << c;
            punto_count++;
        } else if (c == 13 && palabra.length() >= 6 && validar_email(palabra)) { // Enter
            transform(palabra.begin(), palabra.end(), palabra.begin(), ::tolower);
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            if (palabra.back() == '@') {
                arroba_count--;
                arroba_encontrado = false;
            }
            if (palabra.back() == '.') {
                punto_count--;
            }
            palabra.pop_back();
            cout << "\b \b";
        } else if (c == 13) { // Enter con entrada corta
            palabra.clear();
            return palabra;
        }
    }
}
string generar_cadena_aleatoria(int n) {
    static const char alfabeto[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<> dist(0, sizeof(alfabeto) - 2);
    std::string s;
    for (int i = 0; i < n; ++i)
        s += alfabeto[dist(rng)];
    return s;
}
int ingresar_enteros(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        if ((c>='0'&& c<='9') ) {
            palabra += c;
            printf("%c", c);
        } else if (c == 13) {
            return stoi(palabra);
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            printf("\b \b");
        }
    }
}
string ingresar_decimales(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    bool punto_usado = false;
    while (true) {
        c = getch();
        if ((c >= '0' && c <= '9')) {
            palabra += c;
            printf("%c", c);
        } else if (c == '.' && !punto_usado) {
            palabra += c;
            printf("%c", c);
            punto_usado = true;
        } else if (c == 13) {
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            if (palabra.back() == '.') punto_usado = false;
            palabra.pop_back();
            printf("\b \b");
        }
    }
}
string ingresar_id(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && palabra.length() < 20) {  
            palabra += c;
            cout << (char)toupper(c);
        } else if (c == 13 && !palabra.empty()) { 
            // Elimina espacios al inicio y final y convierte a mayúsculas
            palabra.erase(0, palabra.find_first_not_of(" \t\n\r\f\v"));
            palabra.erase(palabra.find_last_not_of(" \t\n\r\f\v") + 1);
            for (char& ch : palabra) ch = toupper(ch);
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            printf("\b \b");
        }
    }
}
bool validarCedulaEcuatoriana(const string& cedula) {
    // Verificar longitud
    if (cedula.length() != 10) {
        return false;
    }
    
    // Verificar que todos los caracteres sean dígitos
    for (char c : cedula) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    
    // Verificar código de provincia (los dos primeros dígitos)
    int provincia = std::stoi(cedula.substr(0, 2));
    if (provincia < 1 || provincia > 24) {
        return false;
    }
    
    // Verificar tercer dígito (0-5 para personas naturales, 6-9 para empresas)
    int tercerDigito = cedula[2] - '0';
    if (tercerDigito > 9) {
        return false;
    }
    
    // Algoritmo de validación del dígito verificador
    int suma = 0;
    
    // Para los 9 primeros dígitos
    for (int i = 0; i < 9; i++) {
        int digito = cedula[i] - '0';
        
        // Para posiciones impares (0, 2, 4, 6, 8)
        if (i % 2 == 0) {
            digito *= 2;
            if (digito > 9) {
                digito -= 9;
            }
        }
        
        suma += digito;
    }
    
    // Cálculo del módulo
    int modulo = suma % 10;
    int verificador = (modulo == 0) ? 0 : 10 - modulo;
    
    // Comparación con el último dígito
    int ultimoDigito = cedula[9] - '0';
    
    return verificador == ultimoDigito;
}
bool validar_telefono(const string& telefono) {
    // Verificar longitud
    if (telefono.length() != 10) {
        return false;
    }
    
    // Verificar que el primer dígito sea 0
    if (telefono[0] != '0') {
        return false;
    }
    
    return true;
}
string ingresar_contrasenia(const string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    while (true) {
        c = getch();
        if ((isalnum(c) || ispunct(c)) && palabra.length() < 20) { // Límite de 20 caracteres
            palabra += c;
            printf("%c", '*'); // Mostrar asterisco
        } else if (c == 13) { // Enter, contraseña válida
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            palabra.pop_back();
            printf("\b \b");
        }
    }
}
string ingresar_contrasenia_administrador(const string& mensaje) {
    char buffer[26] = {0}; // 25 caracteres + terminador nulo
    char* palabra = buffer;  // puntero al inicio
    int index = 0;
    cout<<mensaje;
    while (true) {
        char c = getch();
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && index < 25) {
            *(palabra + index) = c;
            printf("%c", c);
            index++;
            if (index % 5 == 0 && index < 25) {
                cout << "-";
            }
        } else if (c == 13) { // ENTER
            *(palabra + index) = '\0';  // Termina cadena manualmente
            return string(palabra);
        } else if (c == 8 && index > 0) { // BACKSPACE
            *(palabra + index) = '\0'; // borrar último carácter
            printf("\b \b");
            if (index % 5 == 0 && index < 25 ) {
                printf("\b \b"); // Borrar guion si era uno
            }
            index--;
        }
    }
}
bool validar_contrasenia(const string& contrasenia) {
    // Verificar longitud mínima
    if (contrasenia.length() < 8) {
        return false;
    }

    // Verificar al menos una letra mayúscula
    bool tieneMayuscula = false;
    // Verificar al menos una letra minúscula
    bool tieneMinuscula = false;
    // Verificar al menos un dígito
    bool tieneDigito = false;
    // Verificar al menos un carácter especial
    bool tieneEspecial = false;

    for (char c : contrasenia) {
        if (isupper(c)) {
            tieneMayuscula = true;
        } else if (islower(c)) {
            tieneMinuscula = true;
        } else if (isdigit(c)) {
            tieneDigito = true;
        } else if (ispunct(c)) {
            tieneEspecial = true;
        }
    }

    return tieneMayuscula && tieneMinuscula && tieneDigito && tieneEspecial;
}
bool validar_email(const string& email) {
    size_t at_pos = email.find('@');
    size_t dot_pos = email.find('.', at_pos);
    
    if (at_pos == string::npos || dot_pos == string::npos || at_pos > dot_pos) {
        return false;
    }
    
    // Verificar que no haya espacios
    if (email.find(' ') != string::npos) {
        return false;
    }
    
    return true;
}
bool validar_estado_civil(const string& estado_civil) {
    // Lista de estados civiles válidos
    const string estados_validos[] = {"Soltero", "Casado", "Divorciado", "Viudo", "Unión Libre"};
    
    for (const string& estado : estados_validos) {
        if (estado_civil == estado) {
            return true;
        }
    }
    return false;
}
bool validar_monto(const string& montoStr) {
    if (montoStr.empty()) {
        return false;
    }

    // Rechazar números negativos
    if (montoStr[0] == '-') {
        return false;
    }

    // Rechazar puntos iniciales (ej: ".12")
    if (montoStr[0] == '.') {
        return false;
    }

    // Validar ceros iniciales (ej: "012" es inválido, "0.12" es válido)
    if (montoStr.size() > 1 && montoStr[0] == '0' && montoStr[1] != '.') {
        return false;
    }

    int puntoCount = 0;
    int digitosAntesPunto = 0;
    int digitosDespuesPunto = 0;
    bool tienePunto = false;

    for (size_t i = 0; i < montoStr.size(); ++i) {
        char c = montoStr[i];
        if (c == '.') {
            puntoCount++;
            // Más de un punto
            if (puntoCount > 1) return false; 
            tienePunto = true;
            continue;
        }

        // Caracteres no numéricos
        if (!isdigit(c)) return false; 

        if (tienePunto) {
            digitosDespuesPunto++;
            // Más de 2 decimales
            if (digitosDespuesPunto > 2) return false; 
        } else {
            digitosAntesPunto++;
            // Más de 10 dígitos antes del punto
            if (digitosAntesPunto > 10) return false; 
        }
    }

    // Validar que no termine en punto (ej: "12.")
    if (montoStr.back() == '.') return false;

    // Validar que si hay punto, tenga al menos 1 decimal
    if (tienePunto && digitosDespuesPunto == 0) return false;

    // Validar máximo 10 dígitos en total
    if (digitosAntesPunto + digitosDespuesPunto > 10) return false;

    return true;
}
bool validar_id_cuenta(Cliente* cliente, const string& id_cuenta) {
    if (id_cuenta.empty() || cliente == nullptr) return false;

    // Normaliza el id_cuenta ingresado
    string id_buscado = id_cuenta;
    id_buscado.erase(0, id_buscado.find_first_not_of(" \t\n\r\f\v"));
    id_buscado.erase(id_buscado.find_last_not_of(" \t\n\r\f\v") + 1);
    for (char& ch : id_buscado) ch = toupper(ch);

    Cuenta* resultado = nullptr;
    cliente->get_cuentas()->recorrer([&](Cuenta* cuenta) {
        string id_actual = cuenta->get_id_cuenta();
        id_actual.erase(0, id_actual.find_first_not_of(" \t\n\r\f\v"));
        id_actual.erase(id_actual.find_last_not_of(" \t\n\r\f\v") + 1);
        for (char& ch : id_actual) ch = toupper(ch);
        if (id_actual == id_buscado) {
            resultado = cuenta;
        }
    });
    return resultado != nullptr;
}
string validarHora(const string& mensaje) {
    char buffer[7] = {0};
    char* palabra = buffer;
    char c;
    int index = 0;
    cout << mensaje;
    while (true) {
        c = getch();
        if ((c >= '0' && c <= '9') && index < 6) {
            *(palabra + index) = c;
            printf("%c", c);
            index++;
            if (index %2==0&& index < 6) {
                cout << ":";
            }
        } else if (c == 13) { // Enter
            return palabra;
        } else if (c == 8 && index > 0) { // BACKSPACE
            *(palabra + index) = '\0';
            printf("\b \b");
            if (index %2==0 && index < 6) {
                printf("\b \b");
            }
            index--;
        }
    }
}
bool telefono_existe(Banco& banco, const std::string& telefono) {
    auto* clientes = banco.getClientes();
    bool existe = false;
    clientes->recorrer([&](Cliente* c) {
        if (c->get_telefono() == telefono) existe = true;
    });
    return existe;
}
bool email_existe(Banco& banco, const std::string& email) {
    auto* clientes = banco.getClientes();
    bool existe = false;
    clientes->recorrer([&](Cliente* c) {
        if (c->get_email() == email) existe = true;
    });
    return existe;
}
bool validar_hora_minuto_segundo(int hora, int minuto, int segundo) {
    return (hora >= 0 && hora <= 23) &&
           (minuto >= 0 && minuto <= 59) &&
           (segundo >= 0 && segundo <= 59);
}

