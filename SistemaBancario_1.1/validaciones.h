#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include <iostream>
#include "Cliente.h"
#include "Fecha.h"
#include "Banco.h"
#include <string>
using namespace std;
void limpiar_linea(const string& mensaje);
Fecha validarFecha(const std::string& mensaje);
string ingresar_alfabetico(const string& mensaje);
string ingresar_dni(const string& mensaje); // Nueva función
string validarOpcion(string& estado);
string normalizarDireccion(string direccion);
string ingresar_direccion(const string& mensaje);
string ingresar_email(const string& mensaje);
string ingresar_contrasenia(const std::string& mensaje);
string ingresar_decimales(const string& mensaje);
string ingresar_id(const string& mensaje);
string generar_cadena_aleatoria(int n);
string validarHora(const std::string& mensaje);
string ingresar_contrasenia_administrador(const string& mensaje);
int ingresar_enteros(const string& mensaje);
bool validarCedulaEcuatoriana(const std::string& cedula);
bool validar_telefono(const string& telefono);
bool validar_estado_civil(const string& estado_civil);
bool validar_email(const string& email);
bool validar_contrasenia(const std::string& contrasenia);
bool validar_monto(const string& montoStr);
bool validar_id_cuenta(Cliente* cliente, const string& id_cuenta);
bool telefono_existe(Banco& banco, const std::string& telefono);
bool email_existe(Banco& banco, const std::string& email);
bool validar_hora_minuto_segundo(int hora, int minuto, int segundo);
#endif