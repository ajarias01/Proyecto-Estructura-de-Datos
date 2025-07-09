/**
 * @file validaciones.cpp
 * @brief Implementación de funciones de validación y entrada de datos del sistema bancario
 * @author Sistema Bancario
 * @date 2024
 * @details Este archivo contiene la implementación de todas las funciones de validación
 *          y entrada de datos utilizadas en el sistema bancario, incluyendo validaciones
 *          de entrada, formateo de datos, verificaciones de integridad y funciones
 *          especializadas para entrada segura de datos.
 */

#include "Validaciones.h"
using namespace std;

/**
 * @brief Limpia la línea actual en la consola y muestra un mensaje
 * @param mensaje El mensaje a mostrar después de limpiar la línea
 * @details Utiliza secuencias de escape ANSI para limpiar la línea actual
 *          y mostrar un nuevo mensaje en la misma posición
 */
void limpiar_linea(const string& mensaje) {
    cout << "\r\033[K" << mensaje;
}
/**
 * @brief Solicita al usuario ingresar solo caracteres alfabéticos
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con solo caracteres alfabéticos o "__ESC__" si se presiona ESC
 * @details Función que solo acepta letras mayúsculas y minúsculas, rechaza números y símbolos.
 *          Permite usar backspace para correción y ESC para cancelar.
 */
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
        } else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Solicita al usuario ingresar solo números para DNI/cédula
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con solo dígitos numéricos o "__ESC__" si se presiona ESC
 * @details Función especializada para entrada de documentos de identidad.
 *          Solo acepta dígitos del 0-9, permite backspace y ESC para cancelar.
 */
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
        } else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Valida y formatea una opción de entrada
 * @param opcion Referencia a la opción a validar y formatear
 * @return String con la opción formateada
 * @details Convierte el primer carácter a minúscula y el resto a mayúsculas
 *          para estandarizar el formato de las opciones ingresadas
 */
string validarOpcion(string& opcion) {
    for (int i = 1; i < opcion.length(); i++) {
        opcion[0] = tolower(opcion[0]);
        opcion[i] = toupper(opcion[i]);
    }
    return opcion;
}
/**
 * @brief Normaliza una dirección eliminando espacios extra y formateando
 * @param direccion La dirección a normalizar
 * @return String con la dirección normalizada
 * @details Elimina espacios múltiples, capitaliza la primera letra de cada palabra
 *          y formatea la dirección para mantener consistencia en el formato
 */
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
/**
 * @brief Solicita al usuario ingresar una dirección válida
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la dirección ingresada y validada
 * @details Permite ingresar direcciones con caracteres alfanuméricos, espacios, guiones,
 *          puntos, comas, símbolos especiales y caracteres con tildes. Valida que contenga
 *          al menos una letra o número y tiene un límite máximo de 100 caracteres.
 */
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
        } else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Valida y devuelve una fecha ingresada por el usuario
 * @param mensaje Mensaje a mostrar al usuario para solicitar la fecha
 * @return Fecha válida ingresada por el usuario
 * @details Solicita al usuario ingresar una fecha en formato DD/MM/AAAA,
 *          valida que la fecha sea correcta y devuelve un objeto Fecha.
 *          Permite usar backspace para corrección y ESC para cancelar.
 */
Fecha validarFecha(const string& mensaje) {
    Fecha fecha;
    char buffer[11] = {0};
    char* palabra = buffer;
    char c,dia=0,mes=0,anio=0;
    int index = 0;
    cout << mensaje;
    while (true) {
        c = getch();
        if (c == 27) { // ESC
            return Fecha(-1, -1, -1); // Indicador de ESC
        }
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
/**
 * @brief Solicita al usuario ingresar un email válido
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con el email válido ingresado
 * @details Valida el formato del email durante la entrada, asegurando que contenga
 *          solo un símbolo @, caracteres alfanuméricos, guiones, puntos y símbolos
 *          válidos para emails. Permite backspace y ESC para cancelar.
 */
string ingresar_email(const std::string& mensaje) {
    char c;
    string palabra;
    cout << mensaje;
    int arroba_count = 0;
    bool arroba_encontrado = false;
    while (true) {
        c = getch();
        if (isalnum(c) || c == '-' || c == '_' || c == '+' || (c == '.' && !palabra.empty() && !arroba_encontrado && palabra.back() != '.')) {
            // Permitir '.' solo antes del @ y no al inicio ni dos puntos seguidos
            palabra += c;
            cout << c;
        } else if (c == '@' && arroba_count == 0 && !palabra.empty() && palabra.back() != '.') {
            palabra += c;
            cout << c;
            arroba_count++;
            arroba_encontrado = true;
        } else if (c == '.' && arroba_encontrado && palabra.back() != '@' && palabra.back() != '.') {
            // Permitir '.' después del @, pero no justo después del @ ni dos puntos seguidos
            palabra += c;
            cout << c;
        } else if (c == 13 && palabra.length() >= 6 && validar_email(palabra)) { // Enter
            transform(palabra.begin(), palabra.end(), palabra.begin(), ::tolower);
            return palabra;
        } else if (c == 8 && !palabra.empty()) { // Backspace
            if (palabra.back() == '@') {
                arroba_count--;
                arroba_encontrado = false;
            }
            palabra.pop_back();
            cout << "\b \b";
        } else if (c == 13) { // Enter con entrada corta
            palabra.clear();
            return palabra;
        } else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Genera una cadena aleatoria de caracteres alfanuméricos
 * @param n Longitud de la cadena a generar
 * @return String con la cadena aleatoria generada
 * @details Utiliza caracteres alfanuméricos (A-Z, 0-9) para generar IDs únicos
 *          o códigos aleatorios. Usa generadores seguros de números aleatorios.
 */
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
/**
 * @brief Solicita al usuario ingresar solo números enteros
 * @param mensaje Mensaje a mostrar al usuario
 * @return Entero válido ingresado por el usuario
 * @details Valida que la entrada sea un número entero válido, solo acepta dígitos
 *          del 0-9. Permite backspace para corrección y convierte a entero.
 */
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
/**
 * @brief Solicita al usuario ingresar números decimales
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con el número decimal válido
 * @details Acepta números con punto decimal y valida el formato. Solo permite
 *          un punto decimal y números válidos. Permite backspace y ESC para cancelar.
 */
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
        }else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Solicita al usuario ingresar un ID alfanumérico
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con el ID válido ingresado
 * @details Acepta combinaciones de letras y números para identificadores.
 *          Convierte automáticamente a mayúsculas y elimina espacios.
 *          Límite máximo de 20 caracteres, permite backspace y ESC para cancelar.
 */
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
        }else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Valida una cédula ecuatoriana según el algoritmo oficial
 * @param cedula String con la cédula a validar
 * @return true si la cédula es válida, false en caso contrario
 * @details Implementa el algoritmo oficial de validación de cédulas ecuatorianas:
 *          - Verifica longitud de 10 dígitos
 *          - Valida código de provincia (primeros 2 dígitos)
 *          - Aplica algoritmo de módulo 10 para verificar dígito verificador
 */
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
/**
 * @brief Valida el formato de un número telefónico
 * @param telefono String con el teléfono a validar
 * @return true si el formato es válido, false en caso contrario
 * @details Valida que el número telefónico tenga exactamente 10 dígitos
 *          y comience con 0 (formato ecuatoriano). Ej: 0987654321
 */
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
/**
 * @brief Solicita al usuario ingresar una contraseña de forma segura
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la contraseña ingresada
 * @details Oculta los caracteres ingresados mostrando asteriscos para mantener
 *          la seguridad. Acepta caracteres alfanuméricos y símbolos de puntuación.
 *          Límite máximo de 20 caracteres, permite backspace y ESC para cancelar.
 */
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
        }else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Solicita contraseña de administrador de forma segura
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la contraseña de administrador
 * @details Función especializada para autenticación de administrador con entrada
 *          segura. Oculta los caracteres ingresados y tiene un límite de 25 caracteres.
 *          Permite backspace para corrección y ESC para cancelar.
 */
string ingresar_contrasenia_administrador(const string& mensaje) {
    char buffer[26] = {0}; // 25 caracteres + terminador nulo
    char* palabra = buffer;  // puntero al inicio
    int index = 0;
    cout<<mensaje;
    while (true) {
        char c = getch();
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && index < 10) {
            *(palabra + index) = c;
            printf("%c", c);
            index++;
            if (index % 5 == 0 && index < 10) {
                cout << "-";
            }
        } else if (c == 13) { // ENTER
            *(palabra + index) = '\0';  // Termina cadena manualmente
            return string(palabra);
        } else if (c == 8 && index > 0) { // BACKSPACE
            *(palabra + index) = '\0'; // borrar último carácter
            printf("\b \b");
            if (index % 5 == 0 && index < 10 ) {
                printf("\b \b"); // Borrar guion si era uno
            }
            index--;
        }else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Valida la fortaleza de una contraseña
 * @param contrasenia String con la contraseña a validar
 * @return true si cumple los requisitos, false en caso contrario
 * @details Verifica que la contraseña cumpla con los siguientes requisitos:
 *          - Longitud mínima de 8 caracteres
 *          - Al menos una letra mayúscula
 *          - Al menos una letra minúscula
 *          - Al menos un dígito
 *          - Al menos un carácter especial (signos de puntuación)
 */
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
/**
 * @brief Valida el formato de un email
 * @param email String con el email a validar
 * @return true si el formato es válido, false en caso contrario
 * @details Verifica que el email tenga formato válido:
 *          - Contiene exactamente un símbolo @
 *          - No contiene espacios
 *          - No tiene dos puntos seguidos
 *          - Tiene al menos un punto después del @
 *          - No termina en punto
 */
bool validar_email(const string& email) {
    size_t at_pos = email.find('@');
    if (at_pos == string::npos || at_pos == 0 || at_pos == email.length() - 1) {
        return false;
    }
    // No espacios
    if (email.find(' ') != string::npos) {
        return false;
    }
    // No dos puntos seguidos
    if (email.find("..") != string::npos) {
        return false;
    }
    // Debe haber al menos un punto después del @
    size_t dot_pos = email.find('.', at_pos);
    if (dot_pos == string::npos || dot_pos == at_pos + 1 || dot_pos == email.length() - 1) {
        return false;
    }
    // No debe terminar en punto
    if (email.back() == '.') {
        return false;
    }
    return true;
}
/**
 * @brief Valida si el estado civil ingresado es válido
 * @param estado_civil String con el estado civil a validar
 * @return true si es válido, false en caso contrario
 * @details Verifica que el estado civil sea uno de los valores aceptados:
 *          "Soltero", "Casado", "Divorciado", "Viudo", "Unión Libre"
 */
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
/**
 * @brief Valida si un monto monetario es válido
 * @param montoStr String con el monto a validar
 * @return true si es un monto válido, false en caso contrario
 * @details Verifica que sea un número decimal positivo válido con las siguientes reglas:
 *          - No puede ser negativo
 *          - No puede comenzar con punto
 *          - No puede tener ceros iniciales (excepto 0.xx)
 *          - Máximo un punto decimal
 *          - Máximo 2 decimales
 *          - Máximo 10 dígitos antes del punto
 *          - No puede terminar en punto
 */
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
/**
 * @brief Valida si un ID de cuenta pertenece a un cliente
 * @param cliente Puntero al cliente a verificar
 * @param id_cuenta String con el ID de cuenta a validar
 * @return true si la cuenta pertenece al cliente, false en caso contrario
 * @details Verifica que el ID de cuenta exista en las cuentas del cliente.
 *          Normaliza el ID ingresado (elimina espacios y convierte a mayúsculas)
 *          antes de realizar la comparación.
 */
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
/**
 * @brief Valida y devuelve una hora ingresada por el usuario
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la hora en formato válido
 * @details Valida formato de hora HH:MM:SS. Permite ingresar 6 dígitos
 *          que se formatean automáticamente con dos puntos. Permite backspace
 *          para corrección y ESC para cancelar.
 */
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
        }else if (c == 27) { // ESC
            return "__ESC__"; // Valor especial para ESC
        }
    }
}
/**
 * @brief Verifica si un teléfono ya está registrado en el banco
 * @param banco Referencia al objeto Banco
 * @param telefono String con el teléfono a verificar
 * @return true si el teléfono ya existe, false en caso contrario
 * @details Busca en la base de datos de clientes del banco si el teléfono
 *          ya está registrado para evitar duplicados.
 */
bool telefono_existe(Banco& banco, const std::string& telefono) {
    auto* clientes = banco.getClientes();
    bool existe = false;
    clientes->recorrer([&](Cliente* c) {
        if (c->get_telefono() == telefono) existe = true;
    });
    return existe;
}
/**
 * @brief Verifica si un email ya está registrado en el banco
 * @param banco Referencia al objeto Banco
 * @param email String con el email a verificar
 * @return true si el email ya existe, false en caso contrario
 * @details Busca en la base de datos de clientes del banco si el email
 *          ya está registrado para evitar duplicados.
 */
bool email_existe(Banco& banco, const std::string& email) {
    auto* clientes = banco.getClientes();
    bool existe = false;
    clientes->recorrer([&](Cliente* c) {
        if (c->get_email() == email) existe = true;
    });
    return existe;
}
/**
 * @brief Valida si una hora, minuto y segundo son válidos
 * @param hora Valor de hora (0-23)
 * @param minuto Valor de minuto (0-59)
 * @param segundo Valor de segundo (0-59)
 * @return true si los valores son válidos, false en caso contrario
 * @details Verifica que los valores estén en rangos válidos para tiempo:
 *          hora entre 0-23, minuto y segundo entre 0-59.
 */
bool validar_hora_minuto_segundo(int hora, int minuto, int segundo) {
    return (hora >= 0 && hora <= 23) &&
           (minuto >= 0 && minuto <= 59) &&
           (segundo >= 0 && segundo <= 59);
}
/**
 * @brief Valida un valor de búsqueda según el campo especificado
 * @param campo Tipo de campo a validar (1=DNI, 2=Nombre, 3=Apellido, 4=Teléfono)
 * @param valor String con el valor a validar
 * @return true si el valor es válido para el campo, false en caso contrario
 * @details Aplica validaciones específicas según el tipo de campo de búsqueda:
 *          - Campo 1 (DNI): Valida cédula ecuatoriana
 *          - Campo 2 (Nombre): Solo caracteres alfabéticos y espacios
 *          - Campo 3 (Apellido): Solo caracteres alfabéticos y espacios
 *          - Campo 4 (Teléfono): Valida formato de teléfono
 */
bool validar_valor_busqueda(int campo, const std::string& valor) {
    switch (campo) {
        case 1: // DNI
            return validarCedulaEcuatoriana(valor);
        case 2: // Nombre
            return !valor.empty() && std::all_of(valor.begin(), valor.end(), [](char c){ return std::isalpha(c) || std::isspace(c); });
        case 3: // Apellido
            return !valor.empty() && std::all_of(valor.begin(), valor.end(), [](char c){ return std::isalpha(c) || std::isspace(c); });
        case 4: // Teléfono
            return validar_telefono(valor);
        default:
            return false;
    }
}

