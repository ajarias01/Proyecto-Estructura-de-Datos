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
#include "QR.h"
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

    int fila_ajustada = std::max(fila_inicio, 3);

    mover_cursor(1, fila_ajustada);
    mover_cursor(2, fila_ajustada);
    cout << titulo;

    for (int e = 0; e < n; e++) {
        mover_cursor(10, fila_ajustada + 2 + e);
        mover_cursor(2, fila_ajustada + 2 + e);
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
                mover_cursor(10, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << "    " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << string(80, ' ');
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << "   " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];

                opcionSeleccionada--;
                if (opcionSeleccionada < 1) opcionSeleccionada = n;

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
                if (opcionSeleccionada > n) opcionSeleccionada = 1;

                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << string(80, ' ');
                mover_cursor(2, fila_ajustada + 2 + (opcionSeleccionada - 1));
                cout << " ➤ " << opcionSeleccionada << " " << opciones[opcionSeleccionada - 1];
                break;
        }
    } while (tecla != ENTER);

    return opcionSeleccionada - 1; // Ajuste para devolver índice 0-based
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
    
    return opcion == 0; // Retorna true para "SÍ" (índice 0)
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
        if (tecla == 72 && opcion > 1) cursor_y -= 3;
        else if (tecla == 80 && opcion < nopciones) cursor_y += 3;
        imprimir_opciones();
        mover_cursor(cursor_x, cursor_y);
        opcion = ((cursor_y - 3) / 3) + 1;
    }
    
    return opcion - 1; // Ajuste para devolver índice 0-based
}

bool validar_credenciales_administrador(const string& usuario, const string& contrasenia) {
    const string USUARIO_ADMIN = "admin";
    const string CONTRASENIA_ADMIN = "1234567890";
    return usuario == USUARIO_ADMIN && contrasenia == CONTRASENIA_ADMIN;
}

void recuperar_backup_por_fecha(Banco& banco)
{
    int horas = 0, minutos = 0, segundos = 0;
    string fecha_hora;
    system("cls");
    visibilidad_cursor(true);
    try
    {
        Fecha fecha;
        do {
            cout << endl;
            limpiar_linea("➤ Ingrese la fecha del backup (DD/MM/YYYY): ");
            fecha = validarFecha("");
            if (fecha.get_dia() == -1) return;
        } while (!fecha.validar());
        cout << endl;

        do {
            limpiar_linea("➤ Ingrese la hora del backup (HH:MM:SS): ");
            fecha_hora = validarHora("");
            if (fecha_hora == "__ESC__") return;
            horas = stoi(fecha_hora.substr(0, 2));
            minutos = stoi(fecha_hora.substr(2, 2));
            segundos = stoi(fecha_hora.substr(4, 2));
        } while (!validar_hora_minuto_segundo(horas, minutos, segundos));
        cout << endl;

        char buffer[100];
        snprintf(buffer, sizeof(buffer), "backup_clientes_%04d%02d%02d_%02d%02d%02d.bin",
                fecha.get_anuario(), fecha.get_mes(), fecha.get_dia(),
                horas, minutos, segundos);
        string nombre_archivo = buffer;

        ifstream test_file(nombre_archivo, ios::binary);
        if (!test_file.good()) {
            cout << "\n=== ERROR: El backup no existe para esa fecha y hora ===" << endl;
            pausar_consola();
            return;
        }
        test_file.close();

        cout << "\nIntentando recuperar backup: " << nombre_archivo << endl;
        this_thread::sleep_for(chrono::seconds(3));
        ListaDoble<Cliente*>* nuevos_clientes = RespaldoDatos::restaurarClientesBinario(nombre_archivo);
        if (!nuevos_clientes || nuevos_clientes->esta_vacia()) {
            cout << "\n=== ERROR: No se pudieron cargar datos del backup ===" << endl;
            pausar_consola();
            return;
        }

        if (banco.getClientes()) {
            delete banco.getClientes();
        }
        banco.setClientes(nuevos_clientes);

        banco.guardar_datos_binario_sin_backup("datos.bin");

        cout << "\n=== BACKUP MÁS RECIENTE RECUPERADO EXITOSAMENTE ===" << endl;
        cout << "Datos cargados desde: " << nombre_archivo << endl;
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
    catch (const std::exception& e)
    {
        cerr << "Error al recuperar backup: " << e.what() << endl;
        cout << "\n=== ERROR AL RECUPERAR BACKUP ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nRegresando al menú principal...\n";
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

    const char* campos[] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    int campo = seleccionar_opcion("Ordenar/buscar por:", campos, 5, 2);

    const char* ordenes[] = {"Ascendente", "Descendente"};
    int orden = seleccionar_opcion("Orden:", ordenes, 2, 10);

    GestorClientes gestor;
    auto& lista = *clientes; // Initial declaration
    int n = lista.getTam();

    switch (campo) {
        case 0: // DNI
            gestor.radixSortCampoNumerico(lista, n, [](Cliente* c) { return std::stoi(c->get_dni()); });
            break;
        case 1: // Nombre
            gestor.radixSortStringCampo(lista, n, [](Cliente* c) { return c->get_nombres(); });
            break;
        case 2: // Apellido
            gestor.radixSortStringCampo(lista, n, [](Cliente* c) { return c->get_apellidos(); });
            break;
        case 3: // Teléfono
            gestor.radixSortCampoNumerico(lista, n, [](Cliente* c) { return std::stoi(c->get_telefono()); });
            break;
        case 4: // Email
            gestor.radixSortStringCampo(lista, n, [](Cliente* c) { return c->get_email(); });
            break;
    }
    if (orden == 1) {
        std::vector<Cliente*> temp;
        for (int i = 0; i < n; ++i) temp.push_back(lista.get_contador(i));
        std::reverse(temp.begin(), temp.end());
        for (int i = 0; i < n; ++i) lista.set_contador(i, temp[i]);
    }

    // Menú de selección de campo
    ajustar_cursor_para_marquesina();
    const int NUM_CAMPOS = 5;
    const char* CAMPOS[NUM_CAMPOS] = {"DNI", "Nombre", "Apellido", "Teléfono", "Email"};
    campo = seleccionar_opcion("===== SELECCIONAR CAMPO =====", CAMPOS, NUM_CAMPOS, 4);

    // Menú de selección de orden
    system("cls");
    ajustar_cursor_para_marquesina();
    const int NUM_ORDENES = 2;
    const char* ORDENES[NUM_ORDENES] = {"Ascendente", "Descendente"};
    orden = seleccionar_opcion("===== SELECCIONAR ORDEN =====", ORDENES, NUM_ORDENES, 4);

    // Convertir lista a vector para ordenamiento más eficiente
    n = lista.getTam(); // Update n with the current size
    std::vector<Cliente*> vectorClientes;
    for (int i = 0; i < n; ++i) {
        vectorClientes.push_back(lista.get_contador(i));
    }

    // Función de comparación personalizada según el campo seleccionado
    auto comparar = [campo, orden](Cliente* a, Cliente* b) -> bool {
        if (campo == 0) { // DNI
            return orden == 0 ? std::stoi(a->get_dni()) < std::stoi(b->get_dni())
                             : std::stoi(a->get_dni()) > std::stoi(b->get_dni());
        } else if (campo == 1) { // Nombre
            string nombre_a = a->get_nombres();
            string nombre_b = b->get_nombres();
            transform(nombre_a.begin(), nombre_a.end(), nombre_a.begin(), ::tolower);
            transform(nombre_b.begin(), nombre_b.end(), nombre_b.begin(), ::tolower);
            return orden == 0 ? nombre_a < nombre_b : nombre_a > nombre_b;
        } else if (campo == 2) { // Apellido
            string apellido_a = a->get_apellidos();
            string apellido_b = b->get_apellidos();
            transform(apellido_a.begin(), apellido_a.end(), apellido_a.begin(), ::tolower);
            transform(apellido_b.begin(), apellido_b.end(), apellido_b.begin(), ::tolower);
            return orden == 0 ? apellido_a < apellido_b : apellido_a > apellido_b;
        } else if (campo == 3) { // Teléfono
            return orden == 0 ? std::stoi(a->get_telefono()) < std::stoi(b->get_telefono())
                             : std::stoi(a->get_telefono()) > std::stoi(b->get_telefono());
        } else if (campo == 4) { // Email
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
    for (int i = 0; i < n; ++i) {
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

    string respuesta, valor_buscar;
    do {
        limpiar_linea("➤ ¿Desea buscar un cliente específico? (S/N): ");
        respuesta = ingresar_alfabetico("");
        if (respuesta == "__ESC__") return;
        transform(respuesta.begin(), respuesta.end(), respuesta.begin(), ::toupper);
    } while (respuesta != "S" && respuesta != "N");
    cout << endl;
    if (respuesta == "S") {
        do {
            limpiar_linea("➤ Ingrese el valor a buscar: ");
            if (campo == 0 || campo == 3) {
                valor_buscar = ingresar_dni("");
            } else {
                valor_buscar = ingresar_alfabetico("");
            }
            if (valor_buscar == "__ESC__") return;
        } while (!validar_valor_busqueda(campo, valor_buscar));
        
        int pos = -1;
        if (campo == 0 || campo == 3) {
            int valor = std::stoi(valor_buscar);
            int left = 0, right = n - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                int cmp = (campo == 0) ? std::stoi(lista.get_contador(mid)->get_dni())
                                      : std::stoi(lista.get_contador(mid)->get_telefono());
                if (cmp == valor) { pos = mid; break; }
                if (cmp < valor) left = mid + 1;
                else right = mid - 1;
            }
        } else {
            int left = 0, right = n - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                string cmp = (campo == 1) ? lista.get_contador(mid)->get_nombres()
                                 : (campo == 2) ? lista.get_contador(mid)->get_apellidos()
                                 : lista.get_contador(mid)->get_email();
                int res = cmp.compare(valor_buscar);
                if (res == 0) { pos = mid; break; }
                if (res < 0) left = mid + 1;
                else right = mid - 1;
            }
        }

        if (pos != -1) {
            Cliente* cliente = lista.get_contador(pos);
            cout << "\n=== CLIENTE ENCONTRADO ===\n";
            cout << "DNI: " << cliente->get_dni() << "\n";
            cout << "Nombre: " << cliente->get_nombres() << "\n";
            cout << "Apellido: " << cliente->get_apellidos() << "\n";
            cout << "Teléfono: " << cliente->get_telefono() << "\n";
            cout << "Email: " << cliente->get_email() << "\n";
            auto* cuentas = cliente->get_cuentas();
            if (cuentas && !cuentas->esta_vacia()) {
                cout << "\n=== CUENTAS DEL CLIENTE ===\n";
                cout << std::left << std::setw(15) << "ID Cuenta" 
                     << std::setw(12) << "Tipo" 
                     << std::setw(15) << "Saldo" 
                     << std::setw(20) << "Fecha Apertura" << std::endl;
                cout << "--------------------------------------------------------------\n";
                cuentas->recorrer([](Cuenta* cuenta) {
                    cout << std::left << std::setw(15) << cuenta->get_id_cuenta()
                         << std::setw(12) << cuenta->get_tipo()
                         << std::setw(15) << ("$" + std::to_string(cuenta->get_saldo()))
                         << std::setw(20) << cuenta->get_fecha_apertura().to_string() << std::endl;
                });
                cout << "--------------------------------------------------------------\n";
            } else {
                cout << "\nEste cliente no tiene cuentas registradas.\n";
            }
        } else {
            cout << "\nNo se encontró ningún cliente con ese valor.\n";
        }
    }

    cout << "\nPresione cualquier tecla para continuar...";
    getch();
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
    cout << "\n===========================================" << endl;
    cout << "===    CIFRAR ARCHIVO DE RESPALDO    ===" << endl;
    cout << "===========================================" << endl;

    vector<string> backups;
    for (const auto& entry : filesystem::directory_iterator(".")) {
        string nombre = entry.path().filename().string();
        if (nombre.find("backup_clientes_") == 0 && nombre.substr(nombre.size() - 4) == ".bin") {
            backups.push_back(nombre);
        }
    }

    if (backups.empty()) {
        cout << "\nNo hay archivos de respaldo (.bin) para cifrar.\n";
        pausar_consola();
        return;
    }

    cout << "\n=== SELECCIONE EL ARCHIVO DE RESPALDO A CIFRAR ===\n";
    for (size_t i = 0; i < backups.size(); ++i) {
        cout << (i + 1) << ". " << backups[i] << endl;
    }

    int seleccion = 0;
    string numero;
    do {
        limpiar_linea("Seleccione el archivo a cifrar (1-" + to_string(backups.size()) + "): ");
        numero = ingresar_dni("");
        if (numero == "__ESC__") return;
        if (!numero.empty()) {
            try {
                seleccion = stoi(numero);
            } catch (...) {
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
    cout << "-------------------------------------------" << endl;
    cout << "=== ¡CIFRADO COMPLETADO CON ÉXITO! ===" << endl;
    size_t pos1 = nombre_archivo.find("backup_clientes_");
    size_t pos2 = nombre_archivo.find(".bin");
    string timestamp = (pos1 != string::npos && pos2 != string::npos)
        ? nombre_archivo.substr(16, pos2 - 16)
        : "error_timestamp";
    cout << "El archivo ha sido cifrado y guardado como: cifrado_" << timestamp << ".txt" << endl;
    cout << "===========================================" << endl;

    pausar_consola();
}

void menu_administrador(Banco& banco)
{
    const int NUM_OPCIONES = 10;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Consultar movimientos por fecha",
        "Consultar cuentas por DNI/nombre",
        "Base de datos",
        "Árbol binario",
        "Recuperar backup por fecha y hora",
        "Cifrar archivos",
        "Descifrar archivos",
        "Verificar integridad de datos (Hash)",
        "Generar tabla Hash",
        "Salir"
    };

    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);
    string usuario, contrasenia;
    try
    {
        cout << "\n\t\t===========================================" << endl;
        cout << "\t\t==========     ADMINISTRADOR     ==========" << endl;
        cout << "\t\t===========================================" << endl;
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
                    cifrar_archivos_txt(banco);
                    break;
                case 6:
                    descifrar_archivos_txt(banco);
                    break;
                case 7:
                    verificar_hash(banco);
                    break;
                case 8:
                    buscar_con_tabla_hash(banco);
                    break;
                case 9:
                    return;
            }
        } while (opcion != 8); // Cambiado a 8 para coincidir con "Salir" (índice 8)
    }
    catch (const std::exception& e)
    {
        cerr << "Error en autenticación: " << e.what() << endl;
        cout << "\n=== ERROR AL ACCEDER AL MENÚ DE ADMINISTRADOR ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void menu_cliente(Banco& banco)
{
    const int NUM_OPCIONES = 4;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Realizar depósito",
        "Realizar retiro",
        "Generar QR",
        "Salir"
    };

    system("cls");
    ajustar_cursor_para_marquesina();
    visibilidad_cursor(true);
    string dni, contrasenia;
    try
    {
        do {
            limpiar_linea("➤ Ingrese su DNI: ");
            dni = ingresar_dni("");
            if (dni == "__ESC__") return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;

        do
        {
            limpiar_linea("➤ Ingrese su contraseña: ");
            contrasenia = ingresar_contrasenia("");
            if (contrasenia == "__ESC__") return;
        } while (!validar_contrasenia(contrasenia));
        cout << endl;

        Cliente* cliente = banco.buscar_cliente(dni);
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
                        string id_cuenta;
                        do {
                            limpiar_linea("➤ Ingrese el ID de la cuenta para generar QR: ");
                            id_cuenta = ingresar_id("");
                            if (id_cuenta == "__ESC__") break;
                        } while (!validar_id_cuenta(cliente, id_cuenta));
                        if (id_cuenta != "__ESC__") {
                            QR qr(cliente->get_nombres() + " " + cliente->get_apellidos(), id_cuenta);
                            qr.generateQRAndPDF();
                        }
                    }
                    break;
                case 3:
                    return;
            }
        } while (opcion != 3); // Cambiado a 3 para coincidir con "Salir" (índice 3)
    }
    catch (const std::exception& e)
    {
        cerr << "Error al iniciar sesión: " << e.what() << endl;
        cout << "\n=== ERROR AL INICIAR SESIÓN ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void menu_principal(Banco& banco)
{
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
                detener_marquesina();
                return;
        }
    } while (opcion != 4); // Cambiado a 4 para coincidir con "Salir" (índice 4)
}

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
    catch (const std::exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        cout << "\n=== ERROR AL ABRIR AYUDA TÉCNICA ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nRegresando al menú principal...\n";
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
        int fila_actual = 4;

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
                throw runtime_error("El cliente con DNI " + dni + " ya tiene una cuenta de este tipo.");
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
            throw runtime_error("Error al crear la cuenta");
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
            time_t tt = chrono::system_clock::to_time_t(appointmentTime);
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
            time_t tt = chrono::system_clock::to_time_t(appointmentTime);
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

        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw runtime_error("Cliente no encontrado");

        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        do {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el ID de la cuenta: ");
            id_cuenta = ingresar_id("");
            if (id_cuenta == "__ESC__") return;
        } while (!validar_id_cuenta(cliente, id_cuenta));
        cout << endl;
        fila_actual += 2;
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);

        mover_cursor(1, fila_actual++);
        cout << "Saldo actual: $" << cuenta->get_saldo() << endl;
        fila_actual++;

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

        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea realizar el depósito?" << endl;
        fila_actual += 1;

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

        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw runtime_error("Cliente no encontrado");

        mover_cursor(1, fila_actual++);
        cout << "Cliente: " << cliente->get_nombres() << " " << cliente->get_apellidos() << " | DNI: " << cliente->get_dni() << endl;
        fila_actual++;

        do {
            mover_cursor(1, fila_actual);
            limpiar_linea("➤ Ingrese el ID de la cuenta: ");
            id_cuenta = ingresar_id("");
            if (id_cuenta == "__ESC__") return;
        } while (!validar_id_cuenta(cliente, id_cuenta));
        cout << endl;
        fila_actual += 2;
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);

        mover_cursor(1, fila_actual++);
        cout << "Saldo actual: $" << cuenta->get_saldo() << endl;
        fila_actual++;

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

        mover_cursor(1, fila_actual);
        cout << "¿Está seguro que desea realizar el retiro?" << endl;
        fila_actual += 1;

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
            cout << "     Datos guardados en datos.bin" << endl;
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
    catch (const std::exception& e)
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
            if (dni == "__ESC__") return;
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        fila_actual += 2;

        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw runtime_error("Cliente con DNI " + dni + " no encontrado");

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
    catch (const std::exception& e)
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
        if (dni == "__ESC__") return;
        if (!dni.empty() && validarCedulaEcuatoriana(dni)) criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el nombre del cliente (o deje vacío): ");
        nombre = ingresar_alfabetico("");
        if (nombre == "__ESC__") return;
        if (!nombre.empty()) criterio_valido = true;
        cout << endl;
        fila_actual += 2;

        mover_cursor(1, fila_actual);
        limpiar_linea("➤ Ingrese el apellido del cliente (o deje vacío): ");
        apellido = ingresar_alfabetico("");
        if (apellido == "__ESC__") return;
        if (!apellido.empty()) criterio_valido = true;
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
        
        banco.consultar_cuentas_cliente(dni, nombre, apellido, fila_actual);
        getch();
        
        inicializar_marquesina();
    }
    catch (const std::exception& e)
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

void menu_cuenta(Banco& banco) {
    const int NUM_SUCURSALES = 3;
    const char* SUCURSALES[NUM_SUCURSALES] = {
        "Sucursal Norte - Av. 10 de Agosto y Mariana de Jesús",
        "Sucursal Centro - Av. 12 de Octubre y Veintimilla",
        "Sucursal Sur - Av. Morán Valverde y Rumichaca"
    };

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
        case 0: // Adjusted indices to 0-based
            menu_cuenta_aplicacion(banco);
            break;
        case 1:
            menu_cita_presencial(banco);
            break;
        case 2:
            return;
    }

    int branchChoice = seleccionar_opcion("===== SELECCIÓN DE SUCURSAL =====", SUCURSALES, NUM_SUCURSALES, 4);
    int branchId = branchChoice;
    string sucursal_seleccionada = SUCURSALES[branchChoice - 1];

    const int NUM_OPCIONES = 3;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Cuenta de Ahorros",
        "Cuenta Corriente",
        "Menú Principal"
    };

    visibilidad_cursor(true);
    system("cls");
    ajustar_cursor_para_marquesina();
    int fila_actual = 4;

    mover_cursor(1, fila_actual);
    cout << "==============================================" << endl;
    mover_cursor(1, ++fila_actual);
    cout << "         SELECCIÓN DE SUCURSAL                " << endl;
    mover_cursor(1, ++fila_actual);
    cout << "==============================================" << endl;
    fila_actual += 2;

    int opcion;
    do {
        system("cls");
        ajustar_cursor_para_marquesina();
        opcion = seleccionar_opcion("===== TIPO DE CUENTA =====", OPCIONES, NUM_OPCIONES, 4);
        switch (opcion) {
            case 0:
                abrir_cuenta(banco, 1, branchId, sucursal_seleccionada);
                break;
            case 1:
                abrir_cuenta(banco, 2, branchId, sucursal_seleccionada);
                break;
            case 2:
                return;
        }
    } while (opcion != 2); // Loop until "Menú Principal" is selected
}

Marquesina marquesina_global;

void inicializar_marquesina() {
    marquesina_global.iniciar("SISTEMA BANCARIO");
}

void detener_marquesina() {
    marquesina_global.detener();
}

void ajustar_cursor_para_marquesina() {
    mover_cursor(0, 2);
}

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
            if (valor_buscar == "__ESC__") return;
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
                auto* cuentas = cliente->get_cuentas();
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