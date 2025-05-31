#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Validaciones.h"
#include "Banco.h"
#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define ENTER 13

void visibilidad_cursor(bool);
void mover_cursor(int x, int y);
int desplegar_menu(const char** opciones, int nopciones);
void menu_principal(Banco& banco);
void menu_cliente(Banco& banco);
void menu_cuenta(Banco& banco);
void menu_administrador(Banco& banco);
void abrir_cuenta(Banco& banco, int tipo_cuenta);
void pausar_consola();
void realizar_deposito(Banco& banco, const std::string& dni);
void realizar_retiro(Banco& banco, const std::string& dni);
void consultar_movimientos(Banco& banco);
void consultar_cuentas(Banco& banco);
void recuperar_backup_por_fecha(Banco& banco);
void mostrar_ayuda_tecnica();
void cargar_base_datos(Banco& banco);
bool validar_credenciales_administrador(const std::string& usuario, const std::string& contrasenia);

#endif