/**
 * @file Menus.h
 * @brief Declaración de funciones de menú y utilidades de interfaz para el sistema bancario.
 * @author Sistema Bancario
 * @date 2024
 * @details Este archivo contiene las declaraciones de todas las funciones relacionadas con
 * la interfaz de usuario, menús, operaciones bancarias y utilidades de consola.
 */

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

/**
 * @def TECLA_ARRIBA
 * @brief Código ASCII para la tecla de flecha hacia arriba.
 */
#define TECLA_ARRIBA 72

/**
 * @def TECLA_ABAJO
 * @brief Código ASCII para la tecla de flecha hacia abajo.
 */
#define TECLA_ABAJO 80

/**
 * @def ENTER
 * @brief Código ASCII para la tecla Enter.
 */
#define ENTER 13

/**
 * @var marquesina_global
 * @brief Variable global para controlar la marquesina del sistema.
 * @details Esta variable permite controlar la animación de la marquesina
 * que se muestra en la interfaz de usuario.
 */
extern Marquesina marquesina_global;

// ========== FUNCIONES DE UTILIDADES DE CONSOLA ==========

/**
 * @brief Pausa la consola y espera que el usuario presione Enter.
 * @details Función que pausa la ejecución del programa hasta que el usuario
 * presione la tecla Enter para continuar.
 */
void pausar_consola();

/**
 * @brief Controla la visibilidad del cursor en la consola.
 * @param visible true para mostrar el cursor, false para ocultarlo.
 * @details Utiliza la API de Windows para controlar la visibilidad del cursor
 * en la consola, útil para mejorar la apariencia de la interfaz.
 */
void visibilidad_cursor(bool visible);

/**
 * @brief Mueve el cursor a una posición específica en la consola.
 * @param x Coordenada X (columna) donde posicionar el cursor.
 * @param y Coordenada Y (fila) donde posicionar el cursor.
 * @details Utiliza la API de Windows para posicionar el cursor en
 * coordenadas específicas de la consola.
 */
void mover_cursor(int x, int y);

// ========== FUNCIONES DE MARQUESINA ==========

/**
 * @brief Inicializa la marquesina del sistema.
 * @details Inicia la animación de la marquesina con el texto del sistema bancario.
 */
void inicializar_marquesina();

/**
 * @brief Detiene la marquesina del sistema.
 * @details Pausa la animación de la marquesina para evitar interferencias
 * con otras operaciones de la interfaz.
 */
void detener_marquesina();

/**
 * @brief Ajusta el cursor para dejar espacio a la marquesina.
 * @details Posiciona el cursor en una ubicación que no interfiera
 * con la visualización de la marquesina.
 */
void ajustar_cursor_para_marquesina();

// ========== FUNCIONES DE SELECCIÓN Y NAVEGACIÓN ==========

/**
 * @brief Muestra un menú de selección navegable con teclas de dirección.
 * @param titulo Título del menú a mostrar.
 * @param opciones Array de cadenas con las opciones del menú.
 * @param n Número de opciones en el array.
 * @param fila_inicio Fila inicial donde comenzar a mostrar el menú.
 * @return Índice de la opción seleccionada (base 0).
 * @details Permite al usuario navegar por las opciones usando las teclas
 * de flecha arriba/abajo y seleccionar con Enter.
 */
int seleccionar_opcion(const char *titulo, const char *opciones[], int n, int fila_inicio);

/**
 * @brief Muestra un menú de confirmación Sí/No.
 * @return true si el usuario selecciona "SÍ", false si selecciona "NO".
 * @details Presenta al usuario una opción binaria de confirmación
 * con navegación por teclas de dirección.
 */
bool seleccionar_Si_No();

/**
 * @brief Despliega un menú con botones estilizados.
 * @param opciones Array de cadenas con las opciones del menú.
 * @param nopciones Número de opciones disponibles.
 * @return Índice de la opción seleccionada (base 0).
 * @details Muestra un menú con botones visualmente estilizados
 * y permite la navegación con teclas de dirección.
 */
int desplegar_menu(const char** opciones, int nopciones);

// ========== FUNCIONES DE AUTENTICACIÓN ==========

/**
 * @brief Valida las credenciales del administrador del sistema.
 * @param usuario Nombre de usuario ingresado.
 * @param contrasenia Contraseña ingresada.
 * @return true si las credenciales son correctas, false en caso contrario.
 * @details Verifica si las credenciales proporcionadas corresponden
 * a las credenciales de administrador predefinidas.
 */
bool validar_credenciales_administrador(const string& usuario, const string& contrasenia);

// ========== FUNCIONES DE GESTIÓN DE DATOS ==========

/**
 * @brief Recupera un backup de clientes basado en fecha y hora específicas.
 * @param banco Referencia al objeto Banco donde cargar los datos recuperados.
 * @details Permite al usuario especificar una fecha y hora para recuperar
 * un backup específico de la base de datos de clientes.
 */
void recuperar_backup_por_fecha(Banco& banco);

/**
 * @brief Carga y muestra la base de datos de clientes con opciones de ordenamiento.
 * @param banco Referencia al objeto Banco con los datos de los clientes.
 * @details Permite visualizar, ordenar y buscar en la base de datos de clientes
 * con diferentes criterios de ordenamiento y filtrado.
 */
void cargar_base_datos(Banco& banco);

/**
 * @brief Muestra la estructura del árbol binario de clientes.
 * @param banco Referencia al objeto Banco con la estructura del árbol.
 * @details Visualiza la estructura del árbol binario utilizado para
 * organizar los datos de los clientes del sistema.
 */
void mostrar_arbol_binario(Banco& banco);

// ========== FUNCIONES DE CIFRADO Y SEGURIDAD ==========

/**
 * @brief Descifra archivos de respaldo en formato TXT.
 * @param banco Referencia al objeto Banco (parámetro requerido).
 * @details Permite seleccionar y descifrar archivos de respaldo
 * que fueron cifrados previamente con cifrado César.
 */
void descifrar_archivos_txt(Banco& banco);

/**
 * @brief Cifra archivos de respaldo binarios a formato TXT.
 * @param banco Referencia al objeto Banco (parámetro requerido).
 * @details Convierte archivos de respaldo binarios a formato TXT
 * aplicando cifrado César para proteger la información.
 */
void cifrar_archivos_txt(Banco& banco);

/**
 * @brief Verifica la integridad de los datos usando algoritmos hash.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @details Calcula y compara hashes MD5 para verificar la integridad
 * de los archivos de datos del sistema.
 */
void verificar_hash(Banco& banco);

/**
 * @brief Implementa búsqueda usando tabla hash para localizar clientes.
 * @param banco Referencia al objeto Banco para acceder a los datos.
 * @details Construye una tabla hash y permite realizar búsquedas
 * eficientes de clientes basadas en diferentes campos.
 */
void buscar_con_tabla_hash(Banco& banco);

// ========== FUNCIONES DE MENÚS PRINCIPALES ==========

/**
 * @brief Muestra el menú de administrador con todas sus opciones.
 * @param banco Referencia al objeto Banco para operaciones administrativas.
 * @details Presenta el menú principal para administradores con opciones
 * de consulta, gestión de datos, cifrado y verificación de integridad.
 */
void menu_administrador(Banco& banco);

/**
 * @brief Muestra el menú de cliente con opciones de transacciones.
 * @param banco Referencia al objeto Banco para operaciones bancarias.
 * @details Presenta el menú principal para clientes con opciones
 * de depósito, retiro y generación de códigos QR.
 */
void menu_cliente(Banco& banco);

/**
 * @brief Muestra el menú principal del sistema bancario.
 * @param banco Referencia al objeto Banco principal del sistema.
 * @details Presenta el menú de inicio con opciones para crear cuenta,
 * iniciar sesión, acceso de administrador y ayuda técnica.
 */
void menu_principal(Banco& banco);

/**
 * @brief Muestra la ayuda técnica del sistema.
 * @details Abre el manual de ayuda técnica en formato CHM
 * con documentación detallada del sistema.
 */
void mostrar_ayuda_tecnica();

/**
 * @brief Muestra la documentación técnica del código (Doxygen).
 * @details Abre la documentación completa del código fuente
 * generada automáticamente con Doxygen en formato CHM.
 */
void mostrar_documentacion_doxygen();

// ========== FUNCIONES DE GESTIÓN DE CUENTAS ==========

/**
 * @brief Abre una nueva cuenta bancaria con sucursal asignada.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @param tipo_cuenta Tipo de cuenta (1=Ahorros, 2=Corriente).
 * @param branchId ID de la sucursal donde se abre la cuenta.
 * @param sucursal Nombre de la sucursal.
 * @details Proceso completo de apertura de cuenta con asignación
 * de sucursal y agendamiento de cita presencial.
 */
void abrir_cuenta(Banco& banco, int tipo_cuenta, int branchId, const string& sucursal);

/**
 * @brief Abre una nueva cuenta bancaria vía aplicación móvil.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @param tipo_cuenta Tipo de cuenta (1=Ahorros, 2=Corriente).
 * @details Proceso de apertura de cuenta completamente digital
 * sin necesidad de visita presencial a sucursal.
 */
void abrir_cuenta_sin_sucursal(Banco& banco, int tipo_cuenta);

/**
 * @brief Muestra el menú de métodos de apertura de cuentas.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @details Permite elegir entre apertura vía aplicación o
 * agendamiento de cita presencial.
 */
void menu_cuenta(Banco& banco);

/**
 * @brief Muestra el menú de apertura de cuentas vía aplicación.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @details Interfaz para seleccionar el tipo de cuenta a crear
 * usando el método de aplicación móvil.
 */
void menu_cuenta_aplicacion(Banco& banco);

/**
 * @brief Muestra el menú de citas presenciales para apertura de cuentas.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @details Interfaz para agendar citas presenciales en sucursales
 * para la apertura de nuevas cuentas bancarias.
 */
void menu_cita_presencial(Banco& banco);

/**
 * @brief Muestra las sucursales cercanas al usuario.
 * @details Utiliza el sistema de geolocalización para mostrar
 * las sucursales más cercanas a la ubicación del usuario.
 */
void mostrar_sucursales_cercanas();

/**
 * @brief Procesa el agendamiento de una cita presencial.
 * @details Utiliza el sistema de geolocalización para encontrar
 * la sucursal más cercana y agendar una cita disponible.
 */
void agendar_cita_presencial();

// ========== FUNCIONES DE TRANSACCIONES BANCARIAS ==========

/**
 * @brief Realiza un depósito en una cuenta bancaria.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @param dni DNI del cliente propietario de la cuenta.
 * @details Proceso completo de depósito con validación de datos,
 * actualización de saldo y registro de movimientos.
 */
void realizar_deposito(Banco& banco, const std::string& dni);

/**
 * @brief Realiza un retiro de una cuenta bancaria.
 * @param banco Referencia al objeto Banco para las operaciones.
 * @param dni DNI del cliente propietario de la cuenta.
 * @details Proceso completo de retiro con validación de saldo,
 * límites de retiro y registro de movimientos.
 */
void realizar_retiro(Banco& banco, const std::string& dni);

// ========== FUNCIONES DE CONSULTAS ==========

/**
 * @brief Consulta los movimientos de un cliente en un rango de fechas.
 * @param banco Referencia al objeto Banco para buscar movimientos.
 * @details Permite consultar el historial de movimientos de un cliente
 * filtrando por rango de fechas específico.
 */
void consultar_movimientos(Banco& banco);

/**
 * @brief Consulta las cuentas de un cliente por DNI, nombre o apellido.
 * @param banco Referencia al objeto Banco para buscar cuentas.
 * @details Permite buscar y visualizar las cuentas de un cliente
 * usando diferentes criterios de búsqueda.
 */
void consultar_cuentas(Banco& banco);

// ========== FUNCIONES DE BACKUP MANUAL ==========

/**
 * @brief Crea un backup manual de los datos del sistema.
 * @param banco Referencia al objeto Banco para crear el backup.
 * @details Permite al administrador crear un backup manual de todos
 * los datos del sistema con marca de tiempo.
 */
void crear_backup_manual(Banco& banco);

/**
 * @brief Verifica si existe el archivo datos.txt y maneja su recuperación.
 * @param banco Referencia al objeto Banco para cargar los datos.
 * @return true si los datos fueron cargados exitosamente, false en caso contrario.
 * @details Verifica la existencia de datos.txt y si no existe, presenta
 * un menú para seleccionar un backup para recuperar.
 */
bool verificar_y_recuperar_datos(Banco& banco);

#endif