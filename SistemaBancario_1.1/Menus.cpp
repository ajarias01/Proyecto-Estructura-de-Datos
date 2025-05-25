#include "Menus.h"
#include <stdexcept>
#include <random>
#include <algorithm>
#include <iostream>
using namespace std;

void pausar_consola() {
    cout << "Presione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void visibilidad_cursor(bool visible) {
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = visible;
    SetConsoleCursorInfo(manejo_consola, &info);
}

void mover_cursor(int x, int y) {
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(manejo_consola, pos);
}

int desplegar_menu(const char** opciones, int nopciones) {
    const short int ANCHO_BOTON = 35;
    visibilidad_cursor(false);
    char tecla;
    HANDLE manejo_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    short int cursor_x = 0, cursor_y = 1, color_act = 0, opcion = 1;
    int colores[2] = {15, 191};

    auto imprimir_opciones = [&]() {
        auto imprimir_boton = [ANCHO_BOTON](const char* opcion) {
            short int caracteres_en_opcion = 0;
            while (opcion[caracteres_en_opcion] != '\0') {
                caracteres_en_opcion++;
            }
            int espacios_faltantes = ANCHO_BOTON - caracteres_en_opcion;
            for (short int i = 0; i < ANCHO_BOTON; i++) {
                printf("%c", ' ');
            }
            printf("\n");
            for (short int i = 0; i < espacios_faltantes / 2; i++) {
                printf("%c", ' ');
            }
            printf("%s", opcion);
            if (espacios_faltantes % 2 == 0) {
                for (short int i = 0; i < espacios_faltantes / 2; i++) {
                    printf("%c", ' ');
                }
            } else {
                for (short int i = 0; i < (espacios_faltantes / 2) + 1; i++) {
                    printf("%c", ' ');
                }
            }
            printf("\n");
            for (short int i = 0; i < ANCHO_BOTON; i++) {
                printf("%c", ' ');
            }
        };

        for (int i = 0; i < nopciones; i++) {
            color_act = (i * 3) + 1 == cursor_y ? 1 : 0;
            mover_cursor(cursor_x, (i * 3));
            SetConsoleTextAttribute(manejo_consola, colores[color_act]);
            imprimir_boton(opciones[i]);
        }
        SetConsoleTextAttribute(manejo_consola, colores[0]);
    };

    imprimir_opciones();
    while ((tecla = getch()) != 13) {
        if (tecla == 72 && opcion > 1) cursor_y -= 3;
        else if (tecla == 80 && opcion < nopciones) cursor_y += 3;
        imprimir_opciones();
        mover_cursor(cursor_x, cursor_y);
        opcion = ((cursor_y - 1) / 3) + 1;
    }
    return opcion;
}

void desplegar_menu_principal(Banco& banco) {
    system("chcp 65001 > nul");
    const int NUM_OPCIONES = 10;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Abrir cuenta",
        "Realizar depósito",
        "Realizar retiro",
        "Consultar movimientos por fecha",
        "Consultar cuentas por DNI/nombre",
        "Calcular intereses acumulados",
        "Guardar datos",
        "Cargar datos",
        "Ayuda Técnica",
        "Salir"
    };

    int opcion;
    do {
        system("cls");
        opcion = desplegar_menu(OPCIONES, NUM_OPCIONES);
        switch (opcion) {
            case 1:
                menu_cuenta(banco);
                break;
            case 2:
                realizar_deposito(banco);
                break;
            case 3:
                realizar_retiro(banco);
                break;
            case 4:
                consultar_movimientos(banco);
                break;
            case 5:
                consultar_cuentas(banco);
                break;
            case 6:
                calcular_intereses(banco);
                break;
            case 7:
                guardar_datos(banco);
                break;
            case 8:
                cargar_datos(banco);
                break;
            case 9: 
                mostrar_ayuda_tecnica(); 
                break;
            case 10: 
                return;
        }
    } while (opcion != NUM_OPCIONES);
}

void mostrar_ayuda_tecnica() {
    system("cls");
    std::cout << "Abriendo el Manual de Ayuda Técnica...\n";
    HINSTANCE result = ShellExecute(NULL, "open", "AyudaTecnicaBanco.chm", NULL, NULL, SW_SHOWNORMAL);
    if (result <= (HINSTANCE)32) {
        std::cerr << "Error: No se pudo abrir el archivo de ayuda. Asegúrese de que 'AyudaTecnicaBanco.chm' esté en el directorio del programa.\n";
    } else {
        std::cout << "Manual de Ayuda Técnica abierto exitosamente.\n";
    }
    pausar_consola();
}

void abrir_cuenta(Banco& banco, int tipo_cuenta) {
    string dni, nombre, apellido, direccion, telefono, email, depositar_inicial;
    Fecha fecha_nacimiento;
    double saldo_inicial = 0;
    visibilidad_cursor(true);
    system("cls");
    
    try {
        // Validación de DNI
        do {
            cout << "\r!!!Ingrese el DNI del cliente: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el DNI del cliente: ";
            dni = ingresar_dni("");
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        
        // Validación de nombre
        do {
            cout << "\r!!!Ingrese el nombre del cliente: ";
            cout << "                                    ";
            cout << "\r!!!Ingrese el nombre del cliente: ";
            nombre = ingresar_alfabetico("");
        } while (nombre.empty() || nombre.length() < 3);
        cout << endl;
        
        // Validación de apellido
        do {
            cout << "\r!!!Ingrese el apellido del cliente: ";
            cout << "                                      ";
            cout << "\r!!!Ingrese el apellido del cliente: ";
            apellido = ingresar_alfabetico("");
        } while (apellido.empty() || apellido.length() < 3);
        cout << endl;
        
        // Validación de dirección
        do {
            cout << "\r!!!Ingrese la dirección del cliente: ";
            cout << "                                       ";
            cout << "\r!!!Ingrese la dirección del cliente: ";
            direccion = ingresar_direccion("");
        } while (direccion.empty() || direccion.length() < 5);
        cout << endl;
        
        // Validación de teléfono
        do {
            cout << "\r!!!Ingrese el teléfono del cliente: ";
            cout << "                                      ";
            cout << "\r!!!Ingrese el teléfono del cliente: ";
            telefono = ingresar_dni("");
        } while (!validar_telefono(telefono));
        cout << endl;
        
        // Validación de email
        do {
            cout << "\r!!!Ingrese el email del cliente: ";
            cout << "                                   ";
            cout << "\r!!!Ingrese el email del cliente: ";
            email = ingresar_email("");
        } while (!validar_email(email));
        cout << endl;
        
        // Validación de fecha de nacimiento
        do {
            cout << "\r!!!Ingrese la fecha de nacimiento (DD/MM/YYYY): ";
            cout << "                                                ";
            cout << "\r!!!Ingrese la fecha de nacimiento (DD/MM/YYYY): ";
            fecha_nacimiento = validarFecha("");
        } while (fecha_nacimiento.empty());
        cout << endl;

        // Generar ID aleatorio basado en la cédula
        string id_str = dni.substr(0, 4);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 25);
        for (int i = 0; i < 4; i++) {
            id_str += char('A' + dis(gen));
        }
        int id_cuenta = stoi(id_str.substr(0, 8));

        // Preguntar si desea depositar un monto inicial
        do {
            cout << "\r!!!¿Desea depositar un monto inicial? (S/N): ";
            cout << "                                             ";
            cout << "\r!!!¿Desea depositar un monto inicial? (S/N): ";
            depositar_inicial = ingresar_alfabetico("");
            transform(depositar_inicial.begin(), depositar_inicial.end(), depositar_inicial.begin(), ::toupper);
        } while (depositar_inicial != "S" && depositar_inicial != "N");
        cout << endl;
        
        if (depositar_inicial == "S") {
            do {
                cout << "\r!!!Ingrese el saldo inicial: ";
                cout << "                               ";
                cout << "\r!!!Ingrese el saldo inicial: ";
                saldo_inicial = ingresar_reales("");
            } while (saldo_inicial < 0);
            cout << endl;
        }

        // Crear la cuenta según el tipo
        Fecha fecha_apertura;
        Cuenta* cuenta = nullptr;
        
        if (tipo_cuenta == 1) {
            cout << "La tasa de interés para la cuenta de ahorros es 5%" << endl;
            double tasa_interes = 5.0;
            cuenta = new Ahorro(id_cuenta, saldo_inicial, fecha_apertura, tasa_interes);
        } else if (tipo_cuenta == 2) {
            double limite_retiro_diario;
            do {
                cout << "\r!!!Ingrese el límite de retiro diario: ";
                cout << "                                        ";
                cout << "\r!!!Ingrese el límite de retiro diario: ";
                limite_retiro_diario = ingresar_reales("");
            } while (limite_retiro_diario <= 0);
            cout << endl;
            cuenta = new Corriente(id_cuenta, saldo_inicial, fecha_apertura, limite_retiro_diario);
        }

        // Verificar que la cuenta se creó correctamente
        if (!cuenta) {
            throw std::runtime_error("Error al crear la cuenta");
        }

        // Crear cliente y agregar cuenta
        Cliente* cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fecha_nacimiento);
        Cliente* cliente_existente = banco.buscar_cliente(dni);
        
        if (cliente_existente) {
            cliente_existente->agregar_cuenta(cuenta);
            delete cliente;
        } else {
            cliente->agregar_cuenta(cuenta);
            banco.agregar_cliente(cliente);
        }

        // Guardar datos temporalmente en temp.bin
        banco.guardar_datos_binario("temp.bin");

        cout << "\n=== CUENTA CREADA EXITOSAMENTE ===" << endl;
        cout << "ID de cuenta: " << id_cuenta << endl;
        cout << "Tipo de cuenta: " << (tipo_cuenta == 1 ? "Ahorros" : "Corriente") << endl;
        cout << "Saldo inicial: $" << saldo_inicial << endl;
        cout << "Datos guardados temporalmente. Use 'Guardar datos' para confirmarlos." << endl;
        cout << "\nPresione Enter para regresar al menú principal...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        
    } catch (const std::exception& e) {
        cout << "\n=== ERROR AL CREAR CUENTA ===" << endl;
        cout << "Error: " << e.what() << endl;
        cout << "\nPresione Enter para regresar al menú principal...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

void realizar_deposito(Banco& banco) {
    string dni;
    try {
        system("cls");
        visibilidad_cursor(true);
        do {
            cout << "\r!!!Ingrese el DNI del cliente: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el DNI del cliente: ";
            dni = ingresar_dni("");
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        
        int id_cuenta;
        do {
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            id_cuenta = ingresar_enteros("");
        } while (id_cuenta <= 0);
        cout << endl;
        
        double monto;
        do {
            cout << "\r!!!Ingrese el monto a depositar: ";
            cout << "                                   ";
            cout << "\r!!!Ingrese el monto a depositar: ";
            monto = ingresar_reales("");
        } while (monto <= 0);
        cout << endl;

        Fecha fecha;
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);
        if (!cuenta) throw std::runtime_error("Cuenta no encontrada");
        cuenta->depositar(monto, fecha);
        std::cout << "\n=== DEPÓSITO REALIZADO EXITOSAMENTE ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al realizar depósito: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL REALIZAR DEPÓSITO ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    }
}

void realizar_retiro(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string dni;
        do {
            cout << "\r!!!Ingrese el DNI del cliente: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el DNI del cliente: ";
            dni = ingresar_dni("");
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        
        int id_cuenta;
        do {
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            id_cuenta = ingresar_enteros("");
        } while (id_cuenta <= 0);
        cout << endl;
        
        double monto;
        do {
            cout << "\r!!!Ingrese el monto a retirar: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el monto a retirar: ";
            monto = ingresar_reales("");
        } while (monto <= 0);
        cout << endl;

        Fecha fecha;
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);
        if (!cuenta) throw std::runtime_error("Cuenta no encontrada");
        if (cuenta->retirar(monto, fecha)) {
            std::cout << "\n=== RETIRO REALIZADO EXITOSAMENTE ===\n";
        } else {
            std::cout << "\n=== FALLO EN EL RETIRO ===\n";
        }
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al realizar retiro: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL REALIZAR RETIRO ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    }
}

void consultar_movimientos(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string dni;
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        cout << "                                               ";
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        dni = ingresar_dni("");
        cout << endl;
        
        std::string fecha_inicio_str;
        do {
            cout << "\r!!!Ingrese la fecha de inicio (YYYY-MM-DD): ";
            cout << "                                              ";
            cout << "\r!!!Ingrese la fecha de inicio (YYYY-MM-DD): ";
            fecha_inicio_str = ingresar_alfabetico("");
        } while (fecha_inicio_str.length() != 10);
        cout << endl;
        
        std::string fecha_fin_str;
        do {
            cout << "\r!!!Ingrese la fecha de fin (YYYY-MM-DD): ";
            cout << "                                            ";
            cout << "\r!!!Ingrese la fecha de fin (YYYY-MM-DD): ";
            fecha_fin_str = ingresar_alfabetico("");
        } while (fecha_fin_str.length() != 10);
        cout << endl;

        Fecha fecha_inicio;
        Fecha fecha_fin;
        if (fecha_inicio.string_to_fecha(fecha_inicio_str) == -1 || fecha_fin.string_to_fecha(fecha_fin_str) == -1) {
            throw std::runtime_error("Formato de fecha inválido");
        }
        banco.consultar_movimientos_rango(dni, fecha_inicio, fecha_fin);
        std::cout << "\n=== CONSULTA FINALIZADA ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar movimientos: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL CONSULTAR MOVIMIENTOS ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    }
}

void consultar_cuentas(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string dni;
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        cout << "                                               ";
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        dni = ingresar_dni("");
        cout << endl;
        
        std::string nombre;
        cout << "\r!!!Ingrese el nombre del cliente (o deje vacío): ";
        cout << "                                                  ";
        cout << "\r!!!Ingrese el nombre del cliente (o deje vacío): ";
        nombre = ingresar_alfabetico("");
        cout << endl;

        banco.consultar_cuentas_cliente(dni, nombre);
        std::cout << "\n=== CONSULTA FINALIZADA ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar cuentas: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL CONSULTAR CUENTAS ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    }
}

void calcular_intereses(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        int id_cuenta;
        do {
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            id_cuenta = ingresar_enteros("");
        } while (id_cuenta <= 0);
        cout << endl;
        
        std::string fecha_hasta_str;
        do {
            cout << "\r!!!Ingrese la fecha hasta (YYYY-MM-DD): ";
            cout << "                                          ";
            cout << "\r!!!Ingrese la fecha hasta (YYYY-MM-DD): ";
            fecha_hasta_str = ingresar_alfabetico("");
        } while (fecha_hasta_str.length() != 10);
        cout << endl;

        Fecha fecha_hasta;
        if (fecha_hasta.string_to_fecha(fecha_hasta_str) == -1) {
            throw std::runtime_error("Formato de fecha inválido");
        }
        banco.calcular_intereses_cuenta(id_cuenta, fecha_hasta);
        std::cout << "\n=== CÁLCULO DE INTERESES FINALIZADO ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al calcular intereses: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL CALCULAR INTERESES ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    }
}

void guardar_datos(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string archivo_base;
        do {
            cout << "\r!!!Ingrese el nombre del archivo para guardar (sin .bin): ";
            cout << "                                                           ";
            cout << "\r!!!Ingrese el nombre del archivo para guardar (sin .bin): ";
            archivo_base = ingresar_alfabetico("");
        } while (archivo_base.empty() || archivo_base.length() < 1);
        cout << endl;

        // Añadir .bin al nombre del archivo
        std::string archivo = archivo_base + ".bin";

        banco.guardar_datos_binario(archivo);
        std::cout << "\n=== DATOS GUARDADOS EXITOSAMENTE EN " << archivo << " ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar datos: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL GUARDAR DATOS ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    }
}

void cargar_datos(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string archivo_base;
        do {
            cout << "\r!!!Ingrese el nombre del archivo para cargar (sin .bin): ";
            cout << "                                                           ";
            cout << "\r!!!Ingrese el nombre del archivo para cargar (sin .bin): ";
            archivo_base = ingresar_alfabetico("");
        } while (archivo_base.empty() || archivo_base.length() < 1);
        cout << endl;

        // Añadir .bin al nombre del archivo
        std::string archivo = archivo_base + ".bin";

        banco.cargar_datos_binario(archivo);
        std::cout << "\n=== DATOS CARGADOS EXITOSAMENTE DESDE " << archivo << " ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL CARGAR DATOS ===\n";
        std::cout << "Regresando al menú principal...\n";
        pausar_consola();
    }
}

void menu_cuenta(Banco& banco) {
    system("chcp 65001 > nul");
    const int NUM_OPCIONES = 3;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Cuenta de Ahorros",
        "Cuenta Corriente",
        "Menú Principal"
    };
    int opcion;
    do {
        system("cls");
        opcion = desplegar_menu(OPCIONES, NUM_OPCIONES);
        switch (opcion) {
            case 1: abrir_cuenta(banco, 1); break;
            case 2: abrir_cuenta(banco, 2); break;
            case 3: return;
        }
    } while (opcion != NUM_OPCIONES);
}