#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Validaciones.h"
#include "Banco.h"

void visibilidad_cursor(bool);
void mover_cursor(int x, int y);
int desplegar_menu(const char** opciones, int nopciones);
void desplegar_menu_principal(Banco& banco);
void abrir_cuenta(Banco& banco, int tipo_cuenta);
void realizar_deposito(Banco& banco);
void realizar_retiro(Banco& banco);
void consultar_movimientos(Banco& banco);
void consultar_cuentas(Banco& banco);
void calcular_intereses(Banco& banco);
void guardar_datos(Banco& banco);
void menu_cuenta(Banco& banco);
void cargar_datos(Banco& banco);

#endif