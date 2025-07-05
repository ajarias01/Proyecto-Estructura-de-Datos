#include "Menus.h"
#include "Fecha.h"
#include "Ahorro.h"
#include "Corriente.h"
#include "validaciones.h"
#include "RespaldoDatos.h"
#include "GestorClientes.h"
#include "Marquesina.h"
#include "ArbolBinario.h"
#include "Ubicacion.h"
#include <stdexcept>
#include <conio.h>
#include <random>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>   
#include <thread>
#include <cmath>
#include <filesystem>

using namespace std;

// Variable global del sistema de geolocalización
SimpleGeolocationSystem geoSystem;

void pausar_consola()
{
    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void visibilidad_cursor(bool visible)
{
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = visible;
    SetConsoleCursorInfo(manejo_consola, &info);
}

void mover_cursor(int x, int y)
{
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(manejo_consola, pos);
}

int seleccionar_opcion(const char *titulo, const char *opciones[], int n, int fila_inicio) {
    system("chcp 65001 > nul"); 
    int opcionSeleccionada = 1; 
    int tecla;

    // Ajustar la fila de inicio para dejar espacio para la marquesina
    int fila_ajustada = std::max(fila_inicio, 3); // Asegurar que nunca sea menor que 3

    // Dibujar el título, alineado a la izquierda, en la fila especificada
    mover_cursor(2, fila_ajustada);
    cout << titulo;

    // Dibujar las opciones iniciales, empezando justo debajo del título
    for (int e = 0; e < n; e++) {
        mover_cursor(2, fila_ajustada + 2 + e); // +2 para dejar un espacio entre título y opciones
        // Limpiar la línea completa antes de escribir
        cout << string(80, ' ');
        mover_cursor(2, fila_ajustada + 2 + e);
        if (e + 1 == opcionSeleccionada) {
            cout << " ➤ " << e + 1 << " " << opciones[e];
        } else {
            cout << "   " << e + 1 << " " << opciones[e];
        }
    }

    do {
        tecla = getch();
        switch (tecla) {
            case TECLA_ARRIBA:
                // Limpiar y actualizar la opción anterior
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << string(80, ' ');
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << "   " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];

                opcionSeleccionada--;
                if (opcionSeleccionada < 1) opcionSeleccionada = n;

                // Limpiar y actualizar la nueva opción seleccionada
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << string(80, ' ');
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << " ➤ " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];
                break;

            case TECLA_ABAJO:
                // Limpiar y actualizar la opción anterior
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << string(80, ' ');
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << "   " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];

                opcionSeleccionada++;
                if (opcionSeleccionada > n) opcionSeleccionada = 1;

                // Limpiar y actualizar la nueva opción seleccionada
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << string(80, ' ');
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << " ➤ " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];
                break;
        }
    } while (tecla != ENTER);

    return opcionSeleccionada;
}

bool seleccionar_Si_No() {
    const int NUM_OPCIONES = 2;
    const char* OPCIONES[NUM_OPCIONES] = {
        "SÍ",
        "NO"
    };
    
    system("cls");
    ajustar_cursor_para_marquesina();
    int opcion = seleccionar_opcion("===== CONFIRMAR OPERACIÓN =====", OPCIONES, NUM_OPCIONES, 4);
    
    if (opcion == 1) {
        return true;
    } else {
        return false;
    }
}

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
        auto imprimir_boton = [ANCHO_BOTON](const char* opcion)
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
            color_act = (i * 3) + 3 == cursor_y ? 1 : 0; // Ajustar para empezar desde línea 3
            mover_cursor(cursor_x, (i * 3) + 3); // Ajustar posición Y para dejar espacio a la marquesina
            SetConsoleTextAttribute(manejo_consola, colores[color_act]);
            imprimir_boton(opciones[i]);
        }
        // Restaurar colores por defecto y asegurar que el cursor esté en posición correcta
        SetConsoleTextAttribute(manejo_consola, 15); // Color blanco por defecto
        std::cout.flush(); // Forzar el vaciado del buffer
    };

    imprimir_opciones();
    while ((tecla = getch()) != 13)
    {
        if (tecla == 72 && opcion > 1) cursor_y -= 3;
        else if (tecla == 80 && opcion < nopciones) cursor_y += 3;
        imprimir_opciones();
        mover_cursor(cursor_x, cursor_y);
        opcion = ((cursor_y - 3) / 3) + 1; // Ajustar cálculo para empezar desde línea 3
    }
    
    return opcion;
}

bool validar_credenciales_administrador(const string& usuario, const string& contrasenia) {
    const string USUARIO_ADMIN = "admin";
    const string CONTRASENIA_ADMIN = "1234567890";
    return usuario == USUARIO_ADMIN && contrasenia == CONTRASENIA_ADMIN;
}

void recuperar_backup_por_fecha(Banco& banco)
{
    int horas = 0, minutos = 0, segundos = 0;
    string fecha_hora,backup;
    system("cls");
    visibilidad_cursor(true);
    try
    {
        Fecha fecha;
        // Validar fecha
        do {
            std::cout << std::endl;
            limpiar_linea("➤ Ingrese la fecha del backup (DD/MM/YYYY): ");
            fecha = validarFecha("");
            if (fecha.get_dia() == -1) return;
        } while (!fecha.validar());
        std::cout << std::endl;

        // Validar hora
        do {
            limpiar_linea("➤ Ingrese la hora del backup (HH:MM:SS): ");
            fecha_hora = validarHora("");
            if (fecha_hora == "__ESC__") return;
            horas = stoi(fecha_hora.substr(0, 2));
            minutos = stoi(fecha_hora.substr(2, 2));
            segundos = stoi(fecha_hora.substr(4, 2));
        } while (!validar_hora_minuto_segundo(horas, minutos, segundos));
        cout << endl;

        // Formato de nombre: backup_YYYYMMDD_HHMMSS.bin
        char buffer[100];
        // Usar guiones en lugar de espacios para compatibilidad
        snprintf(buffer, sizeof(buffer), "backup_clientes_%04d%02d%02d_%02d%02d%02d.bin",
                fecha.get_anuario(), fecha.get_mes(), fecha.get_dia(),
                horas, minutos, segundos);
        std::string nombre_archivo = buffer;

        // Validar si el archivo existe
        std::ifstream test_file(nombre_archivo, std::ios::binary);
        if (!test_file.good()) {
            std::cout << "\n=== ERROR: El backup no existe para esa fecha y hora ===" << std::endl;
            pausar_consola();
            return;
        }
        test_file.close();

        std::cout << "\nIntentando recuperar backup: " << nombre_archivo << std::endl;
        this_thread::sleep_for(std::chrono::seconds(3));
        ListaDoble<Cliente*>* nuevos_clientes = RespaldoDatos::restaurarClientesBinario(nombre_archivo);
        if (!nuevos_clientes || nuevos_clientes->esta_vacia()) {
            std::cout << "\n=== ERROR: No se pudieron cargar datos del backup ===" << std::endl;
            pausar_consola();
            return;
        }

        // Actualizar el banco con los nuevos clientes
        if (banco.getClientes()) {
            delete banco.getClientes();
        }
        banco.setClientes(nuevos_clientes);

        banco.guardar_datos_binario_sin_backup("datos.bin");

        std::cout << "\n=== BACKUP MÁS RECIENTE RECUPERADO EXITOSAMENTE ===" << std::endl;
        std::cout << "Datos cargados desde: " << nombre_archivo << std::endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error al recuperar backup: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL RECUPERAR BACKUP ===" << std::endl;
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void cargar_base_datos(Banco& banco) {
    system("cls");
    auto* clientes = banco.getClientes();
    if (!clientes || clientes->esta_vacia()) {
        cout << "No hay clientes registrados en la base de datos.\n";
        pausar_consola();
        return;
    }

    // Menú de selección de campo
    ajustar_cursor_para_marquesina();
    const int NUM_CAMPOS = 5;
    const char* CAMPOS[NUM_CAMPOS] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    int campo = seleccionar_opcion("===== SELECCIONAR CAMPO =====", CAMPOS, NUM_CAMPOS, 4);

    // Menú de selección de orden
    system("cls");
    ajustar_cursor_para_marquesina();
    const int NUM_ORDENES = 2;
    const char* ORDENES[NUM_ORDENES] = {"Ascendente", "Descendente"};
    int orden = seleccionar_opcion("===== SELECCIONAR ORDEN =====", ORDENES, NUM_ORDENES, 4);

    // Convertir lista a vector para ordenamiento más eficiente
    auto& lista = *clientes;
    int n = lista.getTam();
    std::vector<Cliente*> vectorClientes;
    for (int i = 0; i < n; ++i) {
        vectorClientes.push_back(lista.get_contador(i));
    }

    // Función de comparación personalizada según el campo seleccionado
    auto comparar = [campo, orden](Cliente* a, Cliente* b) -> bool {
        bool resultado;
        switch (campo) {
            case 1: // DNI
                resultado = std::stoi(a->get_dni()) < std::stoi(b->get_dni());
                break;
            case 2: { // Nombre
                string nombre_a = a->get_nombres();
                string nombre_b = b->get_nombres();
                // Convertir a minúsculas para comparación insensible a mayúsculas
                transform(nombre_a.begin(), nombre_a.end(), nombre_a.begin(), ::tolower);
                transform(nombre_b.begin(), nombre_b.end(), nombre_b.begin(), ::tolower);
                resultado = nombre_a < nombre_b;
                break;
            }
            case 3: { // Apellido
                string apellido_a = a->get_apellidos();
                string apellido_b = b->get_apellidos();
                // Convertir a minúsculas para comparación insensible a mayúsculas
                transform(apellido_a.begin(), apellido_a.end(), apellido_a.begin(), ::tolower);
                transform(apellido_b.begin(), apellido_b.end(), apellido_b.begin(), ::tolower);
                resultado = apellido_a < apellido_b;
                break;
            }
            case 4: // Teléfono
                resultado = std::stoi(a->get_telefono()) < std::stoi(b->get_telefono());
                break;
            case 5: { // Email
                string email_a = a->get_email();
                string email_b = b->get_email();
                // Convertir a minúsculas para comparación insensible a mayúsculas
                transform(email_a.begin(), email_a.end(), email_a.begin(), ::tolower);
                transform(email_b.begin(), email_b.end(), email_b.begin(), ::tolower);
                resultado = email_a < email_b;
                break;
            }
            default:
                resultado = false;
        }
        // Si es orden descendente, invertir el resultado
        return (orden == 1) ? resultado : !resultado;
    };

    // Ordenar usando std::sort con comparador personalizado
    std::sort(vectorClientes.begin(), vectorClientes.end(), comparar);

    // Devolver los elementos ordenados a la lista
    for (int i = 0; i < n; ++i) {
        lista.set_contador(i, vectorClientes[i]);
    }

    // DETENER LA MARQUESINA ANTES DE MOSTRAR LA TABLA
    detener_marquesina();
    
    // Mostrar tabla alineada
    system("cls");
    cout << "\n\t\t===========================================" << std::endl;
    cout << "\t\t===    LISTADO DE CLIENTES Y CUENTAS    ===" << std::endl;
    cout << "\t\t===========================================" << std::endl;
    cout << std::left
            << "| " << std::setw(4)  << "#" 
            << "| " << std::setw(12) << "DNI" 
            << "| " << std::setw(15) << "Nombre" 
            << "| " << std::setw(15) << "Apellido" 
            << "| " << std::setw(12) << "Teléfono" 
            << "| " << std::setw(25) << "Email" 
            << "|" << std::endl;
    cout << "---------------------------------------------------------------" 
            << "---------------------" << std::endl;
    for (int i = 0; i < n; ++i) {
        Cliente* cliente = lista.get_contador(i);
        std::cout << std::left
                << "| " << std::setw(4)  << (i + 1)
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

    // --- BÚSQUEDA DE CLIENTE POR CÉDULA ---
    string respuesta, cedula_buscar;
    do {
        limpiar_linea("➤ ¿Desea buscar un cliente específico? (S/N): ");
        respuesta = ingresar_alfabetico("");
        if (respuesta == "__ESC__") return;
        transform(respuesta.begin(), respuesta.end(), respuesta.begin(), ::toupper);
    } while (respuesta != "S" && respuesta != "N");
    
    cout << endl;
    if (respuesta == "S") {
        do {
            limpiar_linea("➤ Ingrese la cédula a buscar: ");
            cedula_buscar = ingresar_dni("");
            if (cedula_buscar == "__ESC__") return;
        } while (!validarCedulaEcuatoriana(cedula_buscar));
        
        // Buscar cliente por cédula
        Cliente* clienteEncontrado = nullptr;
        for (int i = 0; i < n; ++i) {
            if (vectorClientes[i]->get_dni() == cedula_buscar) {
                clienteEncontrado = vectorClientes[i];
                break;
            }
        }

        if (clienteEncontrado) {
            std::cout << "\n=== CLIENTE ENCONTRADO ===\n";
            std::cout << "DNI: " << clienteEncontrado->get_dni() << "\n";
            std::cout << "Nombre: " << clienteEncontrado->get_nombres() << "\n";
            std::cout << "Apellido: " << clienteEncontrado->get_apellidos() << "\n";
            std::cout << "Teléfono: " << clienteEncontrado->get_telefono() << "\n";
            std::cout << "Email: " << clienteEncontrado->get_email() << "\n";
            
            // Mostrar las cuentas del cliente
            auto* cuentas = clienteEncontrado->get_cuentas();
            if (cuentas && !cuentas->esta_vacia()) {
                std::cout << "\n=== CUENTAS DEL CLIENTE ===\n";
                std::cout << std::left << std::setw(15) << "ID Cuenta" 
                         << std::setw(12) << "Tipo" 
                         << std::setw(15) << "Saldo" 
                         << std::setw(20) << "Fecha Apertura" << std::endl;
                std::cout << "--------------------------------------------------------------\n";
                
                cuentas->recorrer([](Cuenta* cuenta) {
                    std::cout << std::left << std::setw(15) << cuenta->get_id_cuenta()
                             << std::setw(12) << cuenta->get_tipo()
                             << std::setw(15) << ("$" + std::to_string(cuenta->get_saldo()))
                             << std::setw(20) << cuenta->get_fecha_apertura().to_string() << std::endl;
                });
                std::cout << "--------------------------------------------------------------\n";
            } else {
                std::cout << "\nEste cliente no tiene cuentas registradas.\n";
            }
        } else {
            std::cout << "\nNo se encontró ningún cliente con esa cédula.\n";
        }
    }

    cout << "\nPresione cualquier tecla para continuar...";
    getch();
    
    // REINICIAR LA MARQUESINA AL SALIR
    inicializar_marquesina();
}

void descifrar_archivos_txt(Banco& banco) {
    system("cls");    
    RespaldoDatos respaldo;
    respaldo.seleccionarYDescifrarTxt();
}

void cifrar_archivos_txt(Banco& banco) {
    RespaldoDatos respaldo;
    system("cls");
    visibilidad_cursor(true);
    std::cout << "\n===========================================" << std::endl;
    std::cout << "===    CIFRAR ARCHIVO DE RESPALDO    ===" << std::endl;
    std::cout << "===========================================" << std::endl;


    // 1. Buscar todos los backups disponibles
    std::vector<std::string> backups;
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        std::string nombre = entry.path().filename().string();
        if (nombre.find("backup_clientes_") == 0 && nombre.substr(nombre.size() - 4) == ".bin") {
            backups.push_back(nombre);
        }
    }

    if (backups.empty()) {
        std::cout << "\nNo hay archivos de respaldo (.bin) para cifrar.\n";
        pausar_consola();
        return;
    }

    // 2. Mostrar lista para seleccionar
    std::cout << "\n=== SELECCIONE EL ARCHIVO DE RESPALDO A CIFRAR ===\n";
    for (size_t i = 0; i < backups.size(); ++i) {
        std::cout << (i + 1) << ". " << backups[i] << std::endl;
    }

    int seleccion = 0;
    std::string numero;
    do {
        limpiar_linea("Seleccione el archivo a cifrar (1-" + std::to_string(backups.size()) + "): ");
        numero = ingresar_dni("");
        if (numero == "__ESC__") return;
        if (!numero.empty()) {
            try {
                seleccion = std::stoi(numero);
            } catch (...) {
                seleccion = 0;
            }
        }
    } while (seleccion < 1 || seleccion > static_cast<int>(backups.size()));
    cout << endl;
    std::string nombre_archivo = backups[seleccion - 1];

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Archivo seleccionado: " << nombre_archivo << std::endl;
    std::cout << "Iniciando proceso de cifrado..." << std::endl;
    int numCesar = 3;
    respaldo.cifrarArchivoABaseTxt(nombre_archivo, numCesar);
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "=== ¡CIFRADO COMPLETADO CON ÉXITO! ===" << std::endl;
    // Mostrar nombre del archivo cifrado
    size_t pos1 = nombre_archivo.find("backup_clientes_");
    size_t pos2 = nombre_archivo.find(".bin");
    std::string timestamp = (pos1 != std::string::npos && pos2 != std::string::npos)
        ? nombre_archivo.substr(16, pos2 - 16)
        : "error_timestamp";
    std::cout << "El archivo ha sido cifrado y guardado como: cifrado_" << timestamp << ".txt" << std::endl;
    std::cout << "===========================================" << std::endl;

    pausar_consola();
}

void menu_administrador(Banco& banco)
{
    const int NUM_OPCIONES = 8;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Consultar movimientos por fecha",
        "Consultar cuentas por DNI/nombre",
        "Base de datos",
        "Árbol binario",
        "Recuperar backup por fecha y hora",
        "Cifrar archivos",
        "Descifrar archivos",
        "Salir"
    };

    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);
    std::string usuario, contrasenia;
    try
    {
        std::cout << "\n\t\t===========================================" << std::endl;
        std::cout << "\t\t==========     ADMINISTRADOR     ==========" << std::endl;
        std::cout << "\t\t===========================================" << std::endl;
       do {
            limpiar_linea("➤ Ingrese el usuario del administrador: ");
            usuario = ingresar_alfabetico("");
            if (usuario == "__ESC__") return;
        } while (usuario.empty() || usuario.length() < 4);
        cout << endl;
        do {
            limpiar_linea("➤ Ingrese la contraseña del administrador: ");
            contrasenia = ingresar_contrasenia_administrador("");
            if (contrasenia == "__ESC__") return;
        } while (contrasenia.empty());
        cout << endl;

        if (!validar_credenciales_administrador(usuario, contrasenia))
        {
            throw std::runtime_error("Usuario o contraseña incorrectos");
        }

        int opcion;
        do
        {
            system("cls");
            ajustar_cursor_para_marquesina();
            opcion = seleccionar_opcion("===== MENÚ ADMINISTRADOR =====", OPCIONES, NUM_OPCIONES, 4);
            switch (opcion)
            {
                case 1:
                    consultar_movimientos(banco);
                    break;
                case 2:
                    consultar_cuentas(banco);
                    break;
                case 3:
                    cargar_base_datos(banco);
                    break;
                case 4:
                    mostrar_arbol_binario(banco);
                    break;
                case 5:
                    recuperar_backup_por_fecha(banco);
                    break;
                case 6:
                    cifrar_archivos_txt(banco);
                    break;
                case 7:
                    descifrar_archivos_txt(banco);
                    break;
                case 8:
                    return;
            }
        } while (opcion != NUM_OPCIONES);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error en autenticación: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL ACCEDER AL MENÚ DE ADMINISTRADOR ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void menu_cliente(Banco& banco)
{
    const int NUM_OPCIONES = 3;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Realizar depósito",
        "Realizar retiro",
        "Salir"
    };

    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);
    std::string dni, contrasenia;
    try
    {

        // Validación de DNI
        do{
            limpiar_linea("➤ Ingrese su DNI: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__") return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;

        // Validación de contraseña
        do
        {
            limpiar_linea("➤ Ingrese su contraseña: ");
            contrasenia = ingresar_contrasenia("");
            if (contrasenia == "__ESC__") return;
        } while (!validar_contrasenia(contrasenia));
        cout << endl;

        // Verificar cliente y contraseña
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente)
        {
            throw std::runtime_error("Cliente no encontrado");
        }
        if (cliente->get_contrasenia() != contrasenia)
        {
            throw std::runtime_error("Contraseña incorrecta");
        }

        // Mostrar menú cliente si la autenticación es exitosa
        int opcion;
        do
        {
            system("cls");
            ajustar_cursor_para_marquesina();
            opcion = seleccionar_opcion("===== MENÚ CLIENTE =====", OPCIONES, NUM_OPCIONES, 4);
            switch (opcion)
            {
                case 1:
                    realizar_deposito(banco, dni);
                    break;
                case 2:
                    realizar_retiro(banco, dni);
                    break;
                case 3:
                    return;
            }
        } while (opcion != NUM_OPCIONES);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error al iniciar sesión: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL INICIAR SESIÓN ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void menu_principal(Banco& banco)
{
    // Inicializar la marquesina al comenzar el programa
    inicializar_marquesina();
    
    const int NUM_OPCIONES = 5;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Crear cuenta",
        "Iniciar sesión",
        "Administrador",
        "Ayuda Técnica",
        "Salir"
    };

    int opcion;
    do
    {
        system("cls");
        ajustar_cursor_para_marquesina();
        opcion = seleccionar_opcion("===== MENU PRINCIPAL =====", OPCIONES, NUM_OPCIONES, 4);
        switch (opcion)
        {
            case 1:
                menu_cuenta(banco);
                break;
            case 2:
                menu_cliente(banco);
                break;
            case 3:
                menu_administrador(banco);
                break;
            case 4:
                mostrar_ayuda_tecnica();
                break;
            case 5:
                detener_marquesina();
                return;
        }
    } while (opcion != NUM_OPCIONES);
}

void mostrar_ayuda_tecnica()
{
    system("cls");
    visibilidad_cursor(true);
    try
    {   
        std::cout << std::endl;
        std::cout << "Abriendo el Manual de Ayuda Técnica...\n";
        this_thread::sleep_for(std::chrono::seconds(3));
        HINSTANCE result = ShellExecute(NULL, "open", "AyudaTecnicaBanco.chm", NULL, NULL, SW_SHOWNORMAL);
        if (result <= (HINSTANCE)32)
        {
            throw std::runtime_error("No se pudo abrir el archivo de ayuda. Asegúrese de que 'AyudaTecnicaBanco.chm' esté en el directorio del programa.");
        }
        std::cout << "Manual de Ayuda Técnica abierto exitosamente.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL ABRIR AYUDA TÉCNICA ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
    }
    pausar_consola();
}

// ... (mantén todas las funciones anteriores hasta menu_cuenta sin cambios)


void abrir_cuenta_sin_sucursal(Banco& banco, int tipo_cuenta) {
    string dni, nombre, apellido, direccion, telefono, email, depositar_inicial, saldo_inicial1, contrasenia;
    Fecha fecha_nacimiento;
    double saldo_inicial = 0;
    visibilidad_cursor(true);
    system("cls");
    ajustar_cursor_para_marquesina();
    try {
        int fila_actual = 4; // Ajustar para dejar espacio para la marquesina

        do {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el DNI del cliente: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__") return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        fila_actual += 2;

        Cliente* cliente_existe = banco.buscar_cliente(dni);
        bool ya_tiene = false;
        if (cliente_existe) {
            cliente_existe->get_cuentas()->recorrer([&](Cuenta* cuenta) {
                if ((tipo_cuenta == 1 && cuenta->get_tipo() == "Ahorros") ||
                    (tipo_cuenta == 2 && cuenta->get_tipo() == "Corriente")) {
                    ya_tiene = true;
                }
            });
            if (ya_tiene) {
                throw std::runtime_error("El cliente con DNI " + dni + " ya tiene una cuenta de este tipo.");
            }
        }

        if (cliente_existe) {
            nombre = cliente_existe->get_nombres();
            apellido = cliente_existe->get_apellidos();
            direccion = cliente_existe->get_direccion();
            telefono = cliente_existe->get_telefono();
            email = cliente_existe->get_email();
            fecha_nacimiento = cliente_existe->get_fecha_nacimiento();
            contrasenia = cliente_existe->get_contrasenia();
        } else {
            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el nombre del cliente: ");
                nombre = ingresar_alfabetico("");
                if (nombre == "__ESC__") return;
            } while (nombre.empty() || nombre.length() < 3);
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el apellido del cliente: ");
                apellido = ingresar_alfabetico("");
                if (apellido == "__ESC__") return;
            } while (apellido.empty() || apellido.length() < 3);
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la dirección del cliente: ");
                direccion = ingresar_direccion("");
                if (direccion == "__ESC__") return;
            } while (direccion.empty() || direccion.length() < 5);
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el teléfono del cliente: ");
                telefono = ingresar_dni("");
                if (telefono == "__ESC__") return;
            } while (!validar_telefono(telefono) || telefono_existe(banco, telefono));
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el email del cliente: ");
                email = ingresar_email("");
                if (email == "__ESC__") return;
            } while (!validar_email(email) || email_existe(banco, email));
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la fecha de nacimiento (DD/MM/YYYY): ");
                fecha_nacimiento = validarFecha("");
            } while (fecha_nacimiento.empty());
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la contraseña (mínimo 20 caracteres, al menos una mayúscula, minúscula, dígito y carácter especial): ");
                contrasenia = ingresar_contrasenia("");
                if (contrasenia == "__ESC__") return;
            } while (!validar_contrasenia(contrasenia));
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ ¿Desea depositar un monto inicial? (S/N): ");
                depositar_inicial = ingresar_alfabetico("");
                if (depositar_inicial == "__ESC__") return;
                transform(depositar_inicial.begin(), depositar_inicial.end(), depositar_inicial.begin(), ::toupper);
            } while (depositar_inicial != "S" && depositar_inicial != "N");
            cout << endl;
            fila_actual += 2;

            if (depositar_inicial == "S") {
                do {
                    mover_cursor(1, fila_actual);
                    limpiar_linea("➤ Ingrese el saldo inicial: ");
                    saldo_inicial1 = ingresar_decimales("");
                    if (saldo_inicial1 == "__ESC__") return;
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
        if (!resultado) {
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
        Cuenta* cuenta = nullptr;
        if (tipo_cuenta == 1) {
            mover_cursor(1, fila_actual + 4);
            cout << "La tasa de interés para la cuenta de ahorros es 5%" << endl;
            double tasa_interes = 5.0;
            cuenta = new Ahorro(id_cuenta, saldo_inicial, fecha_apertura, tasa_interes);
            fila_actual += 5;
        } else if (tipo_cuenta == 2) {
            int limite_retiro_diario;
            do {
                mover_cursor(1, fila_actual + 4);
                limpiar_linea("!!!Ingrese el límite de retiro diario: ");
                limite_retiro_diario = ingresar_enteros("");
            } while (limite_retiro_diario <= 0);
            cout << endl;
            cuenta = new Corriente(id_cuenta, saldo_inicial, fecha_apertura, limite_retiro_diario);
            fila_actual += 5;
        }

        if (!cuenta) {
            throw std::runtime_error("Error al crear la cuenta");
        }

        if (cliente_existe) {
            cliente_existe->agregar_cuenta(cuenta);
            banco.guardar_datos_binario("datos.bin");
            mover_cursor(1, fila_actual);
            cout << "=== CUENTA CREADA EXITOSAMENTE ===" << endl;
            mover_cursor(1, fila_actual + 1);
            cout << "ID de cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual + 2);
            cout << "Tipo: " << (tipo_cuenta == 1 ? "Cuenta de Ahorros" : "Cuenta Corriente") << endl;
            mover_cursor(1, fila_actual + 3);
            cout << "¡Cuenta creada vía aplicación móvil!" << endl;
        } else {
            Cliente* cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fecha_nacimiento, contrasenia);
            cliente->agregar_cuenta(cuenta);
            banco.agregar_cliente(cliente);
            banco.guardar_datos_binario("datos.bin");
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
    } catch (const std::exception& e) {
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

void abrir_cuenta(Banco& banco, int tipo_cuenta, int branchId, const string& sucursal) {
    string dni, nombre, apellido, direccion, telefono, email, depositar_inicial, saldo_inicial1, contrasenia;
    Fecha fecha_nacimiento;
    double saldo_inicial = 0;
    visibilidad_cursor(true);
    system("cls");
    ajustar_cursor_para_marquesina();
    try {
        int fila_actual = 4; // Ajustar para dejar espacio para la marquesina

        do {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el DNI del cliente: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__") return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        fila_actual += 2;

        Cliente* cliente_existe = banco.buscar_cliente(dni);
        bool ya_tiene = false;
        if (cliente_existe) {
            cliente_existe->get_cuentas()->recorrer([&](Cuenta* cuenta) {
                if ((tipo_cuenta == 1 && cuenta->get_tipo() == "Ahorros") ||
                    (tipo_cuenta == 2 && cuenta->get_tipo() == "Corriente")) {
                    ya_tiene = true;
                }
            });
            if (ya_tiene) {
                throw std::runtime_error("El cliente con DNI " + dni + " ya tiene una cuenta de este tipo.");
            }
        }

        if (cliente_existe) {
            nombre = cliente_existe->get_nombres();
            apellido = cliente_existe->get_apellidos();
            direccion = cliente_existe->get_direccion();
            telefono = cliente_existe->get_telefono();
            email = cliente_existe->get_email();
            fecha_nacimiento = cliente_existe->get_fecha_nacimiento();
            contrasenia = cliente_existe->get_contrasenia();
        } else {
            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el nombre del cliente: ");
                nombre = ingresar_alfabetico("");
                if (nombre == "__ESC__") return;
            } while (nombre.empty() || nombre.length() < 3);
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el apellido del cliente: ");
                apellido = ingresar_alfabetico("");
                if (apellido == "__ESC__") return;
            } while (apellido.empty() || apellido.length() < 3);
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la dirección del cliente: ");
                direccion = ingresar_direccion("");
                if (direccion == "__ESC__") return;
            } while (direccion.empty() || direccion.length() < 5);
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el teléfono del cliente: ");
                telefono = ingresar_dni("");
                if (telefono == "__ESC__") return;
            } while (!validar_telefono(telefono) || telefono_existe(banco, telefono));
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese el email del cliente: ");
                email = ingresar_email("");
                if (email == "__ESC__") return;
            } while (!validar_email(email) || email_existe(banco, email));
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la fecha de nacimiento (DD/MM/YYYY): ");
                fecha_nacimiento = validarFecha("");
            } while (fecha_nacimiento.empty());
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ Ingrese la contraseña (mínimo 20 caracteres, al menos una mayúscula, minúscula, dígito y carácter especial): ");
                contrasenia = ingresar_contrasenia("");
                if (contrasenia == "__ESC__") return;
            } while (!validar_contrasenia(contrasenia));
            cout << endl;
            fila_actual += 2;

            do {
                mover_cursor(1, fila_actual);
                limpiar_linea("➤ ¿Desea depositar un monto inicial? (S/N): ");
                depositar_inicial = ingresar_alfabetico("");
                if (depositar_inicial == "__ESC__") return;
                transform(depositar_inicial.begin(), depositar_inicial.end(), depositar_inicial.begin(), ::toupper);
            } while (depositar_inicial != "S" && depositar_inicial != "N");
            cout << endl;
            fila_actual += 2;

            if (depositar_inicial == "S") {
                do {
                    mover_cursor(1, fila_actual);
                    limpiar_linea("➤ Ingrese el saldo inicial: ");
                    saldo_inicial1 = ingresar_decimales("");
                    if (saldo_inicial1 == "__ESC__") return;
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
        if (!resultado) {
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
        Cuenta* cuenta = nullptr;
        if (tipo_cuenta == 1) {
            mover_cursor(1, fila_actual + 4);
            cout << "La tasa de interés para la cuenta de ahorros es 5%" << endl;
            double tasa_interes = 5.0;
            cuenta = new Ahorro(id_cuenta, saldo_inicial, fecha_apertura, tasa_interes);
            fila_actual += 5;
        } else if (tipo_cuenta == 2) {
            int limite_retiro_diario;
            do {
                mover_cursor(1, fila_actual + 4);
                limpiar_linea("!!!Ingrese el límite de retiro diario: ");
                limite_retiro_diario = ingresar_enteros("");
            } while (limite_retiro_diario <= 0);
            cout << endl;
            cuenta = new Corriente(id_cuenta, saldo_inicial, fecha_apertura, limite_retiro_diario);
            fila_actual += 5;
        }

        if (!cuenta) {
            throw std::runtime_error("Error al crear la cuenta");
        }

        cuenta->set_branchId(branchId);
        auto appointmentTime = banco.findNextAvailableSlot(branchId);
        cuenta->set_appointmentTime(appointmentTime);
        banco.addAppointment(branchId, appointmentTime);

        if (cliente_existe) {
            cliente_existe->agregar_cuenta(cuenta);
            banco.guardar_datos_binario("datos.bin");
            mover_cursor(1, fila_actual);
            cout << "=== CUENTA CREADA EXITOSAMENTE ===" << endl;
            mover_cursor(1, fila_actual + 1);
            cout << "ID de cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual + 2);
            cout << "Sucursal: " << sucursal << endl; 
            time_t tt = std::chrono::system_clock::to_time_t(appointmentTime);
            tm local_tm = *localtime(&tt);
            cout << "Cita asignada: " << local_tm.tm_mday << "/" << local_tm.tm_mon + 1 << "/" << local_tm.tm_year + 1900
                 << " " << local_tm.tm_hour << ":" << (local_tm.tm_min < 10 ? "0" : "") << local_tm.tm_min << endl;
        } else {
            Cliente* cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fecha_nacimiento, contrasenia);
            cliente->agregar_cuenta(cuenta);
            banco.agregar_cliente(cliente);
            banco.guardar_datos_binario("datos.bin");
            mover_cursor(1, fila_actual);
            cout << "=== CUENTA CREADA EXITOSAMENTE ===" << endl;
            mover_cursor(1, fila_actual + 1);
            cout << "ID de cuenta: " << id_cuenta << endl;
            mover_cursor(1, fila_actual + 2);
            cout << "Sucursal: " << sucursal << endl; 
            time_t tt = std::chrono::system_clock::to_time_t(appointmentTime);
            tm local_tm = *localtime(&tt);
            cout << "Cita asignada: " << local_tm.tm_mday << "/" << local_tm.tm_mon + 1 << "/" << local_tm.tm_year + 1900
                 << " " << local_tm.tm_hour << ":" << (local_tm.tm_min < 10 ? "0" : "") << local_tm.tm_min << endl;
        }
    } catch (const std::exception& e) {
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

// Función auxiliar isWorkingHour (puedes moverla a un archivo separado)
//bool isWorkingHour(const std::chrono::system_clock::time_point& tp) {
//    time_t tt = std::chrono::system_clock::to_time_t(tp);
//    tm local_tm = *localtime(&tt);
//    int hour = local_tm.tm_hour;
//    int wday = local_tm.tm_wday; // 0 = domingo, 1 = lunes, etc.
//    return (wday >= 1 && wday <= 5 && hour >= 9 && hour < 17);
//}

void realizar_deposito(Banco& banco, const std::string& dni)
{
    string salario, id_cuenta;
    double monto = 0;
    system("cls");
    visibilidad_cursor(true);
    try
    {
        int fila_actual = 2;
        // Encabezado bonito
        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "         DEPÓSITO EN CUENTA BANCARIA         " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        // Obtener cliente autenticado
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");

        // Mostrar datos del cliente
        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        // Validación de ID de cuenta
        do {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el ID de la cuenta: ");
            id_cuenta = ingresar_id("");
            if (id_cuenta == "__ESC__") return;
        } while (!validar_id_cuenta(cliente, id_cuenta));
        cout << endl;
        fila_actual += 2;
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);

        // Mostrar saldo actual
        mover_cursor(1, fila_actual++);
        cout << "Saldo actual: $" << cuenta->get_saldo() << endl;
        fila_actual++;

        // Validación de monto
        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el monto a depositar: ");
            salario = ingresar_decimales("");
            if (salario == "__ESC__") return;
        } while (!validar_monto(salario));
        monto = stod(salario);
        cout << endl;
        fila_actual += 2;

        // Confirmación
        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea realizar el depósito?" << endl;
        fila_actual += 1; // Avanzar una fila para el título del menú

        // Confirmación de depósito
        bool resultado = seleccionar_Si_No();
        if (!resultado) {
            cout << "Operación cancelada. No se realizó el depósito." << endl;
            cout << "Presione Enter para regresar al menú principal...";
            cin.get();
            return;
        }
        Fecha fecha;
        cuenta->depositar(monto, fecha);
        banco.guardar_datos_binario("datos.bin");
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
        cout << "   Datos guardados en datos.bin" << endl;
        mover_cursor(1, fila_actual++);
        cout << "==============================================" << endl;
        mover_cursor(1, fila_actual++);
        cout << "Regresando al menú cliente..." << endl;
        pausar_consola();
    }
    catch (const std::exception& e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        std::cerr << "Error al realizar depósito: " << e.what() << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "\n=== ERROR AL REALIZAR DEPÓSITO ===" << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "Error: " << e.what() << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "\nRegresando al menú cliente...\n";
        pausar_consola();
    }
}

void realizar_retiro(Banco& banco, const std::string& dni)
{
    system("cls");
    visibilidad_cursor(true);
    std::string salario, id_cuenta;
    double monto = 0;
    try
    {
        int fila_actual = 2;
        // Encabezado bonito
        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "           RETIRO DE CUENTA BANCARIA         " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        // Obtener cliente autenticado
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");

        // Mostrar datos del cliente
        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        // Validación de ID de cuenta
        do {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el ID de la cuenta: ");
            id_cuenta = ingresar_id("");
            if (id_cuenta == "__ESC__") return;
        } while (!validar_id_cuenta(cliente, id_cuenta));
        cout << endl;
        fila_actual += 2;
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);

        // Mostrar saldo actual
        mover_cursor(1, fila_actual++);
        cout << "Saldo actual: $" << cuenta->get_saldo() << endl;
        fila_actual++;

        // Validación de monto
        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el monto a retirar: ");
            salario = ingresar_decimales("");
            if (salario == "__ESC__") return;
        } while (!validar_monto(salario));
        monto = stod(salario);
        cout << endl;
        fila_actual += 2;

        // Confirmación
        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea realizar el retiro?" << endl;
        fila_actual += 1; // Avanzar una fila para el título del menú

        // Confirmación de retiro
        bool resultado = seleccionar_Si_No();
        if (!resultado) {
            cout << "Operación cancelada. No se realizó el retiro." << endl;
            cout << "Presione Enter para regresar al menú principal...";
            cin.get();
            return;
        }

        Fecha fecha;
        if (cuenta->retirar(monto, fecha))
        {
            banco.guardar_datos_binario("datos.bin");
            RespaldoDatos::respaldoClientesBinario("respaldo_clientes.bin", *banco.getClientes());
            fila_actual+=4;
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
            cout << "     Datos guardados en datos.bin" << endl;
            mover_cursor(1, fila_actual++);
            cout << "==============================================" << endl;
            mover_cursor(1, fila_actual++);
            cout << "Regresando al menú cliente..." << endl;
        }
        else
        {
            fila_actual+=4;
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
    catch (const std::exception& e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        std::cerr << "Error al realizar retiro: " << e.what() << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "\n=== ERROR AL REALIZAR RETIRO ===" << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "Error: " << e.what() << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "\nRegresando al menú cliente...\n";
        pausar_consola();
    }
}

void consultar_movimientos(Banco& banco)
{
    system("cls");
    visibilidad_cursor(true);
    try
    {
        int fila_actual = 2;
        std::string dni;
        Fecha fecha_inicio, fecha_fin;

        // Encabezado bonito
        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "      CONSULTA DE MOVIMIENTOS POR FECHA      " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        // Validación de DNI (obligatorio)
        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el DNI del cliente: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__") return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        fila_actual += 2;

        // Verificar si el cliente existe
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente con DNI " + dni + " no encontrado");

        // Mostrar datos del cliente
        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        // Validación de fecha de inicio
        do
        {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese la fecha de inicio (DD/MM/YYYY): ");
            fecha_inicio = validarFecha("");
        } while (fecha_inicio.empty());
        cout << endl;
        fila_actual += 2;

        // Validación de fecha de fin
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
        this_thread::sleep_for(std::chrono::seconds(3));
        
        // DETENER LA MARQUESINA ANTES DE CONSULTAR
        detener_marquesina();
        
        banco.consultar_movimientos_rango(dni, fecha_inicio, fecha_fin);
        getch();
        
        // REINICIAR LA MARQUESINA AL SALIR
        inicializar_marquesina();
        return;
    }
    catch (const std::exception& e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        std::cerr << "Error al consultar movimientos: " << e.what() << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "\n=== ERROR AL CONSULTAR MOVIMIENTOS ===" << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "Error: " << e.what() << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
        
        // REINICIAR LA MARQUESINA EN CASO DE ERROR
        inicializar_marquesina();
    }
}

void consultar_cuentas(Banco& banco)
{
    system("cls");
    visibilidad_cursor(true);
    try
    {
        int fila_actual = 2;
        string dni, nombre, apellido;
        bool criterio_valido = false;

        // Encabezado bonito
        mover_cursor(1, fila_actual);
        cout << "==============================================" << endl;
        mover_cursor(1, ++fila_actual);
        cout << "      CONSULTA DE CUENTAS DE CLIENTE         " << endl;
        mover_cursor(1, ++fila_actual);
        cout << "==============================================" << endl;
        fila_actual += 2;

        // Validación de DNI (opcional)
        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el DNI del cliente (o deje vacío): ");
        dni = ingresar_dni("");
        if (dni == "__ESC__") return;
        if (!dni.empty() && validarCedulaEcuatoriana(dni)) criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        // Validación de nombre (opcional)
        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el nombre del cliente (o deje vacío): ");
        nombre = ingresar_alfabetico("");
        if (nombre == "__ESC__") return;
        if (!nombre.empty()) criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        // Validación de apellido (opcional)
        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el apellido del cliente (o deje vacío): ");
        apellido = ingresar_alfabetico("");
        if (apellido == "__ESC__") return;
        if (!apellido.empty()) criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        // Verificar que al menos un criterio sea válido
        if (!criterio_valido)
        {
            mover_cursor(1, fila_actual++);
            throw std::invalid_argument("Debe ingresar al menos un criterio de búsqueda (DNI, nombre o apellido)");
        }

        mover_cursor(1, fila_actual++);
        cout << "Buscando cuentas..." << endl;
        this_thread::sleep_for(std::chrono::seconds(3));
        
        // DETENER LA MARQUESINA ANTES DE CONSULTAR
        detener_marquesina();
        
        banco.consultar_cuentas_cliente(dni, nombre, apellido,fila_actual);
        getch();
        
        // REINICIAR LA MARQUESINA AL SALIR
        inicializar_marquesina();
        return;
    }
    catch (const std::exception& e)
    {
        int fila_error = 20;
        mover_cursor(1, fila_error++);
        std::cerr << "Error al consultar cuentas: " << e.what() << std::endl;
        mover_cursor(1, fila_error++);
        std::cout << "\n=== ERROR AL CONSULTAR CUENTAS ===" << endl;
        mover_cursor(1, fila_error++);
        std::cout << "Error: " << e.what() << endl;
        mover_cursor(1, fila_error++);
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
        
        // REINICIAR LA MARQUESINA EN CASO DE ERROR
        inicializar_marquesina();
    }
}

void menu_cuenta(Banco& banco) {
    // Primero seleccionar el método de apertura
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
        case 1:
            menu_cuenta_aplicacion(banco);
            break;
        case 2:
            menu_cita_presencial(banco);
            break;
        case 3:
            return;
    }
}

// Variable global para la marquesina
Marquesina marquesina_global;

void inicializar_marquesina() {
    marquesina_global.iniciar("SISTEMA BANCARIO");
}

void detener_marquesina() {
    marquesina_global.detener();
}

void ajustar_cursor_para_marquesina() {
    // Mover el cursor a la línea 2 para dejar espacio para la marquesina
    // y asegurar que no interfiera con el contenido del menú
    mover_cursor(0, 2);
}

// Función para el menú de apertura vía aplicación (método tradicional)
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
            case 1: 
                abrir_cuenta_sin_sucursal(banco, 1); 
                break;
            case 2: 
                abrir_cuenta_sin_sucursal(banco, 2); 
                break;
            case 3: 
                return;
        }
    } while (opcion != NUM_OPCIONES);
}

// Función para el menú de cita presencial con API real
void menu_cita_presencial(Banco& banco) {
    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);
    
    cout << "\n=== SISTEMA DE CITAS PRESENCIALES ===" << endl;
    cout << "Bienvenido al sistema de agendamiento de citas" << endl;
    cout << "Servicio: Apertura de cuenta presencial\n" << endl;
    
    // Usar el sistema de geolocalización real
    mostrar_sucursales_cercanas();
    
    // Preguntar si desea agendar cita
    string respuesta;
    do {
        limpiar_linea("¿Desea agendar una cita presencial? (S/N): ");
        respuesta = ingresar_alfabetico("");
        if (respuesta == "__ESC__") return;
        transform(respuesta.begin(), respuesta.end(), respuesta.begin(), ::toupper);
    } while (respuesta != "S" && respuesta != "N");
    
    if (respuesta == "S") {
        agendar_cita_presencial();
    } else {
        cout << "\nGracias por usar nuestro servicio." << endl;
        pausar_consola();
    }
}

// Función para mostrar las sucursales más cercanas usando selección manual
void mostrar_sucursales_cercanas() {
    // Usar el sistema de geolocalización con menús manuales
    geoSystem.runGeolocationSystem();
}

// Función para agendar la cita presencial usando selección manual
void agendar_cita_presencial() {
    string sucursal_num, horario_num;
    
    cout << "\n=== AGENDAR CITA PRESENCIAL ===" << endl;
    
    // Usar el sistema completo de selección manual y mostrar sucursales
    auto result = geoSystem.runGeolocationSystem();
    Branch selected_branch = result.first;
    double distance = result.second;
    
    // Mostrar horarios disponibles para la sucursal más cercana
    auto horarios = geoSystem.generateTimeSlots(selected_branch.queue_position);
    
    cout << "\nSucursal más cercana: " << selected_branch.name << endl;
    cout << "Distancia: " << fixed << setprecision(2) << distance << " km" << endl;
    
    if (selected_branch.queue_position == 0) {
        cout << "Estado: ¡Excelente! No hay cola - Atención inmediata" << endl;
        cout << "Tiempo de espera estimado: 0 minutos" << endl;
    } else {
        cout << "Personas en cola: " << selected_branch.queue_position << endl;
        cout << "Tiempo de espera estimado: " << (selected_branch.queue_position * 40) << " minutos" << endl;
    }
    cout << "\nHorarios disponibles:" << endl;
    for (size_t i = 0; i < horarios.size(); i++) {
        cout << (i + 1) << ". " << horarios[i] << endl;
    }
    
    // Seleccionar horario
    int horario_elegido;
    do {
        limpiar_linea("Seleccione el horario (1-" + to_string(horarios.size()) + "): ");
        horario_num = ingresar_dni("");
        if (horario_num == "__ESC__") return;
        try {
            horario_elegido = stoi(horario_num);
        } catch (...) {
            horario_elegido = 0;
        }
    } while (horario_elegido < 1 || horario_elegido > static_cast<int>(horarios.size()));
    
    // Obtener fecha actual
    string fecha_actual = geoSystem.getCurrentDate();
    
    // Confirmación final
    cout << "\n=== CONFIRMACIÓN DE CITA ===" << endl;
    cout << "Sucursal: " << selected_branch.name << endl;
    cout << "Dirección: " << selected_branch.address << endl;
    cout << "Fecha: " << fecha_actual << endl;
    cout << "Hora: " << horarios[horario_elegido - 1] << endl;
    cout << "Servicio: Apertura de cuenta presencial" << endl;
    cout << "Distancia desde su ubicación: " << fixed << setprecision(2) << distance << " km" << endl;
    
    // Confirmar la cita
    bool confirmar = seleccionar_Si_No();
    if (confirmar) {
        // Actualizar la cola de la sucursal (simular que se agendó una cita)
        geoSystem.updateBranchQueue(selected_branch.id, selected_branch.queue_position + 1);
        
        cout << "\n=== CITA CONFIRMADA ===" << endl;
        cout << "¡Su cita ha sido agendada exitosamente!" << endl;
        cout << "Recuerde llegar 10 minutos antes." << endl;
        cout << "Traiga consigo: cédula, comprobante de ingresos y $25 mínimo para depósito inicial." << endl;
        cout << "Número de confirmación: " << selected_branch.id << horario_elegido << time(nullptr) % 10000 << endl;
    } else {
        cout << "\nCita cancelada." << endl;
    }
    
    pausar_consola();
}