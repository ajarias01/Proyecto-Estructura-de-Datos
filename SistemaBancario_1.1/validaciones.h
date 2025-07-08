/**
 * @file validaciones.h
 * @brief Declaraciones de funciones de validación y entrada de datos del sistema bancario
 * @author Sistema Bancario
 * @date 2024
 * @details Este archivo contiene las declaraciones de todas las funciones de validación
 *          y entrada de datos utilizadas en el sistema bancario, incluyendo validaciones
 *          de entrada, formateo de datos, y verificaciones de integridad.
 */

#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include <iostream>
#include "Cliente.h"
#include "Fecha.h"
#include "Banco.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <random>
#include <conio.h>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;

/**
 * @brief Limpia la línea actual en la consola y muestra un mensaje
 * @param mensaje El mensaje a mostrar después de limpiar la línea
 * @details Utiliza secuencias de escape ANSI para limpiar la línea actual
 */
void limpiar_linea(const string& mensaje);

/**
 * @brief Valida y devuelve una fecha ingresada por el usuario
 * @param mensaje Mensaje a mostrar al usuario para solicitar la fecha
 * @return Fecha válida ingresada por el usuario
 * @details Solicita al usuario ingresar una fecha y la valida antes de devolverla
 */
Fecha validarFecha(const std::string& mensaje);

/**
 * @brief Solicita al usuario ingresar solo caracteres alfabéticos
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con solo caracteres alfabéticos o "__ESC__" si se presiona ESC
 * @details Función que solo acepta letras mayúsculas y minúsculas, rechaza números y símbolos
 */
string ingresar_alfabetico(const string& mensaje);

/**
 * @brief Solicita al usuario ingresar solo números para DNI/cédula
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con solo dígitos numéricos o "__ESC__" si se presiona ESC
 * @details Función especializada para entrada de documentos de identidad
 */
string ingresar_dni(const string& mensaje);

/**
 * @brief Valida y formatea una opción de entrada
 * @param estado Referencia a la opción a validar y formatear
 * @return String con la opción formateada
 * @details Convierte el primer carácter a minúscula y el resto a mayúsculas
 */
string validarOpcion(string& estado);

/**
 * @brief Normaliza una dirección eliminando espacios extra y formateando
 * @param direccion La dirección a normalizar
 * @return String con la dirección normalizada
 * @details Elimina espacios múltiples y formatea la dirección consistentemente
 */
string normalizarDireccion(string direccion);

/**
 * @brief Solicita al usuario ingresar una dirección válida
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la dirección ingresada y validada
 * @details Permite ingresar direcciones con caracteres alfanuméricos y símbolos comunes
 */
string ingresar_direccion(const string& mensaje);

/**
 * @brief Solicita al usuario ingresar un email válido
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con el email válido ingresado
 * @details Valida el formato del email durante la entrada
 */
string ingresar_email(const string& mensaje);

/**
 * @brief Solicita al usuario ingresar una contraseña de forma segura
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la contraseña ingresada
 * @details Oculta los caracteres ingresados mostrando asteriscos
 */
string ingresar_contrasenia(const std::string& mensaje);

/**
 * @brief Solicita al usuario ingresar números decimales
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con el número decimal válido
 * @details Acepta números con punto decimal y valida el formato
 */
string ingresar_decimales(const string& mensaje);

/**
 * @brief Solicita al usuario ingresar un ID alfanumérico
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con el ID válido ingresado
 * @details Acepta combinaciones de letras y números para identificadores
 */
string ingresar_id(const string& mensaje);

/**
 * @brief Genera una cadena aleatoria de caracteres alfanuméricos
 * @param n Longitud de la cadena a generar
 * @return String con la cadena aleatoria generada
 * @details Utiliza caracteres alfanuméricos para generar IDs únicos
 */
string generar_cadena_aleatoria(int n);

/**
 * @brief Valida y devuelve una hora ingresada por el usuario
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la hora en formato válido
 * @details Valida formato de hora HH:MM:SS
 */
string validarHora(const std::string& mensaje);

/**
 * @brief Solicita contraseña de administrador de forma segura
 * @param mensaje Mensaje a mostrar al usuario
 * @return String con la contraseña de administrador
 * @details Función especializada para autenticación de administrador
 */
string ingresar_contrasenia_administrador(const string& mensaje);

/**
 * @brief Solicita al usuario ingresar solo números enteros
 * @param mensaje Mensaje a mostrar al usuario
 * @return Entero válido ingresado por el usuario
 * @details Valida que la entrada sea un número entero válido
 */
int ingresar_enteros(const string& mensaje);

/**
 * @brief Valida una cédula ecuatoriana según el algoritmo oficial
 * @param cedula String con la cédula a validar
 * @return true si la cédula es válida, false en caso contrario
 * @details Implementa el algoritmo de validación de cédulas ecuatorianas
 */
bool validarCedulaEcuatoriana(const std::string& cedula);

/**
 * @brief Valida el formato de un número telefónico
 * @param telefono String con el teléfono a validar
 * @return true si el formato es válido, false en caso contrario
 * @details Valida longitud y formato de números telefónicos
 */
bool validar_telefono(const string& telefono);

/**
 * @brief Valida si el estado civil ingresado es válido
 * @param estado_civil String con el estado civil a validar
 * @return true si es válido, false en caso contrario
 * @details Verifica que el estado civil sea uno de los valores aceptados
 */
bool validar_estado_civil(const string& estado_civil);

/**
 * @brief Valida el formato de un email
 * @param email String con el email a validar
 * @return true si el formato es válido, false en caso contrario
 * @details Verifica que el email tenga formato válido con @ y dominio
 */
bool validar_email(const string& email);

/**
 * @brief Valida la fortaleza de una contraseña
 * @param contrasenia String con la contraseña a validar
 * @return true si cumple los requisitos, false en caso contrario
 * @details Verifica longitud mínima, mayúsculas, minúsculas, números y símbolos
 */
bool validar_contrasenia(const std::string& contrasenia);

/**
 * @brief Valida si un monto monetario es válido
 * @param montoStr String con el monto a validar
 * @return true si es un monto válido, false en caso contrario
 * @details Verifica que sea un número decimal positivo válido
 */
bool validar_monto(const string& montoStr);

/**
 * @brief Valida si un ID de cuenta pertenece a un cliente
 * @param cliente Puntero al cliente a verificar
 * @param id_cuenta String con el ID de cuenta a validar
 * @return true si la cuenta pertenece al cliente, false en caso contrario
 * @details Verifica que el ID de cuenta exista en las cuentas del cliente
 */
bool validar_id_cuenta(Cliente* cliente, const string& id_cuenta);

/**
 * @brief Verifica si un teléfono ya está registrado en el banco
 * @param banco Referencia al objeto Banco
 * @param telefono String con el teléfono a verificar
 * @return true si el teléfono ya existe, false en caso contrario
 * @details Busca en la base de datos si el teléfono ya está registrado
 */
bool telefono_existe(Banco& banco, const std::string& telefono);

/**
 * @brief Verifica si un email ya está registrado en el banco
 * @param banco Referencia al objeto Banco
 * @param email String con el email a verificar
 * @return true si el email ya existe, false en caso contrario
 * @details Busca en la base de datos si el email ya está registrado
 */
bool email_existe(Banco& banco, const std::string& email);

/**
 * @brief Valida si una hora, minuto y segundo son válidos
 * @param hora Valor de hora (0-23)
 * @param minuto Valor de minuto (0-59)
 * @param segundo Valor de segundo (0-59)
 * @return true si los valores son válidos, false en caso contrario
 * @details Verifica que los valores estén en rangos válidos para tiempo
 */
bool validar_hora_minuto_segundo(int hora, int minuto, int segundo);

/**
 * @brief Valida un valor de búsqueda según el campo especificado
 * @param campo Tipo de campo a validar
 * @param valor String con el valor a validar
 * @return true si el valor es válido para el campo, false en caso contrario
 * @details Aplica validaciones específicas según el tipo de campo de búsqueda
 */
bool validar_valor_busqueda(int campo, const std::string& valor);

#endif