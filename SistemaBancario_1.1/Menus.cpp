#include "Menus.h"
#include "Fecha.h"
#include "validaciones.h"
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

bool validar_credenciales_administrador(const std::string& usuario, const std::string& contrasenia) {
    const std::string USUARIO_ADMIN = "admin";
    const std::string CONTRASENIA_ADMIN = "Admin123!";
    return usuario == USUARIO_ADMIN && contrasenia == CONTRASENIA_ADMIN;
}

void menu_administrador(Banco& banco) {
    system("chcp 65001 > nul");
    const int NUM_OPCIONES = 5;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Consultar movimientos por fecha",
        "Consultar cuentas por DNI/nombre",
        "Guardar datos",
        "Cargar datos",
        "Salir"
    };

    system("cls");
    visibilidad_cursor(true);
    std::string usuario, contrasenia;
    try {
        do {
            cout << "\r!!!Ingrese el usuario del administrador: ";
            cout << "                                         ";
            cout << "\r!!!Ingrese el usuario del administrador: ";
            usuario = ingresar_alfabetico("");
        } while (usuario.empty() || usuario.length() < 4);
        cout << endl;

        do {
            cout << "\r!!!Ingrese la contraseña del administrador: ";
            cout << "                                             ";
            cout << "\r!!!Ingrese la contraseña del administrador: ";
            contrasenia = ingresar_contrasenia("");
        } while (!validar_contrasenia(contrasenia));
        cout << endl;

        if (!validar_credenciales_administrador(usuario, contrasenia)) {
            throw std::runtime_error("Usuario o contraseña incorrectos");
        }

        int opcion;
        do {
            system("cls");
            opcion = desplegar_menu(OPCIONES, NUM_OPCIONES);
            switch (opcion) {
                case 1:
                    consultar_movimientos(banco);
                    break;
                case 2:
                    consultar_cuentas(banco);
                    break;
                case 3:
                    guardar_datos(banco);
                    break;
                case 4: 
                    cargar_datos(banco);
                    break;
                case 5:
                    return;
            }
        } while (opcion != NUM_OPCIONES);
    } catch (const std::exception& e) {
        std::cerr << "Error en autenticación: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL ACCEDER AL MENÚ DE ADMINISTRADOR ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void menu_cliente(Banco& banco) {
    system("chcp 65001 > nul");
    const int NUM_OPCIONES = 3;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Realizar depósito",
        "Realizar retiro",
        "Salir"
    };

    int opcion;
    do {
        system("cls");
        opcion = desplegar_menu(OPCIONES, NUM_OPCIONES);
        switch (opcion) {
            case 1:
                realizar_deposito(banco);
                break;
            case 2:
                realizar_retiro(banco);
                break;
            case 3:
                return;
        }
    } while (opcion != NUM_OPCIONES);
}

void menu_principal(Banco& banco) {
    system("chcp 65001 > nul");
    const int NUM_OPCIONES = 5;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Crear cuenta",
        "Iniciar sesión",
        "Administrador",
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
                menu_cliente(banco);
                break;
            case 3:
                menu_administrador(banco);
                break;
            case 4:
                mostrar_ayuda_tecnica();
                break;
            case 5:
                return;
        }
    } while (opcion != NUM_OPCIONES);
}

void mostrar_ayuda_tecnica() {
    system("cls");
    visibilidad_cursor(true);
    try {
        std::cout << "Abriendo el Manual de Ayuda Técnica...\n";
        HINSTANCE result = ShellExecute(NULL, "open", "AyudaTecnicaBanco.chm", NULL, NULL, SW_SHOWNORMAL);
        if (result <= (HINSTANCE)32) {
            throw std::runtime_error("No se pudo abrir el archivo de ayuda. Asegúrese de que 'AyudaTecnicaBanco.chm' esté en el directorio del programa.");
        }
        std::cout << "Manual de Ayuda Técnica abierto exitosamente.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL ABRIR AYUDA TÉCNICA ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
    }
    pausar_consola();
}

void abrir_cuenta(Banco& banco, int tipo_cuenta) {
    string dni, nombre, apellido, direccion, telefono, email, depositar_inicial, saldo_inicial1, contrasenia;
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

        // Validación de contraseña
        do {
            cout << "\r!!!Ingrese la contraseña (Debe tener almenos 9 caracteres, un numero, mayuscula, minusculas y un caracter especial): ";
            cout << "                             ";
            cout << "\r!!!Ingrese la contraseña(Debe tener almenos 9 caracteres, un numero, mayuscula, minusculas y un caracter especial): ";
            contrasenia = ingresar_contrasenia("");
        } while (!validar_contrasenia(contrasenia));
        cout << endl;

        // Generar ID de cuenta
        std::string id_cuenta = nombre.substr(0, 1) + apellido.substr(0, 1);
        id_cuenta += (tipo_cuenta == 1) ? "A" : "C";
        id_cuenta += dni.substr(dni.length() - 6, 6);

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
                saldo_inicial1 = ingresar_decimales("");
            } while (!validar_monto(saldo_inicial1));
            saldo_inicial = stod(saldo_inicial1);
        }
        cout << endl;

        // Crear la cuenta según el tipo
        Fecha fecha_apertura;
        Cuenta* cuenta = nullptr;
        
        if (tipo_cuenta == 1) {
            cout << "La tasa de interés para la cuenta de ahorros es 5%" << endl;
            double tasa_interes = 5.0;
            cuenta = new Ahorro(id_cuenta, saldo_inicial, fecha_apertura, tasa_interes);
        } else if (tipo_cuenta == 2) {
            int limite_retiro_diario;
            do {
                cout << "\r!!!Ingrese el límite de retiro diario: ";
                cout << "                                        ";
                cout << "\r!!!Ingrese el límite de retiro diario: ";
                limite_retiro_diario = ingresar_enteros("");
            } while (limite_retiro_diario <= 0);
            cout << endl;
            cuenta = new Corriente(id_cuenta, saldo_inicial, fecha_apertura, limite_retiro_diario);
        }
        
        if (!cuenta) {
            throw std::runtime_error("Error al crear la cuenta");
        }

        Cliente* cliente = new Cliente(dni, nombre, apellido, direccion, telefono, email, fecha_nacimiento, contrasenia);
        Cliente* cliente_existente = banco.buscar_cliente(dni);
        
        if (cliente_existente) {
            cliente_existente->agregar_cuenta(cuenta);
            delete cliente;
        } else {
            cliente->agregar_cuenta(cuenta);
            banco.agregar_cliente(cliente);
        }

        banco.guardar_datos_binario("datos.bin");

        cout << "\n=== CUENTA CREADA EXITOSAMENTE ===" << endl;
        cout << "ID de cuenta: " << id_cuenta << endl;
        cout << "Tipo de cuenta: " << (tipo_cuenta == 1 ? "Ahorros" : "Corriente") << endl;
        cout << "Saldo inicial: $" << saldo_inicial << endl;
        cout << "Datos guardados en datos.bin" << endl;
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
    string dni, salario, id_cuenta;
    double monto = 0;
    system("cls");
    visibilidad_cursor(true);
    try {
        // Validación de DNI
        do {
            cout << "\r!!!Ingrese el DNI del cliente: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el DNI del cliente: ";
            dni = ingresar_dni("");
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        
        // Validación de ID de cuenta
        do {
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            id_cuenta = ingresar_alfabetico("");
        } while (id_cuenta.length() < 9);
        cout << endl;
        
        // Validación de monto
        do {
            cout << "\r!!!Ingrese el monto a depositar: ";
            cout << "                                   ";
            cout << "\r!!!Ingrese el monto a depositar: ";
            salario = ingresar_decimales("");
        } while (!validar_monto(salario));
        monto = stod(salario);
        cout << endl;

        Fecha fecha;
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);
        if (!cuenta) throw std::runtime_error("Cuenta no encontrada");
        cuenta->depositar(monto, fecha);
        banco.guardar_datos_binario("datos.bin");

        std::cout << "\n=== DEPÓSITO REALIZADO EXITOSAMENTE ===" << endl;
        std::cout << "Monto depositado: $" << monto << endl;
        std::cout << "Cuenta: " << id_cuenta << endl;
        std::cout << "Datos guardados en datos.bin" << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al realizar depósito: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL REALIZAR DEPÓSITO ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void realizar_retiro(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    std::string dni, salario, id_cuenta;
    double monto = 0;
    try {
        // Validación de DNI
        do {
            cout << "\r!!!Ingrese el DNI del cliente: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el DNI del cliente: ";
            dni = ingresar_dni("");
        } while (!validarCedulaEcuatoriana(dni));
        cout << endl;
        
        // Validación de ID de cuenta
        do {
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el ID de la cuenta: ";
            id_cuenta = ingresar_alfabetico("");
        } while (id_cuenta.length() < 9);
        cout << endl;
        
        // Validación de monto
        do {
            cout << "\r!!!Ingrese el monto a retirar: ";
            cout << "                                 ";
            cout << "\r!!!Ingrese el monto a retirar: ";
            salario = ingresar_decimales("");
        } while (!validar_monto(salario));
        monto = stod(salario);
        cout << endl;

        Fecha fecha;
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);
        if (!cuenta) throw std::runtime_error("Cuenta no encontrada");
        if (cuenta->retirar(monto, fecha)) {
            banco.guardar_datos_binario("datos.bin");
            std::cout << "\n=== RETIRO REALIZADO EXITOSAMENTE ===" << endl;
            std::cout << "Monto retirado: $" << monto << endl;
            std::cout << "Cuenta: " << id_cuenta << endl;
            std::cout << "Datos guardados en datos.bin" << endl;
        } else {
            std::cout << "\n=== FALLO EN EL RETIRO ===" << endl;
            std::cout << "Verifique el saldo o el límite de retiro diario." << endl;
        }
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al realizar retiro: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL REALIZAR RETIRO ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void consultar_movimientos(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    try {
        std::string dni;
        Fecha fecha_inicio, fecha_fin;
        
        // Validación de DNI (puede estar vacío)
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        cout << "                                               ";
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        dni = ingresar_dni("");
        cout << endl;
        
        // Validación de fecha de inicio
        do {
            cout << "\r!!!Ingrese la fecha de inicio (YYYY-MM-DD): ";
            cout << "                                              ";
            cout << "\r!!!Ingrese la fecha de inicio (YYYY-MM-DD): ";
            fecha_inicio = validarFecha("");
        } while (fecha_inicio.empty());
        cout << endl;
        
        // Validación de fecha de fin
        do {
            cout << "\r!!!Ingrese la fecha de fin (YYYY-MM-DD): ";
            cout << "                                            ";
            cout << "\r!!!Ingrese la fecha de fin (YYYY-MM-DD): ";
            fecha_fin = validarFecha("");
        } while (fecha_fin.empty() || fecha_inicio > fecha_fin);
        cout << endl;

        banco.consultar_movimientos_rango(dni, fecha_inicio, fecha_fin);
        std::cout << "\n=== CONSULTA FINALIZADA ===" << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar movimientos: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL CONSULTAR MOVIMIENTOS ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void consultar_cuentas(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    try {
        std::string dni, nombre, apellido;
        
        // Validación de DNI (puede estar vacío)
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        cout << "                                               ";
        cout << "\r!!!Ingrese el DNI del cliente (o deje vacío): ";
        dni = ingresar_dni("");
        cout << endl;

        // Validación de nombre (puede estar vacío)
        cout << "\r!!!Ingrese el nombre del cliente (o deje vacío): ";
        cout << "                                                  ";
        cout << "\r!!!Ingrese el nombre del cliente (o deje vacío): ";
        nombre = ingresar_alfabetico("");
        cout << endl;

        // Validación de apellido (puede estar vacío)
        cout << "\r!!!Ingrese el apellido del cliente (o deje vacío): ";
        cout << "                                                    ";
        cout << "\r!!!Ingrese el apellido del cliente (o deje vacío): ";
        apellido = ingresar_alfabetico("");
        cout << endl;

        // Combinar nombre y apellido para la búsqueda
        std::string criterio = nombre.empty() ? apellido : nombre;
        banco.consultar_cuentas_cliente(dni, criterio);
        std::cout << "\n=== CONSULTA FINALIZADA ===" << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar cuentas: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL CONSULTAR CUENTAS ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void guardar_datos(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    try {
        std::string archivo_base;
        
        // Validación del nombre del archivo
        do {
            cout << "\r!!!Ingrese el nombre del archivo para guardar (sin .bin): ";
            cout << "                                                           ";
            cout << "\r!!!Ingrese el nombre del archivo para guardar (sin .bin): ";
            archivo_base = ingresar_alfabetico("");
        } while (archivo_base.empty() || archivo_base.length() < 1);
        cout << endl;

        std::string archivo = archivo_base + ".bin";
        banco.guardar_datos_binario(archivo);
        std::cout << "\n=== DATOS GUARDADOS EXITOSAMENTE EN " << archivo << " ===" << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar datos: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL GUARDAR DATOS ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    }
}

void cargar_datos(Banco& banco) {
    system("cls");
    visibilidad_cursor(true);
    try {
        std::string archivo_base;
        
        // Validación del nombre del archivo
        do {
            cout << "\r!!!Ingrese el nombre del archivo para cargar (sin .bin): ";
            cout << "                                                           ";
            cout << "\r!!!Ingrese el nombre del archivo para cargar (sin .bin): ";
            archivo_base = ingresar_alfabetico("");
        } while (archivo_base.empty() || archivo_base.length() < 1);
        cout << endl;

        std::string archivo = archivo_base + ".bin";
        banco.cargar_datos_binario(archivo);
        std::cout << "\n=== DATOS CARGADOS EXITOSAMENTE DESDE " << archivo << " ===" << endl;
        std::cout << "\nRegresando al menú principal...\n";
        pausar_consola();
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
        std::cout << "\n=== ERROR AL CARGAR DATOS ===" << endl;
        std::cout << "Error: " << e.what() << endl;
        std::cout << "\nRegresando al menú principal...\n";
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