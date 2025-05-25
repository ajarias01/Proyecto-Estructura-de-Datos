#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include <iostream>
#include "Fecha.h"
#include <string>
using namespace std;
Fecha validarFecha(const std::string& mensaje);
string ingresar_alfabetico(const string& mensaje);
string ingresar_dni(const string& mensaje); // Nueva función
string validarOpcion(string& estado);
string ingresar_opcion(const string& mensaje);
string normalizarDireccion(string direccion);
string ingresar_direccion(const string& mensaje);
string ingresar_email(const string& mensaje);
int ingresar_enteros(const string& mensaje);
string ingresar_decimales(const string& mensaje);
bool validarCedulaEcuatoriana(const std::string& cedula);
bool validar_telefono(const string& telefono);
bool validar_estado_civil(const string& estado_civil);
bool validar_email(const string& email);
std::string ingresar_contrasenia(const std::string& mensaje);
bool validar_contrasenia(const std::string& contrasenia);
bool validar_monto(const string& montoStr);
#endif