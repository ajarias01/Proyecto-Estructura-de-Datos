/**
 * @file Menus.cpp
 * @brief Implementación de los menús y utilidades de interacción para el sistema bancario.
 *
 * Este archivo contiene las funciones de menú principal, menú de cliente, menú de administrador,
 * operaciones de cuentas, depósitos, retiros, consultas, cifrado/descifrado, verificación de hash,
 * manejo de la marquesina y utilidades de interfaz para la aplicación bancaria.
 */

#include "Menus.h"
#include "Fecha.h"
#include "Ahorro.h"
#include "Corriente.h"
#include "validaciones.h"
#include "RespaldoDatos.h"
#include "GestorClientes.h"
#include "Marquesina.h"
#include "hash.h"
#include "ArbolBinario.h"
#include "Ubicacion.h"
#include "QRCodeGenerator.h"
#include "pdf_generator.h"
#include <stdexcept>
#include <conio.h>
#include <random>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <chrono>
#include <thread>
#include <cmath>
#include <filesystem>
#include <thread>
#include <openssl/md5.h>
#include <ctime>

using namespace std;

//// Variable global del sistema de geolocalización
SimpleGeolocationSystem geoSystem;

/**
 * @brief Pausa la consola y espera que el usuario presione Enter para continuar.
 */
void pausar_consola()
{
    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

/**
 * @brief Cambia la visibilidad del cursor en la consola de Windows.
 * @param visible true para mostrar el cursor, false para ocultarlo.
 */
void visibilidad_cursor(bool visible)
{
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = visible;
    SetConsoleCursorInfo(manejo_consola, &info);
}

/**
 * @brief Mueve el cursor de la consola a la posición especificada.
 * @param x Coordenada X (columna) donde posicionar el cursor.
 * @param y Coordenada Y (fila) donde posicionar el cursor.
 */
void mover_cursor(int x, int y)
{
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(manejo_consola, pos);
}

int seleccionar_opcion(const char *titulo, const char *opciones[], int n, int fila_inicio)
{
    system("chcp 65001 > nul");
    int opcionSeleccionada = 1;
/**
 * @brief Muestra un menú de opciones y permite la selección usando las teclas de dirección.
 * @param titulo Título del menú a mostrar.
 * @param opciones Array de cadenas con las opciones del menú.
 * @param n Número de opciones en el array.
 * @param fila_inicio Fila inicial donde comenzar a mostrar el menú.
 * @return Índice de la opción seleccionada (base 0).
 */
    int tecla;

    int fila_ajustada = std::max(fila_inicio, 3);

    mover_cursor(1, fila_ajustada);
    mover_cursor(2, fila_ajustada);
    cout << titulo;

    for (int e = 0; e < n; e++)
    {
        mover_cursor(10, fila_ajustada + 2 + e);
        mover_cursor(2, fila_ajustada + 2 + e);
        cout << string(80, ' ');
        mover_cursor(2, fila_ajustada + 2 + e);
        if (e + 1 == opcionSeleccionada)
        {
            cout << " ➤ " << e + 1 << " " << opciones[e];
        }
        else
        {
            cout << "   " << e + 1 << " " << opciones[e];
        }
    }

    do
    {
        tecla = getch();
        switch (tecla)
        {
        case TECLA_ARRIBA:
            mover_cursor(10, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << "    " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];
            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << string(80, ' ');
            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << "   " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];

            opcionSeleccionada--;
            if (opcionSeleccionada < 1)
                opcionSeleccionada = n;

            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << string(80, ' ');
            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << " ➤ " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];
            break;

        case TECLA_ABAJO:
            mover_cursor(10, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << "    " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];

            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << string(80, ' ');
            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << "   " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];

            opcionSeleccionada++;
            if (opcionSeleccionada > n)
                opcionSeleccionada = 1;

            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << string(80, ' ');
            mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
            cout << " ➤ " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];
            break;
        }
    } while (tecla != ENTER);

    return opcionSeleccionada - 1; // Ajuste para devolver índice 0-based
}

bool seleccionar_Si_No()
{
/**
 * @brief Muestra un menú de confirmación Sí/No.
 * @return true si el usuario selecciona "SÍ", false si selecciona "NO".
 */

    const int NUM_OPCIONES = 2;
    const char *OPCIONES[NUM_OPCIONES] = {
        "SÍ",
        "NO"};

    system("cls");
    ajustar_cursor_para_marquesina();
    int opcion = seleccionar_opcion("===== CONFIRMAR OPERACIÓN =====", OPCIONES, NUM_OPCIONES, 4);

    return opcion == 0; // Retorna true para "SÍ" (índice 0)
}

/**
 * @brief Despliega un menú con botones estilizados y permite la navegación.
 * @param opciones Array de cadenas con las opciones del menú.
 * @param nopciones Número de opciones disponibles.
 * @return Índice de la opción seleccionada (base 0).
 */
int desplegar_menu(const char** opciones, int nopciones)
{
    const short int ANCHO_BOTON = 35;
    visibilidad_cursor(false);

    char tecla;
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    short int cursor_x = 0, cursor_y = 3, color_act = 0, opcion = 1;
    int colores[2] = {15, 191};

    auto imprimir_opciones = [&]()
    {
        auto imprimir_boton = [ANCHO_BOTON](const char *opcion)
        {
            short int caracteres_en_opcion = 0;
            while (opcion[caracteres_en_opcion] != '\0')
            {
                caracteres_en_opcion++;
            }
            int espacios_faltantes = ANCHO_BOTON - caracteres_en_opcion;
            for (short int i = 0; i < ANCHO_BOTON; i++)
            {
                printf("%c", ' ');
            }
            printf("\n");
            for (short int i = 0; i < espacios_faltantes / 2; i++)
            {
                printf("%c", ' ');
            }
            printf("%s", opcion);
            if (espacios_faltantes % 2 == 0)
            {
                for (short int i = 0; i < espacios_faltantes / 2; i++)
                {
                    printf("%c", ' ');
                }
            }
            else
            {
                for (short int i = 0; i < (espacios_faltantes / 2) + 1; i++)
                {
                    printf("%c", ' ');
                }
            }
            printf("\n");
            for (short int i = 0; i < ANCHO_BOTON; i++)
            {
                printf("%c", ' ');
            }
        };

        for (int i = 0; i < nopciones; i++)
        {
            color_act = (i * 3) + 3 == cursor_y ? 1 : 0;
            mover_cursor(cursor_x, (i * 3) + 3);
            SetConsoleTextAttribute(manejo_consola, colores[color_act]);
            imprimir_boton(opciones[i]);
        }
        SetConsoleTextAttribute(manejo_consola, 15);
        cout.flush();
    };

    imprimir_opciones();
    while ((tecla = getch()) != 13)
    {
        if (tecla == 72 && opcion > 1)
            cursor_y -= 3;
        else if (tecla == 80 && opcion < nopciones)
            cursor_y += 3;
        imprimir_opciones();
        mover_cursor(cursor_x, cursor_y);
        opcion = ((cursor_y - 3) / 3) + 1;
    }

    return opcion - 1; // Ajuste para devolver índice 0-based
}


/**
 * @brief Valida las credenciales del administrador del sistema.
 * @param usuario Nombre de usuario ingresado.
 * @param contrasenia Contraseña ingresada.
 * @return true si las credenciales son correctas, false en caso contrario.
 */
bool validar_credenciales_administrador(const string& usuario, const string& contrasenia) {
    const string USUARIO_ADMIN = "admin";
    const string CONTRASENIA_ADMIN = "1234567890";
    return usuario == USUARIO_ADMIN && contrasenia == CONTRASENIA_ADMIN;
}

/**
 * @brief Recupera un backup de clientes seleccionado de una lista de backups disponibles.
 * @param banco Referencia al objeto Banco donde cargar los datos recuperados.
 */
void recuperar_backup_por_fecha(Banco& banco)
{
    system("cls");
    ajustar_cursor_para_marquesina();
    detener_marquesina(); // Detener la marquesina para evitar interferencias
    visibilidad_cursor(true);
    
    try
    {
        std::vector<std::string> archivos_backup;
        std::vector<std::time_t> fechas_backup;
        
        cout << "\n===========================================" << endl;
        cout << "===   RECUPERAR BACKUP DE CLIENTES   ===" << endl;
        cout << "===========================================" << endl;
        
        // Buscar archivos de backup
        for (const auto &entry : std::filesystem::directory_iterator("."))
        {
            string nombre = entry.path().filename().string();
            if (nombre.find("backup_clientes_") == 0 && nombre.substr(nombre.size() - 4) == ".bin")
            {
                archivos_backup.push_back(nombre);
                
                // Extraer timestamp del nombre del archivo
                size_t pos1 = nombre.find("backup_clientes_");
                size_t pos2 = nombre.find(".bin");
                if (pos1 != string::npos && pos2 != string::npos)
                {
                    string timestamp = nombre.substr(16, pos2 - 16);
                    if (timestamp.length() == 15) // YYYYMMDD_HHMMSS
                    {
                        struct tm tm = {};
                        tm.tm_year = stoi(timestamp.substr(0, 4)) - 1900;
                        tm.tm_mon = stoi(timestamp.substr(4, 2)) - 1;
                        tm.tm_mday = stoi(timestamp.substr(6, 2));
                        tm.tm_hour = stoi(timestamp.substr(9, 2));
                        tm.tm_min = stoi(timestamp.substr(11, 2));
                        tm.tm_sec = stoi(timestamp.substr(13, 2));
                        fechas_backup.push_back(mktime(&tm));
                    }
                }
            }
        }
        
        if (archivos_backup.empty())
        {
            cout << "\n=== NO HAY BACKUPS DISPONIBLES ===" << endl;
            cout << "No se encontraron archivos de backup en el directorio." << endl;
            cout << "Asegúrese de que existan archivos con formato:" << endl;
            cout << "backup_clientes_YYYYMMDD_HHMMSS.bin" << endl;
            pausar_consola();
            inicializar_marquesina();
            return;
        }
        
        // Ordenar por fecha (más reciente primero)
        vector<pair<string, time_t>> pares;
        for (size_t i = 0; i < archivos_backup.size(); ++i)
        {
            pares.emplace_back(archivos_backup[i], fechas_backup[i]);
        }
        sort(pares.begin(), pares.end(), [](const auto& a, const auto& b) {
            return a.second > b.second; // Más reciente primero
        });
        
        // Actualizar vectores ordenados
        archivos_backup.clear();
        fechas_backup.clear();
        for (const auto& par : pares)
        {
            archivos_backup.push_back(par.first);
            fechas_backup.push_back(par.second);
        }
        
        cout << "\n=== BACKUPS DISPONIBLES ===" << endl;
        cout << "Total de backups encontrados: " << archivos_backup.size() << endl;
        cout << "Seleccione el backup que desea restaurar:" << endl;
        cout << "============================================" << endl;
        
        // Mostrar tabla con flechitas
        cout << std::left << std::setw(5) << "№"
             << std::setw(25) << "Archivo"
             << std::setw(12) << "Fecha"
             << std::setw(10) << "Hora"
             << std::setw(10) << "Tamaño" << endl;
        cout << "------------------------------------------------------------" << endl;
        
        for (size_t i = 0; i < archivos_backup.size(); ++i)
        {
            char fecha_str[20], hora_str[10];
            struct tm* tm_info = localtime(&fechas_backup[i]);
            strftime(fecha_str, sizeof(fecha_str), "%d/%m/%Y", tm_info);
            strftime(hora_str, sizeof(hora_str), "%H:%M:%S", tm_info);
            
            // Obtener tamaño del archivo
            std::ifstream file(archivos_backup[i], std::ios::binary | std::ios::ate);
            std::streamsize size = file.tellg();
            file.close();
            
            string tamaño_str;
            if (size >= 1024 * 1024) {
                tamaño_str = std::to_string(size / (1024 * 1024)) + " MB";
            } else if (size >= 1024) {
                tamaño_str = std::to_string(size / 1024) + " KB";
            } else {
                tamaño_str = std::to_string(size) + " B";
            }
            
            cout << std::left << std::setw(5) << (i + 1)
                 << std::setw(25) << archivos_backup[i].substr(0, 24)
                 << std::setw(12) << fecha_str
                 << std::setw(10) << hora_str
                 << std::setw(10) << tamaño_str << endl;
        }
        cout << "------------------------------------------------------------" << endl;
        
        // Crear array de opciones para selección con flechitas
        vector<const char*> opciones_backup;
        vector<string> opciones_str;
        for (size_t i = 0; i < archivos_backup.size(); ++i)
        {
            char fecha_str[20], hora_str[10];
            struct tm* tm_info = localtime(&fechas_backup[i]);
            strftime(fecha_str, sizeof(fecha_str), "%d/%m/%Y", tm_info);
            strftime(hora_str, sizeof(hora_str), "%H:%M:%S", tm_info);
            
            string opcion = string(fecha_str) + " " + string(hora_str);
            opciones_str.push_back(opcion);
            opciones_backup.push_back(opciones_str.back().c_str());
        }
        
        // Agregar opción de cancelar
        opciones_str.push_back("Cancelar operación");
        opciones_backup.push_back(opciones_str.back().c_str());
        
        system("cls");
        ajustar_cursor_para_marquesina();
        int seleccion = seleccionar_opcion("===== SELECCIONAR BACKUP =====", 
                                         opciones_backup.data(), 
                                         opciones_backup.size(), 4);
        
        // Si selecciona cancelar
        if (seleccion == static_cast<int>(archivos_backup.size()))
        {
            cout << endl;
            cout << "Operación cancelada por el usuario." << endl;
            pausar_consola();
            inicializar_marquesina();
            return;
        }
        
        string nombre_archivo = archivos_backup[seleccion];
        
        // Validar si el archivo todavía existe
        std::ifstream test_file(nombre_archivo, std::ios::binary);
        if (!test_file.good()) {
            cout << "\n=== ERROR: El backup seleccionado ya no existe ===" << endl;
            cout << "Archivo: " << nombre_archivo << endl;
            pausar_consola();
            inicializar_marquesina();
            return;
        }
        test_file.close();
        
        cout << "\n===========================================" << endl;
        cout << "===      INICIANDO RESTAURACION       ===" << endl;
        cout << "===========================================" << endl;
        cout << "Archivo seleccionado: " << nombre_archivo << endl;
        cout << "Cargando datos del backup..." << endl;
        
        ListaDoble<Cliente*>* nuevos_clientes = RespaldoDatos::restaurarClientesBinario(nombre_archivo);
        
        if (!nuevos_clientes || nuevos_clientes->esta_vacia()) {
            cout << "\n=== ERROR: No se pudieron cargar datos del backup ===" << endl;
            cout << "El archivo puede estar corrupto o vacio." << endl;
            pausar_consola();
            inicializar_marquesina();
            return;
        }
        
        // Validar la integridad de los datos cargados
        int clientes_cargados = 0;
        try {
            clientes_cargados = nuevos_clientes->getTam();
        } catch (const std::exception& e) {
            cout << "Error al obtener el tamaño de la lista: " << e.what() << endl;
            delete nuevos_clientes;
            pausar_consola();
            inicializar_marquesina();
            return;
        }
        
        // Validar que los nuevos datos sean válidos antes de proceder
        if (clientes_cargados == 0) {
            cout << "\n=== ADVERTENCIA: El backup contiene 0 clientes ===" << endl;
            cout << "¿Desea continuar con la restauracion? (esto eliminara todos los datos actuales)" << endl;
            bool confirmar = seleccionar_Si_No();
            if (!confirmar) {
                cout << "Restauracion cancelada por el usuario." << endl;
                delete nuevos_clientes;
                pausar_consola();
                inicializar_marquesina();
                return;
            }
        }
        
        // Actualizar el banco con los nuevos clientes
        try {
            banco.setClientes(nuevos_clientes);
        } catch (const std::exception& e) {
            cout << "\n=== ERROR al actualizar datos del banco ===" << endl;
            cout << "Error: " << e.what() << endl;
            
            if (nuevos_clientes) {
                delete nuevos_clientes;
            }
            
            pausar_consola();
            inicializar_marquesina();
            return;
        }
        
        banco.guardar_datos_binario_sin_backup("datos.txt");
        
        // Mostrar estadísticas del backup restaurado
        int totalClientes = clientes_cargados;
        int totalCuentas = 0;
        try {
            auto clientesActuales = banco.getClientes();
            if (clientesActuales) {
                clientesActuales->recorrer([&totalCuentas](Cliente* cliente) {
                    if (cliente && cliente->get_cuentas()) {
                        totalCuentas += cliente->get_cuentas()->getTam();
                    }
                });
            }
        } catch (const std::exception& e) {
            totalCuentas = 0;
        }
        
        cout << "\n=== BACKUP RESTAURADO EXITOSAMENTE ===" << endl;
        cout << "=======================================" << endl;
        cout << "Archivo restaurado: " << nombre_archivo << endl;
        cout << "Clientes restaurados: " << totalClientes << endl;
        cout << "Cuentas restauradas: " << totalCuentas << endl;
        cout << "Fecha del backup: ";
        
        if (seleccion < fechas_backup.size()) {
            struct tm* tm_info = localtime(&fechas_backup[seleccion]);
            char fecha_completa[50];
            strftime(fecha_completa, sizeof(fecha_completa), "%d/%m/%Y a las %H:%M:%S", tm_info);
            cout << fecha_completa << endl;
        } else {
            cout << "Fecha no disponible" << endl;
        }
        
        cout << "=======================================" << endl;
        cout << "El sistema ha sido restaurado al estado del backup." << endl;
        cout << "Todos los datos actuales han sido reemplazados." << endl;
        pausar_consola();
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        system("cls");
        ajustar_cursor_para_marquesina();
        cout << "\n=== ERROR DE SISTEMA DE ARCHIVOS ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "Codigo de error: " << e.code() << endl;
        cout << "\nPosibles causas:" << endl;
        cout << "- Permisos insuficientes para acceder al directorio" << endl;
        cout << "- Disco lleno o problemas de hardware" << endl;
        cout << "- Archivo en uso por otro proceso" << endl;
        cout << "\nRegresando al menu principal...\n";
        pausar_consola();
    }
    catch (const std::exception& e)
    {
        system("cls");
        ajustar_cursor_para_marquesina();
        cout << "\n=== ERROR AL RECUPERAR BACKUP ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nDetalles tecnicos:" << endl;
        cout << "- Verifique que el archivo de backup no este corrupto" << endl;
        cout << "- Asegurese de tener permisos de lectura" << endl;
        cout << "- Verifique el espacio disponible en disco" << endl;
        cout << "\nRegresando al menu principal...\n";
        pausar_consola();
    }
    catch (...)
    {
        system("cls");
        ajustar_cursor_para_marquesina();
        cout << "\n=== ERROR DESCONOCIDO ===" << endl;
        cout << "Ha ocurrido un error inesperado durante la restauracion." << endl;
        cout << "El sistema puede estar en un estado inconsistente." << endl;
        cout << "\nRecomendaciones:" << endl;
        cout << "- Reinicie la aplicacion" << endl;
        cout << "- Verifique la integridad de los archivos" << endl;
        cout << "- Contacte al administrador del sistema" << endl;
        cout << "\nRegresando al menu principal...\n";
        pausar_consola();
    }
    
    inicializar_marquesina(); // Reiniciar la marquesina antes de salir
}


/**
 * @brief Carga y muestra la base de datos de clientes con opciones de ordenamiento y búsqueda.
 * @param banco Referencia al objeto Banco con los datos de los clientes.
 */
void cargar_base_datos(Banco& banco) {
    system("cls");
    auto *clientes = banco.getClientes();
    if (!clientes || clientes->esta_vacia())
    {
        cout << "No hay clientes registrados en la base de datos.\n";
        pausar_consola();
        return;
    }

    // Menú de selección de campo
    ajustar_cursor_para_marquesina();
    const int NUM_CAMPOS = 5;
    const char *CAMPOS[NUM_CAMPOS] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    int campo = seleccionar_opcion("===== SELECCIONAR CAMPO =====", CAMPOS, NUM_CAMPOS, 4);

    // Menú de selección de orden
    system("cls");
    ajustar_cursor_para_marquesina();
    const int NUM_ORDENES = 2;
    const char *ORDENES[NUM_ORDENES] = {"Ascendente", "Descendente"};
    int orden = seleccionar_opcion("===== SELECCIONAR ORDEN =====", ORDENES, NUM_ORDENES, 4);

    // Convertir lista a vector para ordenamiento más eficiente
    auto &lista = *clientes;
    int n = lista.getTam();
    std::vector<Cliente *> vectorClientes;
    for (int i = 0; i < n; ++i)
    {
        vectorClientes.push_back(lista.get_contador(i));
    }

    // Función de comparación personalizada según el campo seleccionado
    auto comparar = [campo, orden](Cliente *a, Cliente *b) -> bool
    {
        if (campo == 0)
        { // DNI
            return orden == 0 ? std::stoi(a->get_dni()) < std::stoi(b->get_dni())
                              : std::stoi(a->get_dni()) > std::stoi(b->get_dni());
        }
        else if (campo == 1)
        { // Nombre
            string nombre_a = a->get_nombres();
            string nombre_b = b->get_nombres();
            transform(nombre_a.begin(), nombre_a.end(), nombre_a.begin(), ::tolower);
            transform(nombre_b.begin(), nombre_b.end(), nombre_b.begin(), ::tolower);
            return orden == 0 ? nombre_a < nombre_b : nombre_a > nombre_b;
        }
        else if (campo == 2)
        { // Apellido
            string apellido_a = a->get_apellidos();
            string apellido_b = b->get_apellidos();
            transform(apellido_a.begin(), apellido_a.end(), apellido_a.begin(), ::tolower);
            transform(apellido_b.begin(), apellido_b.end(), apellido_b.begin(), ::tolower);
            return orden == 0 ? apellido_a < apellido_b : apellido_a > apellido_b;
        }
        else if (campo == 3)
        { // Teléfono
            return orden == 0 ? std::stoi(a->get_telefono()) < std::stoi(b->get_telefono())
                              : std::stoi(a->get_telefono()) > std::stoi(b->get_telefono());
        }
        else if (campo == 4)
        { // Email
            string email_a = a->get_email();
            string email_b = b->get_email();
            transform(email_a.begin(), email_a.end(), email_a.begin(), ::tolower);
            transform(email_b.begin(), email_b.end(), email_b.begin(), ::tolower);
            return orden == 0 ? email_a < email_b : email_a > email_b;
        }
        return false;
    };

    // Ordenar usando std::sort con comparador personalizado
    std::sort(vectorClientes.begin(), vectorClientes.end(), comparar);

    // Devolver los elementos ordenados a la lista
    for (int i = 0; i < n; ++i)
    {
        lista.set_contador(i, vectorClientes[i]);
    }

    // Detener la marquesina antes de mostrar la tabla
    detener_marquesina();

    // Mostrar tabla alineada
    system("cls");
    cout << "\n\t\t===========================================" << std::endl;
    cout << "\t\t===    LISTADO DE CLIENTES Y CUENTAS    ===" << std::endl;
    cout << "\t\t===========================================" << std::endl;
    cout << std::left
         << "| " << std::setw(4) << "#"
         << "| " << std::setw(12) << "DNI"
         << "| " << std::setw(15) << "Nombre"
         << "| " << std::setw(15) << "Apellido"
         << "| " << std::setw(12) << "Teléfono"
         << "| " << std::setw(25) << "Email"
         << "|" << std::endl;
    cout << "---------------------------------------------------------------"
         << "---------------------" << std::endl;
    for (int i = 0; i < n; ++i)
    {
        Cliente *cliente = lista.get_contador(i);
        std::cout << std::left
                  << "| " << std::setw(4) << (i + 1)
                  << "| " << std::setw(12) << cliente->get_dni()
                  << "| " << std::setw(15) << cliente->get_nombres()
                  << "| " << std::setw(15) << cliente->get_apellidos()
                  << "| " << std::setw(12) << cliente->get_telefono()
                  << "| " << std::setw(25) << cliente->get_email()
                  << "|" << std::endl;
    }
    std::cout << "---------------------------------------------------------------"
              << "---------------------" << std::endl;
    std::cout << "\nTotal de clientes: " << n << std::endl;
    std::cout << "===========================================" << std::endl;

    string respuesta, valor_buscar;
    do
    {
        limpiar_linea("➤ ¿Desea buscar un cliente específico? (S/N): ");
        respuesta = ingresar_alfabetico("");
        if (respuesta == "__ESC__")
            return;
        transform(respuesta.begin(), respuesta.end(), respuesta.begin(), ::toupper);
    } while (respuesta != "S" && respuesta != "N");
    cout << endl;
    if (respuesta == "S")
    {
        do
        {
            limpiar_linea("➤ Ingrese el valor a buscar: ");
            if (campo == 0 || campo == 3)
            {
                valor_buscar = ingresar_dni("");
            }
            else
            {
                valor_buscar = ingresar_alfabetico("");
            }
            if (valor_buscar == "__ESC__")
                return;
        } while (!validar_valor_busqueda(campo, valor_buscar));

        int pos = -1;
        if (campo == 0 || campo == 3)
        {
            int valor = std::stoi(valor_buscar);
            int left = 0, right = n - 1;
            while (left <= right)
            {
                int mid = left + (right - left) / 2;
                int cmp = (campo == 0) ? std::stoi(lista.get_contador(mid)->get_dni())
                                       : std::stoi(lista.get_contador(mid)->get_telefono());
                if (cmp == valor)
                {
                    pos = mid;
                    break;
                }
                if (cmp < valor)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
        }
        else
        {
            int left = 0, right = n - 1;
            while (left <= right)
            {
                int mid = left + (right - left) / 2;
                string cmp = (campo == 1)   ? lista.get_contador(mid)->get_nombres()
                             : (campo == 2) ? lista.get_contador(mid)->get_apellidos()
                                            : lista.get_contador(mid)->get_email();
                int res = cmp.compare(valor_buscar);
                if (res == 0)
                {
                    pos = mid;
                    break;
                }
                if (res < 0)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
        }

        if (pos != -1)
        {
            Cliente *cliente = lista.get_contador(pos);
            cout << "\n=== CLIENTE ENCONTRADO ===\n";
            cout << "DNI: " << cliente->get_dni() << "\n";
            cout << "Nombre: " << cliente->get_nombres() << "\n";
            cout << "Apellido: " << cliente->get_apellidos() << "\n";
            cout << "Teléfono: " << cliente->get_telefono() << "\n";
            cout << "Email: " << cliente->get_email() << "\n";
            auto *cuentas = cliente->get_cuentas();
            if (cuentas && !cuentas->esta_vacia())
            {
                cout << "\n=== CUENTAS DEL CLIENTE ===\n";
                cout << std::left << std::setw(15) << "ID Cuenta"
                     << std::setw(12) << "Tipo"
                     << std::setw(15) << "Saldo"
                     << std::setw(20) << "Fecha Apertura" << std::endl;
                cout << "--------------------------------------------------------------\n";
                cuentas->recorrer([](Cuenta *cuenta)
                                  { cout << std::left << std::setw(15) << cuenta->get_id_cuenta()
                                         << std::setw(12) << cuenta->get_tipo()
                                         << std::setw(15) << ("$" + std::to_string(cuenta->get_saldo()))
                                         << std::setw(20) << cuenta->get_fecha_apertura().to_string() << std::endl; });
                cout << "--------------------------------------------------------------\n";
            }
            else
            {
                cout << "\nEste cliente no tiene cuentas registradas.\n";
            }
        }
        else
        {
            cout << "\nNo se encontró ningún cliente con ese valor.\n";
        }
    }

    cout << "\nPresione cualquier tecla para continuar...";
    getch();
    inicializar_marquesina();
}


/**
 * @brief Descifra archivos de respaldo en formato TXT.
 * @param banco Referencia al objeto Banco (parámetro requerido pero no utilizado).
 */
void descifrar_archivos_txt(Banco& banco) {
    system("cls");    
    RespaldoDatos respaldo;
    respaldo.seleccionarYDescifrarTxt();
}


/**
 * @brief Cifra archivos de respaldo binarios a formato TXT con cifrado César.
 * @param banco Referencia al objeto Banco (parámetro requerido pero no utilizado).
 */
void cifrar_archivos_txt(Banco& banco) {
    RespaldoDatos respaldo;
    system("cls");
    visibilidad_cursor(true);
    cout << "\n===========================================" << endl;
    cout << "===    CIFRAR ARCHIVO DE RESPALDO    ===" << endl;
    cout << "===========================================" << endl;

    vector<string> backups;
    for (const auto &entry : filesystem::directory_iterator("."))
    {
        string nombre = entry.path().filename().string();
        if (nombre.find("backup_clientes_") == 0 && nombre.substr(nombre.size() - 4) == ".bin")
        {
            backups.push_back(nombre);
        }
    }

    if (backups.empty())
    {
        cout << "\nNo hay archivos de respaldo (.bin) para cifrar.\n";
        pausar_consola();
        return;
    }

    cout << "\n=== SELECCIONE EL ARCHIVO DE RESPALDO A CIFRAR ===\n";
    for (size_t i = 0; i < backups.size(); ++i)
    {
        cout << (i + 1) << ". " << backups[i] << endl;
    }

    int seleccion = 0;
    string numero;
    do
    {
        limpiar_linea("Seleccione el archivo a cifrar (1-" + to_string(backups.size()) + "): ");
        numero = ingresar_dni("");
        if (numero == "__ESC__")
            return;
        if (!numero.empty())
        {
            try
            {
                seleccion = stoi(numero);
            }
            catch (...)
            {
                seleccion = 0;
            }
        }
    } while (seleccion < 1 || seleccion > static_cast<int>(backups.size()));
    cout << endl;
    string nombre_archivo = backups[seleccion - 1];

    cout << "-------------------------------------------" << endl;
    cout << "Archivo seleccionado: " << nombre_archivo << endl;
    cout << "Iniciando proceso de cifrado..." << endl;
    int numCesar = 3;
    respaldo.cifrarArchivoABaseTxt(nombre_archivo, numCesar);
    cout << "-------------------------------------------" << endl;        cout << "=== CIFRADO COMPLETADO CON EXITO ===" << endl;
    size_t pos1 = nombre_archivo.find("backup_clientes_");
    size_t pos2 = nombre_archivo.find(".bin");
    string timestamp = (pos1 != string::npos && pos2 != string::npos)
                           ? nombre_archivo.substr(16, pos2 - 16)
                           : "error_timestamp";
    cout << "El archivo ha sido cifrado y guardado como: cifrado_" << timestamp << ".txt" << endl;
    cout << "===========================================" << endl;

    pausar_consola();
}


/**
 * @brief Muestra el menú de administrador con todas las opciones disponibles.
 * @param banco Referencia al objeto Banco para realizar operaciones administrativas.
 */
void menu_administrador(Banco &banco)
{
    const int NUM_OPCIONES = 12; // Incrementado a 12
    const char *OPCIONES[NUM_OPCIONES] = {
        "Consultar movimientos por fecha",
        "Consultar cuentas por DNI/nombre",
        "Base de datos",
        "Árbol binario",
        "Recuperar backup por fecha y hora",
        "Crear backup manual",
        "Cifrar archivos",
        "Descifrar archivos",
        "Verificar integridad de datos (Hash)",
        "Generar tabla Hash",
        "Generar PDF de clientes",
        "Salir"};

    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);
    string usuario, contrasenia;
    try
    {
        cout << "\n\t\t===========================================" << endl;
        cout << "\t\t==========     ADMINISTRADOR     ==========" << endl;
        cout << "\t\t===========================================" << endl;
        do
        {
            limpiar_linea("➤ Ingrese el usuario del administrador: ");
            usuario = ingresar_alfabetico("");
            if (usuario == "__ESC__")
                return;
        } while (usuario.empty() || usuario.length() < 4);
        cout << endl;
        do
        {
            limpiar_linea("➤ Ingrese la contraseña del administrador: ");
            contrasenia = ingresar_contrasenia_administrador("");
            if (contrasenia == "__ESC__")
                return;
        } while (contrasenia.empty());
        cout << endl;

        if (!validar_credenciales_administrador(usuario, contrasenia))
        {
            throw runtime_error("Usuario o contraseña incorrectos");
        }

        int opcion;
        do
        {
            system("cls");
            ajustar_cursor_para_marquesina();
            opcion = seleccionar_opcion("===== MENÚ ADMINISTRADOR =====", OPCIONES, NUM_OPCIONES, 4);
            switch (opcion)
            {
            case 0:
                consultar_movimientos(banco);
                break;
            case 1:
                consultar_cuentas(banco);
                break;
            case 2:
                cargar_base_datos(banco);
                break;
            case 3:
                mostrar_arbol_binario(banco);
                break;
            case 4:
                recuperar_backup_por_fecha(banco);
                break;
            case 5:
                crear_backup_manual(banco);
                break;
            case 6:
                cifrar_archivos_txt(banco);
                break;
            case 7:
                descifrar_archivos_txt(banco);
                break;
            case 8:
                verificar_hash(banco);
                break;
            case 9:
                buscar_con_tabla_hash(banco);
                break;
            case 10: // Opción para generar PDF
                generateClientDataPDF(banco.getClientes());
                pausar_consola(); // Pausa para que el usuario vea el resultado
                break;
            case 11: // Nueva posición de "Salir"
                return;
            }
        } while (opcion != 11); // Cambiado a 11 para coincidir con "Salir"
    }
    catch (const std::exception &e)
    {
        cerr << "Error en autenticación: " << e.what() << endl;
        cout << "\n=== ERROR AL ACCEDER AL MENÚ DE ADMINISTRADOR ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}


/**
 * @brief Muestra el menú de cliente con opciones de depósito, retiro y generación de QR.
 * @param banco Referencia al objeto Banco para realizar operaciones bancarias.
 */
void menu_cliente(Banco &banco)
{
    const int NUM_OPCIONES = 4;
    const char *OPCIONES[NUM_OPCIONES] = {
        "Realizar depósito",
        "Realizar retiro",
        "Generar QR",
        "Salir"};

    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);
    string dni, contrasenia;
    try
    {
        do
        {
            limpiar_linea("➤ Ingrese su DNI: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__")
                return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;

        do
        {
            limpiar_linea("➤ Ingrese su contraseña: ");
            contrasenia = ingresar_contrasenia("");
            if (contrasenia == "__ESC__")
                return;
        } while (!validar_contrasenia(contrasenia));
        cout << endl;

        Cliente *cliente = banco.buscar_cliente(dni);
        if (!cliente)
        {
            throw runtime_error("Cliente no encontrado");
        }
        if (cliente->get_contrasenia() != contrasenia)
        {
            throw runtime_error("Contraseña incorrecta");
        }

        int opcion;
        do
        {
            system("cls");
            ajustar_cursor_para_marquesina();
            opcion = seleccionar_opcion("===== MENÚ CLIENTE =====", OPCIONES, NUM_OPCIONES, 4);
            switch (opcion)
            {
            case 0:
                realizar_deposito(banco, dni);
                break;
            case 1:
                realizar_retiro(banco, dni);
                break;
            case 2:
            {
                // Obtener la lista de cuentas del cliente
                ListaDoble<Cuenta *> *cuentas = cliente->get_cuentas();
                if (cuentas->esta_vacia())
                {
                    cout << "El cliente no tiene cuentas asociadas." << endl;
                    pausar_consola();
                    break;
                }

                // Generar QRs para todas las cuentas en un solo PDF
                string fullName = cliente->get_nombres() + " " + cliente->get_apellidos();
                string fileName = fullName;
                // Reemplazar espacios con guiones bajos para el nombre del archivo
                replace(fileName.begin(), fileName.end(), ' ', '_');
                
                QRCodeGenerator qr(fullName, cuentas);
                qr.generateQRAndPDF();
                
                system("cls");
                cout << "\n==============================================" << endl;
                cout << "          GENERACIÓN DE CÓDIGOS QR           " << endl;
                cout << "==============================================" << endl;
                cout << "PDF con códigos QR generado exitosamente" << endl;
                cout << "Cliente: " << fullName << endl;
                cout << "Número de cuentas procesadas: " << cuentas->getTam() << endl;
                cout << "Archivo generado: qr_cliente_" << fileName << ".pdf" << endl;
                cout << "==============================================" << endl;
                pausar_consola();
            }
            break;
            case 3:
                return;
            }
        } while (opcion != 3);
    }
    catch (const std::exception &e)
    {
        cerr << "Error al iniciar sesión: " << e.what() << endl;
        cout << "\n=== ERROR AL INICIAR SESIÓN ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

/**
 * @brief Muestra el menú principal del sistema bancario.
 * @param //banco Referencia al objeto Banco principal del sistema.
 */
void menu_principal(Banco& banco)
{
    inicializar_marquesina();
    
    const int NUM_OPCIONES = 6;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Crear cuenta",
        "Iniciar sesión",
        "Administrador",
        "Ayuda Técnica",
        "Documentación Técnica",
        "Salir"};

    int opcion;
    do
    {
        system("cls");
        ajustar_cursor_para_marquesina();
        opcion = seleccionar_opcion("===== MENU PRINCIPAL =====", OPCIONES, NUM_OPCIONES, 4);
        switch (opcion)
        {
            case 0:
                menu_cuenta(banco);
                break;
            case 1:
                menu_cliente(banco);
                break;
            case 2:
                menu_administrador(banco);
                break;
            case 3:
                mostrar_ayuda_tecnica();
                break;
            case 4:
                mostrar_documentacion_doxygen();
                break;
            case 5:
                detener_marquesina();
                return;
        }
    } while (opcion != 5); // Cambiado a 5 para coincidir con "Salir" (índice 5)
}

/**
 * @brief Muestra la ayuda técnica del sistema abriendo el manual CHM.
 */
void mostrar_ayuda_tecnica()
{
    system("cls");
    visibilidad_cursor(true);
    try
    {
        cout << endl;
        cout << "Abriendo el Manual de Ayuda Técnica...\n";
        this_thread::sleep_for(chrono::seconds(3));
        HINSTANCE result = ShellExecute(NULL, "open", "AyudaTecnicaBanco.chm", NULL, NULL, SW_SHOWNORMAL);
        if (result <= (HINSTANCE)32)
        {
            throw runtime_error("No se pudo abrir el archivo de ayuda. Asegúrese de que 'AyudaTecnicaBanco.chm' esté en el directorio del programa.");
        }
        cout << "Manual de Ayuda Técnica abierto exitosamente.\n";
    }
    catch (const std::exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        cout << "\n=== ERROR AL ABRIR AYUDA TÉCNICA ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nRegresando al menú principal...\n";
    }
    pausar_consola();
}


/**
 * @brief Muestra la documentación técnica del código (Doxygen).
 */
void mostrar_documentacion_doxygen()
{
    system("cls");
    visibilidad_cursor(true);
    try
    {   
        cout << endl;
        cout << "Abriendo la Documentación Técnica del Código...\n";
        this_thread::sleep_for(chrono::seconds(2));
        
        // Verificar si existe el archivo de documentación
        if (std::filesystem::exists("SistemaBancario_Documentation.chm"))
        {
            HINSTANCE result = ShellExecute(NULL, "open", "SistemaBancario_Documentation.chm", NULL, NULL, SW_SHOWNORMAL);
            if (result <= (HINSTANCE)32)
            {
                throw runtime_error("No se pudo abrir el archivo de documentación. Verifique que el archivo 'SistemaBancario_Documentation.chm' esté en el directorio del programa.");
            }
            cout << "Documentación técnica del código abierta exitosamente.\n";
        }
        else if (std::filesystem::exists("docs/html/index.html"))
        {
            // Si no existe el CHM, intentar abrir la versión HTML
            HINSTANCE result = ShellExecute(NULL, "open", "docs/html/index.html", NULL, NULL, SW_SHOWNORMAL);
            if (result <= (HINSTANCE)32)
            {
                throw runtime_error("No se pudo abrir la documentación HTML.");
            }
            cout << "Documentación técnica del código (HTML) abierta exitosamente.\n";
        }
        else
        {
            throw runtime_error("No se encontró la documentación técnica. Ejecute 'generar_docs.bat' para generar la documentación con Doxygen.");
        }
    }
    catch (const std::exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        cout << "\n=== ERROR AL ABRIR DOCUMENTACIÓN TÉCNICA ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nPara generar la documentación:" << endl;
        cout << "1. Ejecute el archivo 'generar_docs.bat'" << endl;
        cout << "2. Asegúrese de tener Doxygen instalado" << endl;
        cout << "3. Vuelva a intentar abrir la documentación" << endl;
        cout << "\nRegresando al menú principal...\n";
    }
    pausar_consola();
}

/**
 * @brief Abre una cuenta bancaria sin asignación de sucursal (vía aplicación móvil).
 * @param banco Referencia al objeto Banco donde agregar la cuenta.
 * @param tipo_cuenta Tipo de cuenta a crear (1 = Ahorros, 2 = Corriente).
 */
void abrir_cuenta_sin_sucursal(Banco& banco, int tipo_cuenta) {
    string dni, nombre, apellido, direccion, telefono, email, depositar_inicial, saldo_inicial1, contrasenia;
    Fecha fecha_nacimiento;
    double saldo_inicial = 0;
    visibilidad_cursor(true);
    system("cls");
    ajustar_cursor_para_marquesina();
    try
    {
        int fila_actual = 4; // Ajustar para dejar espacio para la marquesina

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el DNI del cliente: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__")
                return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        fila_actual += 2;

        Cliente *cliente_existe = banco.buscar_cliente(dni);
        bool ya_tiene = false;
        if (cliente_existe)
        {
            cliente_existe->get_cuentas()->recorrer([&](Cuenta *cuenta)
                                                    {
                if ((tipo_cuenta == 1 && cuenta->get_tipo() == "Ahorros") ||
                    (tipo_cuenta == 2 && cuenta->get_tipo() == "Corriente")) {
                    ya_tiene = true;
                } });
            if (ya_tiene)
            {
                throw std::runtime_error("El cliente con DNI " + dni + " ya tiene una cuenta de este tipo.");
            }
        }

        if (cliente_existe)
        {
            nombre = cliente_existe->get_nombres();
            apellido = cliente_existe->get_apellidos();
            direccion = cliente_existe->get_direccion();
            telefono = cliente_existe->get_telefono();
            email = cliente_existe->get_email();
            fecha_nacimiento = cliente_existe->get_fecha_nacimiento();
            contrasenia = cliente_existe->get_contrasenia();
        }
        else
        {
            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el nombre del cliente: ");
                nombre = ingresar_alfabetico("");
                if (nombre == "__ESC__")
                    return;
            } while (nombre.empty() || nombre.length() < 3);
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el apellido del cliente: ");
                apellido = ingresar_alfabetico("");
                if (apellido == "__ESC__")
                    return;
            } while (apellido.empty() || apellido.length() < 3);
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la dirección del cliente: ");
                direccion = ingresar_direccion("");
                if (direccion == "__ESC__")
                    return;
            } while (direccion.empty() || direccion.length() < 5);
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el teléfono del cliente: ");
                telefono = ingresar_dni("");
                if (telefono == "__ESC__")
                    return;
            } while (!validar_telefono(telefono) || telefono_existe(banco, telefono));
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el email del cliente: ");
                email = ingresar_email("");
                if (email == "__ESC__")
                    return;
            } while (!validar_email(email) || email_existe(banco, email));
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la fecha de nacimiento (DD/MM/YYYY): ");
                fecha_nacimiento = validarFecha("");
            } while (fecha_nacimiento.empty());
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la contraseña (mínimo 20 caracteres, al menos una mayúscula, minúscula, dígito y carácter especial): ");
                contrasenia = ingresar_contrasenia("");
                if (contrasenia == "__ESC__")
                    return;
            } while (!validar_contrasenia(contrasenia));
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ ¿Desea depositar un monto inicial? (S/N): ");
                depositar_inicial = ingresar_alfabetico("");
                if (depositar_inicial == "__ESC__")
                    return;
                transform(depositar_inicial.begin(), depositar_inicial.end(), depositar_inicial.begin(), ::toupper);
            } while (depositar_inicial != "S" && depositar_inicial != "N");
            cout << endl;
            fila_actual += 2;

            if (depositar_inicial == "S")
            {
                do
                {
                    mover_cursor(1, fila_actual);
                    limpiar_linea("➤ Ingrese el saldo inicial: ");
                    saldo_inicial1 = ingresar_decimales("");
                    if (saldo_inicial1 == "__ESC__")
                        return;
                } while (!validar_monto(saldo_inicial1));
                saldo_inicial = stod(saldo_inicial1);
                cout << endl;
                fila_actual += 2;
            }
        }

        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea crear la cuenta con estos datos?" << endl;
        fila_actual += 1;

        bool resultado = seleccionar_Si_No();
        if (!resultado)
        {
            cout << "Operación cancelada. No se creó la cuenta." << endl;
            cout << "Presione Enter para regresar al menú principal...";
            cin.get();
            return;
        }

        std::string id_cuenta_base = "";
        id_cuenta_base += toupper(nombre[0]);
        id_cuenta_base += toupper(apellido[0]);
        id_cuenta_base += (tipo_cuenta == 1) ? "A" : "C";
        std::string ultimos_dni = dni.substr(dni.length() - 6, 6);
        std::transform(ultimos_dni.begin(), ultimos_dni.end(), ultimos_dni.begin(), ::toupper);
        id_cuenta_base += ultimos_dni;
        id_cuenta_base += generar_cadena_aleatoria(3);
        std::string id_cuenta = id_cuenta_base;

        Fecha fecha_apertura;
        Cuenta *cuenta = nullptr;
        if (tipo_cuenta == 1)
        {
            mover_cursor(1, fila_actual + 4);
            cout << "La tasa de interés para la cuenta de ahorros es 5%" << endl;
            double tasa_interes = 5.0;
            cuenta = new Ahorro(id_cuenta, saldo_inicial, fecha_apertura, tasa_interes);
            fila_actual += 5;
        }
        else if (tipo_cuenta == 2)
        {
            int limite_retiro_diario;
            do
            {
                mover_cursor(1, fila_actual + 4);
                limpiar_linea("!!!Ingrese el límite de retiro diario: ");
                limite_retiro_diario = ingresar_enteros("");
            } while (limite_retiro_diario <= 0);
            cout << endl;
            cuenta = new Corriente(id_cuenta, saldo_inicial, fecha_apertura, limite_retiro_diario);
            fila_actual += 5;
        }

        if (!cuenta)
        {
            throw std::runtime_error("Error al crear la cuenta");
        }

        if (cliente_existe)
        {
            cliente_existe->agregar_cuenta(cuenta);
            banco.guardar_datos_binario("datos.txt");
            mover_cursor(1, fila_actual);
            cout << "=== CUENTA CREADA EXITOSAMENTE ===" << endl;
            mover_cursor(1, fila_actual + 1);
            cout << "ID de cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual + 2);
            cout << "Tipo: " << (tipo_cuenta == 1 ? "Cuenta de Ahorros" : "Cuenta Corriente") << endl;
            mover_cursor(1, fila_actual + 3);
            cout << "¡Cuenta creada vía aplicación móvil!" << endl;
        }
        else
        {
            Cliente *cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fecha_nacimiento, contrasenia);
            cliente->agregar_cuenta(cuenta);
            banco.agregar_cliente(cliente);
            banco.guardar_datos_binario("datos.txt");
            mover_cursor(1, fila_actual);
            cout << "=== CUENTA CREADA EXITOSAMENTE ===" << endl;
            mover_cursor(1, fila_actual + 1);
            cout << "ID de cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual + 2);
            cout << "Tipo: " << (tipo_cuenta == 1 ? "Cuenta de Ahorros" : "Cuenta Corriente") << endl;
            mover_cursor(1, fila_actual + 3);
            cout << "¡Cuenta creada vía aplicación móvil!" << endl;
        }

        pausar_consola();
    }
    catch (const std::exception &e)
    {
        int fila_actual = 3;
        mover_cursor(1, fila_actual);
        cout << "=== ERROR AL CREAR CUENTA ===" << endl;
        mover_cursor(1, fila_actual + 1);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_actual + 2);
        cout << "Presione Enter para regresar al menú principal...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}


/**
 * @brief Abre una cuenta bancaria asignando una sucursal específica (cita presencial).
 * @param banco Referencia al objeto Banco donde agregar la cuenta.
 * @param tipo_cuenta Tipo de cuenta a crear (1 = Ahorros, 2 = Corriente).
 * @param branchId ID de la sucursal donde se creará la cuenta.
 * @param sucursal Nombre de la sucursal para mostrar en confirmación.
 */
void abrir_cuenta(Banco& banco, int tipo_cuenta, int branchId, const string& sucursal) {
    string dni, nombre, apellido, direccion, telefono, email, depositar_inicial, saldo_inicial1, contrasenia;
    Fecha fecha_nacimiento;
    double saldo_inicial = 0;
    visibilidad_cursor(true);
    system("cls");
    ajustar_cursor_para_marquesina();
    try
    {
        int fila_actual = 4;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el DNI del cliente: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__")
                return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        fila_actual += 2;

        Cliente *cliente_existe = banco.buscar_cliente(dni);
        bool ya_tiene = false;
        if (cliente_existe)
        {
            cliente_existe->get_cuentas()->recorrer([&](Cuenta *cuenta)
                                                    {
                if ((tipo_cuenta == 1 && cuenta->get_tipo() == "Ahorros") ||
                    (tipo_cuenta == 2 && cuenta->get_tipo() == "Corriente")) {
                    ya_tiene = true;
                } });
            if (ya_tiene)
            {
                throw runtime_error("El cliente con DNI " + dni + " ya tiene una cuenta de este tipo.");
            }
        }

        if (cliente_existe)
        {
            nombre = cliente_existe->get_nombres();
            apellido = cliente_existe->get_apellidos();
            direccion = cliente_existe->get_direccion();
            telefono = cliente_existe->get_telefono();
            email = cliente_existe->get_email();
            fecha_nacimiento = cliente_existe->get_fecha_nacimiento();
            contrasenia = cliente_existe->get_contrasenia();
        }
        else
        {
            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el nombre del cliente: ");
                nombre = ingresar_alfabetico("");
                if (nombre == "__ESC__")
                    return;
            } while (nombre.empty() || nombre.length() < 3);
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el apellido del cliente: ");
                apellido = ingresar_alfabetico("");
                if (apellido == "__ESC__")
                    return;
            } while (apellido.empty() || apellido.length() < 3);
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la dirección del cliente: ");
                direccion = ingresar_direccion("");
                if (direccion == "__ESC__")
                    return;
            } while (direccion.empty() || direccion.length() < 5);
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el teléfono del cliente: ");
                telefono = ingresar_dni("");
                if (telefono == "__ESC__")
                    return;
            } while (!validar_telefono(telefono) || telefono_existe(banco, telefono));
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el email del cliente: ");
                email = ingresar_email("");
                if (email == "__ESC__")
                    return;
            } while (!validar_email(email) || email_existe(banco, email));
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la fecha de nacimiento (DD/MM/YYYY): ");
                fecha_nacimiento = validarFecha("");
            } while (fecha_nacimiento.empty());
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la contraseña (mínimo 20 caracteres, al menos una mayúscula, minúscula, dígito y carácter especial): ");
                contrasenia = ingresar_contrasenia("");
                if (contrasenia == "__ESC__")
                    return;
            } while (!validar_contrasenia(contrasenia));
            cout << endl;
            fila_actual += 2;

            do
            {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ ¿Desea depositar un monto inicial? (S/N): ");
                depositar_inicial = ingresar_alfabetico("");
                if (depositar_inicial == "__ESC__")
                    return;
                transform(depositar_inicial.begin(), depositar_inicial.end(), depositar_inicial.begin(), ::toupper);
            } while (depositar_inicial != "S" && depositar_inicial != "N");
            cout << endl;
            fila_actual += 2;

            if (depositar_inicial == "S")
            {
                do
                {
                    mover_cursor(1, fila_actual);
                    limpiar_linea("➤ Ingrese el saldo inicial: ");
                    saldo_inicial1 = ingresar_decimales("");
                    if (saldo_inicial1 == "__ESC__")
                        return;
                } while (!validar_monto(saldo_inicial1));
                saldo_inicial = stod(saldo_inicial1);
                cout << endl;
                fila_actual += 2;
            }
        }

        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea crear la cuenta con estos datos?" << endl;
        fila_actual += 1;

        bool resultado = seleccionar_Si_No();
        if (!resultado)
        {
            cout << "Operación cancelada. No se creó la cuenta." << endl;
            cout << "Presione Enter para regresar al menú principal...";
            cin.get();
            return;
        }

        string id_cuenta_base = "";
        id_cuenta_base += toupper(nombre[0]);
        id_cuenta_base += toupper(apellido[0]);
        id_cuenta_base += (tipo_cuenta == 1) ? "A" : "C";
        string ultimos_dni = dni.substr(dni.length() - 6, 6);
        transform(ultimos_dni.begin(), ultimos_dni.end(), ultimos_dni.begin(), ::toupper);
        id_cuenta_base += ultimos_dni;
        id_cuenta_base += generar_cadena_aleatoria(3);
        string id_cuenta = id_cuenta_base;

        Fecha fecha_apertura;
        Cuenta *cuenta = nullptr;
        if (tipo_cuenta == 1)
        {
            mover_cursor(1, fila_actual + 4);
            cout << "La tasa de interés para la cuenta de ahorros es 5%" << endl;
            double tasa_interes = 5.0;
            cuenta = new Ahorro(id_cuenta, saldo_inicial, fecha_apertura, tasa_interes);
            fila_actual += 5;
        }
        else if (tipo_cuenta == 2)
        {
            int limite_retiro_diario;
            do
            {
                mover_cursor(1, fila_actual + 4);
                limpiar_linea("!!!Ingrese el límite de retiro diario: ");
                limite_retiro_diario = ingresar_enteros("");
            } while (limite_retiro_diario <= 0);
            cout << endl;
            cuenta = new Corriente(id_cuenta, saldo_inicial, fecha_apertura, limite_retiro_diario);
            fila_actual += 5;
        }

        if (!cuenta)
        {
            throw runtime_error("Error al crear la cuenta");
        }

        cuenta->set_branchId(branchId);
        auto appointmentTime = banco.findNextAvailableSlot(branchId);
        cuenta->set_appointmentTime(appointmentTime);
        banco.addAppointment(branchId, appointmentTime);

        if (cliente_existe)
        {
            cliente_existe->agregar_cuenta(cuenta);
            banco.guardar_datos_binario("datos.txt");
            mover_cursor(1, fila_actual);
            cout << "=== CUENTA CREADA EXITOSAMENTE ===" << endl;
            mover_cursor(1, fila_actual + 1);
            cout << "ID de cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual + 2);
            cout << "Sucursal: " << sucursal << endl;
            time_t tt = chrono::system_clock::to_time_t(appointmentTime);
            tm local_tm = *localtime(&tt);
            cout << "Cita asignada: " << local_tm.tm_mday << "/" << local_tm.tm_mon + 1 << "/" << local_tm.tm_year + 1900
                 << " " << local_tm.tm_hour << ":" << (local_tm.tm_min < 10 ? "0" : "") << local_tm.tm_min << endl;
        }
        else
        {
            Cliente *cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fecha_nacimiento, contrasenia);
            cliente->agregar_cuenta(cuenta);
            banco.agregar_cliente(cliente);
            banco.guardar_datos_binario("datos.txt");
            mover_cursor(1, fila_actual);
            cout << "=== CUENTA CREADA EXITOSAMENTE ===" << endl;
            mover_cursor(1, fila_actual + 1);
            cout << "ID de cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual + 2);
            cout << "Sucursal: " << sucursal << endl;
            time_t tt = chrono::system_clock::to_time_t(appointmentTime);
            tm local_tm = *localtime(&tt);
            cout << "Cita asignada: " << local_tm.tm_mday << "/" << local_tm.tm_mon + 1 << "/" << local_tm.tm_year + 1900
                 << " " << local_tm.tm_hour << ":" << (local_tm.tm_min < 10 ? "0" : "") << local_tm.tm_min << endl;
        }
    }
    catch (const std::exception &e)
    {
        int fila_error = 3;
        mover_cursor(1, fila_error);
        cout << "=== ERROR AL CREAR CUENTA ===" << endl;
        mover_cursor(1, fila_error + 1);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error + 2);
        cout << "Presione Enter para regresar al menú principal...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}


/**
 * @brief Realiza un depósito en una cuenta bancaria específica.
 * @param banco Referencia al objeto Banco para buscar el cliente y la cuenta.
 * @param dni DNI del cliente que realizará el depósito.
 */
void realizar_deposito(Banco& banco, const string& dni)
{
    string salario, id_cuenta;
    double monto = 0;
    system("cls");
    visibilidad_cursor(true);
    try
    {
        int fila_actual = 2;
        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "         DEPÓSITO EN CUENTA BANCARIA         " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        Cliente *cliente = banco.buscar_cliente(dni);
        if (!cliente)
            throw runtime_error("Cliente no encontrado");

        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el ID de la cuenta: ");
            id_cuenta = ingresar_id("");
            if (id_cuenta == "__ESC__")
                return;
        } while (!validar_id_cuenta(cliente, id_cuenta));
        cout << endl;
        fila_actual += 2;
        Cuenta *cuenta = cliente->buscar_cuenta(id_cuenta);

        mover_cursor(1, fila_actual++);
        cout << "Saldo actual: $" << cuenta->get_saldo() << endl;
        fila_actual++;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el monto a depositar: ");
            salario = ingresar_decimales("");
            if (salario == "__ESC__")
                return;
        } while (!validar_monto(salario));
        monto = stod(salario);
        cout << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea realizar el depósito?" << endl;
        fila_actual += 1;

        bool resultado = seleccionar_Si_No();
        if (!resultado)
        {
            cout << "Operación cancelada. No se realizó el depósito." << endl;
            cout << "Presione Enter para regresar al menú principal...";
            cin.get();
            return;
        }

        Fecha fecha;
        cuenta->depositar(monto, fecha);
        banco.guardar_datos_binario("datos.txt");
        RespaldoDatos::respaldoClientesBinario("respaldo_clientes.bin", *banco.getClientes());

        fila_actual += 4;
        mover_cursor(1, fila_actual++);
        cout << "==============================================" << endl;
        mover_cursor(1, fila_actual++);
        cout << "   DEPÓSITO REALIZADO EXITOSAMENTE" << endl;
        mover_cursor(1, fila_actual++);
        cout << "   Monto depositado: $" << monto << endl;
        mover_cursor(1, fila_actual++);
        cout << "   Cuenta: " << id_cuenta << endl;
        mover_cursor(1, fila_actual++);
        cout << "   Nuevo saldo: $" << cuenta->get_saldo() << endl;
        mover_cursor(1, fila_actual++);
        cout << "   Datos guardados en datos.txt" << endl;
        mover_cursor(1, fila_actual++);
        cout << "==============================================" << endl;
        mover_cursor(1, fila_actual++);
        cout << "Regresando al menú cliente..." << endl;
        pausar_consola();
    }
    catch (const std::exception &e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        cerr << "Error al realizar depósito: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\n=== ERROR AL REALIZAR DEPÓSITO ===" << endl;
        mover_cursor(1, fila_error++);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\nRegresando al menú cliente...\n";
        pausar_consola();
    }
}

/**
 * @brief Realiza un retiro de una cuenta bancaria específica.
 * @param banco Referencia al objeto Banco para buscar el cliente y la cuenta.
 * @param dni DNI del cliente que realizará el retiro.
 */
void realizar_retiro(Banco& banco, const string& dni)
{
    system("cls");
    visibilidad_cursor(true);
    string salario, id_cuenta;
    double monto = 0;
    try
    {
        int fila_actual = 2;
        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "           RETIRO DE CUENTA BANCARIA         " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        Cliente *cliente = banco.buscar_cliente(dni);
        if (!cliente)
            throw runtime_error("Cliente no encontrado");

        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el ID de la cuenta: ");
            id_cuenta = ingresar_id("");
            if (id_cuenta == "__ESC__")
                return;
        } while (!validar_id_cuenta(cliente, id_cuenta));
        cout << endl;
        fila_actual += 2;
        Cuenta *cuenta = cliente->buscar_cuenta(id_cuenta);

        mover_cursor(1, fila_actual++);
        cout << "Saldo actual: $" << cuenta->get_saldo() << endl;
        fila_actual++;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el monto a retirar: ");
            salario = ingresar_decimales("");
            if (salario == "__ESC__")
                return;
        } while (!validar_monto(salario));
        monto = stod(salario);
        cout << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea realizar el retiro?" << endl;
        fila_actual += 1;

        bool resultado = seleccionar_Si_No();
        if (!resultado)
        {
            cout << "Operación cancelada. No se realizó el retiro." << endl;
            cout << "Presione Enter para regresar al menú principal...";
            cin.get();
            return;
        }

        Fecha fecha;
        if (cuenta->retirar(monto, fecha))
        {
            banco.guardar_datos_binario("datos.txt");
            RespaldoDatos::respaldoClientesBinario("respaldo_clientes.bin", *banco.getClientes());
            fila_actual += 4;
            mover_cursor(1, fila_actual++);
            cout << "==============================================" << endl;
            mover_cursor(1, fila_actual++);
            cout << "     RETIRO REALIZADO EXITOSAMENTE" << endl;
            mover_cursor(1, fila_actual++);
            cout << "     Monto retirado: $" << monto << endl;
            mover_cursor(1, fila_actual++);
            cout << "     Cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual++);
            cout << "     Nuevo saldo: $" << cuenta->get_saldo() << endl;
            mover_cursor(1, fila_actual++);
            cout << "     Datos guardados en datos.txt" << endl;
            mover_cursor(1, fila_actual++);
            cout << "==============================================" << endl;
            mover_cursor(1, fila_actual++);
            cout << "Regresando al menú cliente..." << endl;
        }
        else
        {
            fila_actual += 4;
            mover_cursor(1, fila_actual++);
            cout << "==============================================" << endl;
            mover_cursor(1, fila_actual++);
            cout << "        FALLO EN EL RETIRO" << endl;
            mover_cursor(1, fila_actual++);
            cout << "Verifique el saldo o el límite de retiro diario." << endl;
            mover_cursor(1, fila_actual++);
            cout << "==============================================" << endl;
            mover_cursor(1, fila_actual++);
            cout << "Regresando al menú cliente..." << endl;
        }
        pausar_consola();
    }
    catch (const std::exception &e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        cerr << "Error al realizar retiro: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\n=== ERROR AL REALIZAR RETIRO ===" << endl;
        mover_cursor(1, fila_error++);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\nRegresando al menú cliente...\n";
        pausar_consola();
    }
}


/**
 * @brief Consulta los movimientos de un cliente en un rango de fechas específico.
 * @param banco Referencia al objeto Banco para buscar el cliente y sus movimientos.
 */
void consultar_movimientos(Banco& banco)
{
    system("cls");
    visibilidad_cursor(true);
    try
    {
        int fila_actual = 2;
        string dni;
        Fecha fecha_inicio, fecha_fin;

        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "      CONSULTA DE MOVIMIENTOS POR FECHA      " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el DNI del cliente: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__")
                return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        fila_actual += 2;

        Cliente *cliente = banco.buscar_cliente(dni);
        if (!cliente)
            throw runtime_error("Cliente con DNI " + dni + " no encontrado");

        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese la fecha de inicio (DD/MM/YYYY): ");
            fecha_inicio = validarFecha("");
        } while (fecha_inicio.empty());
        cout << endl;
        fila_actual += 2;

        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese la fecha de fin (DD/MM/YYYY): ");
            fecha_fin = validarFecha("");
        } while (fecha_fin.empty() || fecha_inicio > fecha_fin);
        cout << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual++);
        cout << "Consultando movimientos..." << endl;
        this_thread::sleep_for(chrono::seconds(3));

        detener_marquesina();

        banco.consultar_movimientos_rango(dni, fecha_inicio, fecha_fin);
        getch();

        inicializar_marquesina();
    }
    catch (const std::exception &e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        cerr << "Error al consultar movimientos: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\n=== ERROR AL CONSULTAR MOVIMIENTOS ===" << endl;
        mover_cursor(1, fila_error++);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
        inicializar_marquesina();
    }
}


/**
 * @brief Consulta las cuentas de un cliente específico por DNI, nombre o apellido.
 * @param banco Referencia al objeto Banco para buscar el cliente y sus cuentas.
 */
void consultar_cuentas(Banco& banco)
{
    system("cls");
    visibilidad_cursor(true);
    try
    {
        int fila_actual = 2;
        string dni, nombre, apellido;
        bool criterio_valido = false;

        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "      CONSULTA DE CUENTAS DE CLIENTE         " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el DNI del cliente (o deje vacío): ");
        dni = ingresar_dni("");
        if (dni == "__ESC__")
            return;
        if (!dni.empty() && validarCedulaEcuatoriana(dni))
            criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el nombre del cliente (o deje vacío): ");
        nombre = ingresar_alfabetico("");
        if (nombre == "__ESC__")
            return;
        if (!nombre.empty())
            criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el apellido del cliente (o deje vacío): ");
        apellido = ingresar_alfabetico("");
        if (apellido == "__ESC__")
            return;
        if (!apellido.empty())
            criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        if (!criterio_valido)
        {
            mover_cursor(1, fila_actual++);
            throw invalid_argument("Debe ingresar al menos un criterio de búsqueda (DNI, nombre o apellido)");
        }

        mover_cursor(1, fila_actual++);
        cout << "Buscando cuentas..." << endl;
        this_thread::sleep_for(chrono::seconds(3));

        detener_marquesina();

        banco.consultar_cuentas_cliente(dni, nombre, apellido, fila_actual, [](Cuenta *cuenta)
                                        { cout << "ID: " << cuenta->get_id_cuenta()
                                               << ", Tipo: " << cuenta->get_tipo()
                                               << ", Saldo: " << cuenta->get_saldo() << endl; });
        getch();

        inicializar_marquesina();
    }
    catch (const std::exception &e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        cerr << "Error al consultar cuentas: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\n=== ERROR AL CONSULTAR CUENTAS ===" << endl;
        mover_cursor(1, fila_error++);
        cout << "Error: " << e.what() << endl;
               mover_cursor(1, fila_error++);
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
        inicializar_marquesina();
    }
}


/**
 * @brief Muestra el menú de selección de métodos para la apertura de cuentas.
 * @param banco Referencia al objeto Banco para las operaciones bancarias.
 * @details Permite al usuario elegir entre crear una cuenta vía aplicación o agendar una cita presencial.
 */
void menu_cuenta(Banco& banco) {
    const int NUM_METODOS = 3;
    const char* METODOS[NUM_METODOS] = {
        "Crear cuenta vía aplicación",
        "Agendar cita presencial",
        "Menú Principal"
    };

    system("cls");
    ajustar_cursor_para_marquesina();
    int metodo = seleccionar_opcion("===== MÉTODO DE APERTURA =====", METODOS, NUM_METODOS, 4);

    switch (metodo) {
        case 0: 
            menu_cuenta_aplicacion(banco);
            break;
        case 1:
            menu_cita_presencial(banco);
            break;
        case 2:
            return;
    }
}

/**
 * @brief Variable global para la marquesina del sistema.
 * @details Controla la marquesina que se muestra en la interfaz del usuario.
 */
Marquesina marquesina_global;

/**
 * @brief Inicializa la marquesina del sistema con un texto específico.
 * @details Inicia la marquesina con el texto "SISTEMA BANCARIO" para ser mostrada en la interfaz.
 */
void inicializar_marquesina() {
    marquesina_global.iniciar("SISTEMA BANCARIO");
}

/**
 * @brief Detiene la marquesina del sistema.
 * @details Pausa la animación de la marquesina para no interferir con otras operaciones.
 */
void detener_marquesina() {
    marquesina_global.detener();
}

/**
 * @brief Ajusta el cursor para dejar espacio a la marquesina.
 * @details Mueve el cursor a la posición adecuada para que la marquesina no interfiera con el contenido.
 */
void ajustar_cursor_para_marquesina() {
    mover_cursor(0, 2);
}

/**
 * @brief Obtiene la hora actual del sistema en formato legible.
 * @return String con la hora actual en formato "HH:MM AM/PM -05, MM/DD/YYYY".
 * @details Utiliza chrono para obtener el tiempo del sistema y formatearlo apropiadamente.
 */
string getCurrentTime() {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&time);
    if (!localTime) {
        cerr << "Error: No se pudo obtener el tiempo local." << endl;
        return "Error en tiempo";
    }
    ostringstream oss;
    oss << put_time(localTime, "%I:%M %p -05, %m/%d/%Y");
    return oss.str(); // Return the formatted string
}

/**
 * @brief Muestra el menú de apertura de cuentas vía aplicación.
 * @param banco Referencia al objeto Banco para las operaciones bancarias.
 * @details Permite al usuario elegir entre crear una cuenta de ahorros o corriente usando la aplicación.
 */
void menu_cuenta_aplicacion(Banco& banco) {
    const int NUM_OPCIONES = 3;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Cuenta de Ahorros",
        "Cuenta Corriente",
        "Regresar"
    };

    int opcion;
    do {
        system("cls");
        ajustar_cursor_para_marquesina();
        opcion = seleccionar_opcion("===== TIPO DE CUENTA - VÍA APLICACIÓN =====", OPCIONES, NUM_OPCIONES, 4);
        switch (opcion) {
            case 0:
                abrir_cuenta_sin_sucursal(banco, 1);
                break;
            case 1:
                abrir_cuenta_sin_sucursal(banco, 2);
                break;
            case 2:
                return;
        }
    } while (opcion != 2);
}

/**
 * @brief Muestra el menú de citas presenciales para apertura de cuentas.
 * @param banco Referencia al objeto Banco para las operaciones bancarias.
 * @details Permite al usuario agendar una cita presencial en una sucursal para la apertura de cuentas.
 */
void menu_cita_presencial(Banco& banco) {
    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);

    cout << "\n=== SISTEMA DE CITAS PRESENCIALES ===" << endl;
    cout << "Bienvenido al sistema de agendamiento de citas" << endl;
    cout << "Servicio: Apertura de cuenta presencial\n" << endl;

    // Preguntar si desea agendar cita antes de hacer la selección completa
    string respuesta;
    do {
        limpiar_linea("¿Desea agendar una cita presencial? (S/N): ");
        respuesta = ingresar_alfabetico("");
        if (respuesta == "_ESC_") return;
        transform(respuesta.begin(), respuesta.end(), respuesta.begin(), ::toupper);
    } while (respuesta != "S" && respuesta != "N");

    if (respuesta == "S") {
        agendar_cita_presencial();
    } else {
        cout << "\nGracias por usar nuestro servicio." << endl;
        pausar_consola();
    }
}

/**
 * @brief Procesa el agendamiento de una cita presencial usando el sistema de geolocalización.
 * @details Utiliza el sistema de geolocalización para encontrar la sucursal más cercana,
 * muestra horarios disponibles y permite al usuario agendar una cita para apertura de cuenta.
 */
void agendar_cita_presencial() {
    string sucursal_num, horario_num;
    
    cout << "\n=== AGENDAR CITA PRESENCIAL ===" << endl;
    cout << "HORARIOS DE ATENCIÓN BANCARIA:" << endl;
    cout << "• Lunes a Viernes: 9:00 AM - 3:30 PM" << endl;
    cout << "• Sábados: 9:00 AM - 12:00 PM" << endl;
    cout << "• Domingos: CERRADO" << endl;
    cout << "=======================================" << endl;
    
    // Usar el sistema completo de selección manual y mostrar sucursales
    auto result = geoSystem.runGeolocationSystem();
    Branch selected_branch = result.first;
    double distance = result.second;
    
    // Mostrar horarios disponibles para la sucursal más cercana
    auto horarios = geoSystem.generateValidBankingTimeSlots(selected_branch.queue_position);
    
    cout << "\nSucursal más cercana: " << selected_branch.name << endl;
    cout << "Distancia: " << fixed << setprecision(2) << distance << " km" << endl;
    
    if (selected_branch.queue_position == 0) {
        cout << "Estado: ¡Excelente! No hay cola - Atención inmediata" << endl;
        cout << "Tiempo de espera estimado: 0 minutos" << endl;
    } else {
        cout << "Personas en cola: " << selected_branch.queue_position << endl;
        cout << "Tiempo de espera estimado: " << (selected_branch.queue_position * 40) << " minutos" << endl;
    }
    
    // Filtrar horarios disponibles (excluir "No disponible")
    vector<string> horarios_validos;
    for (const auto& horario : horarios) {
        if (horario != "No disponible") {
            horarios_validos.push_back(horario);
        }
    }
    
    if (horarios_validos.empty()) {
        cout << "\n NO HAY HORARIOS DISPONIBLES" << endl;
        cout << "Motivos posibles:" << endl;
        cout << "• Es domingo (no hay atención)" << endl;
        cout << "• Fuera del horario de atención" << endl;
        cout << "• Demasiada demanda en los próximos días" << endl;
        cout << "\nIntente nuevamente en otro momento." << endl;
        pausar_consola();
        return;
    }
    
    cout << "\nHorarios disponibles:" << endl;
    for (size_t i = 0; i < horarios_validos.size(); i++) {
        cout << (i + 1) << ". " << horarios_validos[i] << endl;
    }
    
    // Seleccionar horario
    int horario_elegido;
    do {
        limpiar_linea("Seleccione el horario (1-" + to_string(horarios_validos.size()) + "): ");
        horario_num = ingresar_dni("");
        if (horario_num == "_ESC_") return;
        try {
            horario_elegido = stoi(horario_num);
        } catch (...) {
            horario_elegido = 0;
        }
    } while (horario_elegido < 1 || horario_elegido > static_cast<int>(horarios_validos.size()));
    
    // Obtener fecha actual
    string fecha_actual = geoSystem.getCurrentDate();
    
    // Confirmación final
    cout << "\n=== CONFIRMACIÓN DE CITA ===" << endl;
    cout << "Sucursal: " << selected_branch.name << endl;
    cout << "Dirección: " << selected_branch.address << endl;
    cout << "Fecha y Hora: " << horarios_validos[horario_elegido - 1] << endl;
    cout << "Servicio: Apertura de cuenta presencial" << endl;
    cout << "Distancia desde su ubicación: " << fixed << setprecision(2) << distance << " km" << endl;
    cout << "\nIMPORTANTE: Respete los horarios de atención bancaria" << endl;
    cout << "DOCUMENTOS REQUERIDOS:" << endl;
    cout << "• Cédula de identidad original" << endl;
    cout << "• Comprobante de ingresos" << endl;
    cout << "• Depósito inicial mínimo: $25.00" << endl;
    
    // Confirmar la cita
    bool confirmar = seleccionar_Si_No();
    if (confirmar) {
        // Actualizar la cola de la sucursal (simular que se agendó una cita)
        geoSystem.updateBranchQueue(selected_branch.id, selected_branch.queue_position + 1);
        
        cout << "\n=== CITA CONFIRMADA ===" << endl;
        cout << "¡Su cita ha sido agendada exitosamente!" << endl;
        cout << "Fecha y Hora: " << horarios_validos[horario_elegido - 1] << endl;
        cout << "Recuerde llegar 10 minutos antes del horario agendado." << endl;
        cout << "Número de confirmación: CB" << selected_branch.id << horario_elegido << time(nullptr) % 10000 << endl;
    } else {
        cout << "\nCita cancelada." << endl;
    }
    
    pausar_consola();
}

/**
 * @brief Verifica la integridad de los datos usando algoritmos de hash MD5.
 * @param banco Referencia al objeto Banco para las operaciones bancarias.
 * @details Calcula el hash MD5 del archivo de datos y lo compara con el hash almacenado
 * para verificar si los datos han sido modificados. Actualiza el hash si es necesario.
 */
void verificar_hash(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    try {
        int fila_actual = 2;
        string filePath = "datos.txt";
        string hashFile = "hash_stored.txt";

        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "     VERIFICACIÓN DE INTEGRIDAD DE DATOS     " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        string currentHash = Hash::calculateMD5(filePath);
        if (currentHash.empty()) {
            throw runtime_error("No se pudo calcular el hash actual.");
        }

        ifstream hashIn(hashFile);
        string storedHash;
        bool hashFileExists = hashIn.good();
        if (hashIn.is_open()) {
            string line;
            while (getline(hashIn, line)) {
                if (!line.empty()) {
                    storedHash = line.substr(0, 32);
                }
            }
            hashIn.close();
        }

        if (!hashFileExists) {
            ofstream hashOut(hashFile);
            if (hashOut.is_open()) {
                string timeStr = getCurrentTime();
                hashOut << currentHash << " (Actualizado: " << timeStr << ")" << endl;
                hashOut.close();
                mover_cursor(1, fila_actual++);
                cout << "Hash inicial generado y guardado: " << currentHash << " a las " << timeStr << endl;
            } else {
                throw runtime_error("No se pudo crear el archivo de hash.");
            }
        } else {
            string lastStoredHash = storedHash.substr(0, 32);
            if (currentHash != lastStoredHash) {
                ofstream hashOut(hashFile, ios::app);
                if (hashOut.is_open()) {
                    string timeStr = getCurrentTime();
                    hashOut << currentHash << " (Actualizado: " << timeStr << ")" << endl;
                    hashOut.close();
                    mover_cursor(1, fila_actual++);
                    cout << "El archivo ha sido modificado. Nuevo hash añadido: " << currentHash << " a las " << timeStr << endl;
                } else {
                    throw runtime_error("No se pudo abrir el archivo para añadir hash.");
                }
            } else {
                mover_cursor(1, fila_actual++);
                cout << "El archivo no ha sido modificado. Hash actual: " << currentHash << endl;
            }
        }

        ifstream debugHash(hashFile);
        string line;
        mover_cursor(1, fila_actual++);
        cout << "Contenido de hash_stored.txt:" << endl;
        while (getline(debugHash, line)) {
            mover_cursor(1, fila_actual++);
            cout << line << endl;
        }
        debugHash.close();

        mover_cursor(1, fila_actual++);
        cout << "Presione Enter para regresar al menú principal..." << endl;
        pausar_consola();
    }
    catch (const std::exception& e) {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        cerr << "Error al verificar integridad: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\n=== ERROR AL VERIFICAR INTEGRIDAD ===" << endl;
        mover_cursor(1, fila_error++);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

/**
 * @brief Implementa un sistema de búsqueda usando tabla hash para localizar clientes.
 * @param banco Referencia al objeto Banco para acceder a los datos de clientes.
 * @details Construye una tabla hash basada en un campo seleccionado (DNI, nombre, apellido, etc.)
 * y permite realizar búsquedas eficientes de clientes. Muestra el contenido de la tabla y
 * los resultados de búsqueda junto con las cuentas asociadas.
 */
void buscar_con_tabla_hash(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    try {
        int fila_actual = 2;
        Hash hash;

        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "         BÚSQUEDA CON TABLA HASH            " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        // Seleccionar campo para la tabla hash
        const char* campos[] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
        int campo = seleccionar_opcion("Seleccione el campo para la tabla hash:", campos, 5, fila_actual);
        fila_actual += 8;

        // Construir la tabla hash usando los datos de Banco
        hash.buildHashTable(banco, campo);
        mover_cursor(1, fila_actual++);
        cout << "Tabla hash creada con éxito usando el campo: " << campos[campo] << endl;
        fila_actual += 2;

        // Imprimir la tabla hash
        mover_cursor(1, fila_actual++);
        cout << "=== CONTENIDO DE LA TABLA HASH ===\n";
        mover_cursor(1, fila_actual++);
        cout << std::left << std::setw(20) << "Clave" << std::setw(30) << "Valor (Nombre)" << endl;
        mover_cursor(1, fila_actual++);
        cout << "------------------------------------------------\n";
        auto contents = hash.getHashTableContents();
        for (const auto& pair : contents) {
            for (const auto* client : pair.second) {
                mover_cursor(1, fila_actual++);
                cout << std::left << std::setw(20) << pair.first << std::setw(30) << (client ? client->get_nombres() : "N/A") << endl;
            }
        }
        mover_cursor(1, fila_actual++);
        cout << "------------------------------------------------\n";
        fila_actual += 2;

        // Solicitar valor a buscar
        string valor_buscar;
        do {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el valor a buscar: ");
            if (campo == 0 || campo == 3) {
                valor_buscar = ingresar_dni("");
            } else {
                valor_buscar = ingresar_alfabetico("");
            }
            if (valor_buscar == "_ESC_") return;
        } while (valor_buscar.empty());
        cout << endl;
        fila_actual += 2;

        // Mostrar el valor a buscar
        mover_cursor(1, fila_actual++);
        cout << "Buscando el valor: \"" << valor_buscar << "\"\n";
        fila_actual += 2;

        // Realizar búsqueda
        std::vector<Cliente*> clientes = hash.searchHash(valor_buscar);
        if (!clientes.empty()) {
            for (size_t i = 0; i < clientes.size(); ++i) {
                Cliente* cliente = clientes[i];
                mover_cursor(1, fila_actual++);
                cout << "\n=== CLIENTE ENCONTRADO " << (i + 1) << " ===\n";
                mover_cursor(1, fila_actual++);
                cout << "DNI: " << cliente->get_dni() << "\n";
                mover_cursor(1, fila_actual++);
                cout << "Nombre: " << cliente->get_nombres() << "\n";
                mover_cursor(1, fila_actual++);
                cout << "Apellido: " << cliente->get_apellidos() << "\n";
                mover_cursor(1, fila_actual++);
                cout << "Teléfono: " << cliente->get_telefono() << "\n";
                mover_cursor(1, fila_actual++);
                cout << "Email: " << cliente->get_email() << "\n";
                auto *cuentas = cliente->get_cuentas();
                if (cuentas && !cuentas->esta_vacia()) {
                    mover_cursor(1, fila_actual++);
                    cout << "\n=== CUENTAS DEL CLIENTE ===\n";
                    mover_cursor(1, fila_actual++);
                    cout << std::left << std::setw(15) << "ID Cuenta" 
                         << std::setw(12) << "Tipo" 
                         << std::setw(15) << "Saldo" 
                         << std::setw(20) << "Fecha Apertura" << std::endl;
                    mover_cursor(1, fila_actual++);
                    cout << "--------------------------------------------------------------\n";
                    cuentas->recorrer([](Cuenta* cuenta) {
                        cout << std::left << std::setw(15) << cuenta->get_id_cuenta()
                             << std::setw(12) << cuenta->get_tipo()
                             << std::setw(15) << ("$" + std::to_string(cuenta->get_saldo()))
                             << std::setw(20) << cuenta->get_fecha_apertura().to_string() << std::endl;
                    });
                    mover_cursor(1, fila_actual++);
                    cout << "--------------------------------------------------------------\n";
                } else {
                    mover_cursor(1, fila_actual++);
                    cout << "\nEste cliente no tiene cuentas registradas.\n";
                }
            }
        } else {
            mover_cursor(1, fila_actual++);
            cout << "\nNo se encontró ningún cliente con el valor: " << valor_buscar << "\n";
        }

        mover_cursor(1, fila_actual++);
        cout << "Presione Enter para regresar al menú principal...";
        getch();
    }
    catch (const std::exception& e) {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        cerr << "Error al buscar con tabla hash: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\n=== ERROR AL BUSCAR CON TABLA HASH ===" << endl;
        mover_cursor(1, fila_error++);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

/**
 * @brief Crea un backup manual de los datos del sistema.
 * @param banco Referencia al objeto Banco para crear el backup.
 * @details Permite al administrador crear un backup manual de todos
 * los datos del sistema con marca de tiempo.
 */
void crear_backup_manual(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    try {
        int fila_actual = 2;
        
        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "         CREAR BACKUP MANUAL                 " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual++);
        cout << "Creando backup manual de los datos del sistema..." << endl;
        fila_actual++;

        // Crear el backup usando RespaldoDatos
        RespaldoDatos::guardarRespaldoClientesConFecha(*banco.getClientes());
        
        mover_cursor(1, fila_actual++);
        cout << "==============================================" << endl;
        mover_cursor(1, fila_actual++);
        cout << "     BACKUP CREADO EXITOSAMENTE" << endl;
        mover_cursor(1, fila_actual++);
        cout << "Archivo: Backup con marca de tiempo creado" << endl;
        mover_cursor(1, fila_actual++);
        cout << "Ubicación: Directorio actual del proyecto" << endl;
        mover_cursor(1, fila_actual++);
        cout << "==============================================" << endl;
        fila_actual++;
        
        mover_cursor(1, fila_actual++);
        cout << "Presione Enter para regresar al menú de administrador...";
        pausar_consola();
    }
    catch (const std::exception& e) {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        cerr << "Error al crear backup manual: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\n=== ERROR AL CREAR BACKUP MANUAL ===" << endl;
        mover_cursor(1, fila_error++);
        cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        cout << "\nRegresando al menú de administrador...\n";
        pausar_consola();
    }
}

/**
 * @brief Verifica si existe el archivo datos.txt y maneja su recuperación.
 * @param banco Referencia al objeto Banco para cargar los datos.
 * @return true si los datos fueron cargados exitosamente, false en caso contrario.
 * @details Verifica la existencia de datos.txt y si no existe, presenta
 * un menú para seleccionar un backup para recuperar.
 */
bool verificar_y_recuperar_datos(Banco& banco) {
    ifstream archivo("datos.txt");
    
    // Si el archivo existe, intentar cargarlo
    if (archivo.good()) {
        archivo.close();
        try {
            banco.cargar_datos_binario("datos.txt");
            return true;
        }
        catch (const std::exception& e) {
            cout << "Error al cargar datos.txt: " << e.what() << endl;
            // Si hay error al cargar, continúa con la selección de backup
        }
    }
    
    // Si el archivo no existe o hubo error al cargarlo
    archivo.close();
    system("cls");
    visibilidad_cursor(true);
    
    cout << "==============================================" << endl;
    cout << "   ARCHIVO DATOS.TXT NO ENCONTRADO O DAÑADO  " << endl;
    cout << "==============================================" << endl;
    cout << endl;
    cout << "El archivo principal de datos no está disponible." << endl;
    cout << "Es necesario recuperar desde un backup existente." << endl;
    cout << endl;
    
    // Buscar archivos de backup disponibles
    vector<string> backups;
    for (const auto& entry : filesystem::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            string filename = entry.path().filename().string();
            if (filename.find("backup_clientes_") == 0 && filename.find(".bin") != string::npos) {
                backups.push_back(filename);
            }
        }
    }
    
    if (backups.empty()) {
        cout << "=== NO HAY BACKUPS DISPONIBLES ===" << endl;
        cout << "No se encontraron archivos de backup para recuperar." << endl;
        cout << "El sistema se iniciará sin datos." << endl;
        cout << endl;
        cout << "Presione Enter para continuar...";
        pausar_consola();
        return false;
    }
    
    // Ordenar backups por fecha (más reciente primero)
    sort(backups.begin(), backups.end(), [](const string& a, const string& b) {
        return a > b; // Orden descendente (más reciente primero)
    });
    
    // Crear opciones para el menú de selección con flechitas
    vector<string> opciones_str;
    vector<const char*> opciones_backup;
    
    for (size_t i = 0; i < backups.size(); ++i) {
        string opcion_texto = backups[i];
        
        // Extraer y mostrar fecha/hora del nombre del archivo
        size_t pos1 = backups[i].find("backup_clientes_");
        size_t pos2 = backups[i].find(".bin");
        if (pos1 != string::npos && pos2 != string::npos) {
            string timestamp = backups[i].substr(16, pos2 - 16);
            if (timestamp.length() >= 15) {
                string fecha = timestamp.substr(0, 8);
                string hora = timestamp.substr(9, 6);
                
                // Formatear fecha DD/MM/YYYY
                string fecha_formateada = fecha.substr(6, 2) + "/" + 
                                        fecha.substr(4, 2) + "/" + 
                                        fecha.substr(0, 4);
                
                // Formatear hora HH:MM:SS
                string hora_formateada = hora.substr(0, 2) + ":" + 
                                       hora.substr(2, 2) + ":" + 
                                       hora.substr(4, 2);
                
                opcion_texto += " [" + fecha_formateada + " " + hora_formateada + "]";
            }
        }
        
        opciones_str.push_back(opcion_texto);
        opciones_backup.push_back(opciones_str.back().c_str());
    }
    
    // Agregar opción de cancelar
    opciones_str.push_back("Cancelar y salir del programa");
    opciones_backup.push_back(opciones_str.back().c_str());
    
    // Usar el sistema de selección con flechitas
    system("cls");
    int seleccion = seleccionar_opcion("===== SELECCIONAR BACKUP PARA RECUPERAR =====", 
                                     opciones_backup.data(), 
                                     opciones_backup.size(), 4);
    
    // Si selecciona cancelar
    if (seleccion == static_cast<int>(backups.size())) {
        return false;
    }
    
    string archivo_seleccionado = backups[seleccion];
    cout << endl;
    cout << "Archivo seleccionado: " << archivo_seleccionado << endl;
    cout << "ADVERTENCIA: Esto sobrescribirá el archivo datos.txt actual." << endl;
    cout << "¿Está seguro de que desea recuperar este backup?" << endl;
    
    bool confirmar = seleccionar_Si_No();
    if (!confirmar) {
        cout << "Operación cancelada." << endl;
        pausar_consola();
        return false;
    }
    
    try {
        cout << "Recuperando backup..." << endl;
        
        // Cargar el backup seleccionado usando el método correcto
        ListaDoble<Cliente*>* clientes_recuperados = RespaldoDatos::restaurarClientesBinario(archivo_seleccionado);
        
        if (!clientes_recuperados) {
            throw runtime_error("Error al cargar el backup seleccionado");
        }
        
        // Asignar los clientes recuperados al banco
        banco.setClientes(clientes_recuperados);
        
        // Guardar como datos.txt
        banco.guardar_datos_binario("datos.txt");
        
        cout << "==============================================" << endl;
        cout << "     BACKUP RECUPERADO EXITOSAMENTE" << endl;
        cout << "==============================================" << endl;
        cout << "Los datos han sido restaurados desde: " << archivo_seleccionado << endl;
        cout << "El archivo datos.txt ha sido recreado." << endl;
        cout << "El sistema está listo para usar." << endl;
        cout << "==============================================" << endl;
        cout << endl;
        cout << "Presione Enter para continuar...";
        pausar_consola();
        
        return true;
    }
    catch (const std::exception& e) {
        cout << "Error al recuperar backup: " << e.what() << endl;
        cout << "Presione Enter para continuar...";
        pausar_consola();
        return false;
    }
}