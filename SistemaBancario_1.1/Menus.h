#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <iostream>
#include <conio.h>
// Incluir winsock2.h antes que windows.h para evitar conflictos
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <windows.h>
#include "validaciones.h"
#include "Banco.h"
#include "Marquesina.h"
#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define ENTER 13

// Variable global para la marquesina
extern Marquesina marquesina_global;

void pausar_consola();
void visibilidad_cursor(bool visible);
void mover_cursor(int x, int y);
void inicializar_marquesina();
void detener_marquesina();
void ajustar_cursor_para_marquesina();
int seleccionar_opcion(const char *titulo, const char *opciones[], int n, int fila_inicio);
bool seleccionar_Si_No();
int desplegar_menu(const char** opciones, int nopciones);
bool validar_credenciales_administrador(const string& usuario, const string& contrasenia);
void recuperar_backup_por_fecha(Banco& banco);
void cargar_base_datos(Banco& banco);
void mostrar_arbol_binario(Banco& banco);
void descifrar_archivos_txt(Banco& banco);
void cifrar_archivos_txt(Banco& banco);
void menu_administrador(Banco& banco);
void menu_cliente(Banco& banco);
void menu_principal(Banco& banco);
void mostrar_ayuda_tecnica();
void abrir_cuenta(Banco& banco, int tipo_cuenta, int branchId, const string& sucursal);
void abrir_cuenta_sin_sucursal(Banco& banco, int tipo_cuenta);
void menu_cuenta(Banco& banco);
void menu_cuenta_aplicacion(Banco& banco);
void menu_cita_presencial(Banco& banco);
void mostrar_sucursales_cercanas();
void agendar_cita_presencial();
void realizar_deposito(Banco& banco, const std::string& dni);
void realizar_retiro(Banco& banco, const std::string& dni);
void consultar_movimientos(Banco& banco);
void consultar_cuentas(Banco& banco);
void verificar_hash(Banco& banco);

#endif