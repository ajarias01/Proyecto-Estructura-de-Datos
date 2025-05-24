#include "Menus.h"
#include <stdexcept>
using namespace std;


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
    const int NUM_OPCIONES = 8;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Abrir cuenta",
        "Realizar depósito",
        "Realizar retiro",
        "Consultar movimientos por fecha",
        "Consultar cuentas por DNI/nombre",
        "Calcular intereses acumulados",
        "Guardar datos",
        "Cargar datos"
    };

    int opcion;
    do {
        system("cls");
        opcion = desplegar_menu(OPCIONES, NUM_OPCIONES);
        switch (opcion) {
            case 1:menu_cuenta(banco);break;
            case 2: realizar_deposito(banco); break;
            case 3: realizar_retiro(banco); break;
            case 4: consultar_movimientos(banco); break;
            case 5: consultar_cuentas(banco); break;
            case 6: calcular_intereses(banco); break;
            case 7: guardar_datos(banco); break;
            case 8: cargar_datos(banco); break;
        }
    } while (opcion != NUM_OPCIONES + 1);
}

void abrir_cuenta(Banco& banco,int tipo_cuenta) {
    string dni,nombre,direccion,telefono,email,fecha;
    Fecha fecha_nacimiento;
    int saldo_inicial=0;
    visibilidad_cursor(true);
    system("cls");
    try {
          do {
            cout << "\r!!!Ingrese el DNI del cliente: "; // Retorno de carro para sobrescribir la línea
            cout << "                                 "; // Limpia la línea con espacios
            cout << "\r!!!Ingrese el DNI del cliente: "; // Vuelve al inicio de la línea
            dni = ingresar_dni("");
        } while (!validarCedulaEcuatoriana(dni)); 
        cout<<endl;
        do {
            cout << "\r!!!Ingrese el nombre del cliente:"; // Retorno de carro para sobrescribir la línea
            cout << "                                    "; // Limpia la línea con espacios
            cout << "\r!!!Ingrese el nombre del cliente: "; // Vuelve al inicio de la línea
            nombre = ingresar_alfabetico("");
        } while (nombre.empty() || nombre.length() < 3);
        cout<<endl;
        do {
            cout << "\r!!!Ingrese el dirección del cliente: "; // Retorno de carro para sobrescribir la línea
            cout << "                                       "; // Limpia la línea con espacios
            cout << "\r!!!Ingrese la dirección del cliente: "; // Vuelve al inicio de la línea
            direccion = ingresar_direccion("");
        } while (direccion.empty()|| direccion.length() < 5);
        cout<<endl;
        do {
            cout << "\r!!!Ingrese la teléfono del cliente: "; // Retorno de carro para sobrescribir la línea
            cout << "                                      "; // Limpia la línea con espacios
            cout << "\r!!!Ingrese el teléfono del cliente: "; // Vuelve al inicio de la línea
            telefono = ingresar_dni("");
        } while (!validar_telefono(telefono));
        cout<<endl;
        do {
            cout << "\r!!!Ingrese el email del cliente: "; // Retorno de carro para sobrescribir la línea
            cout << "                                   "; // Limpia la línea con espacios
            cout << "\r!!!Ingrese el email del cliente: "; // Vuelve al inicio de la línea
            email = ingresar_email("");
        } while (!validar_email(email));
        cout<<endl;
        do {
            cout << "\r!!!Ingrese el año de nacimiento (DD/MM/YYYY): "; // Retorno de carro para sobrescribir la línea
            cout << "                                                "; // Limpia la línea con espacios
            cout << "\r!!!Ingrese el año de nacimiento (DD/MM/YYYY): "; // Vuelve al inicio de la línea
            fecha_nacimiento = validarFecha("");
        } while (fecha_nacimiento.empty());
        cout<<endl;
        do{
            cout << "\r!!!Ingrese el saldo inicial: "; // Retorno de carro para sobrescribir la línea
            cout << "                               "; // Limpia la línea con espacios
            cout << "\r!!!Ingrese el saldo inicial: "; // Vuelve al inicio de la línea
            saldo_inicial = ingresar_reales("");
        } while (saldo_inicial < 0);
        cout<<endl;
        int id_cuenta = ingresar_enteros("Ingrese el ID de la cuenta");
        Fecha fecha_apertura;
        Cuenta* cuenta;
        if (tipo_cuenta == 1) {
            double tasa_interes = ingresar_reales("Ingrese la tasa de interés anual (%)");
            cuenta = new Ahorro(id_cuenta, saldo_inicial, fecha_apertura, tasa_interes);
        } else if (tipo_cuenta == 2) {
            double limite_retiro_diario = ingresar_reales("Ingrese el límite de retiro diario");
            cuenta = new Corriente(id_cuenta, saldo_inicial, fecha_apertura, limite_retiro_diario);
        } 
        Cliente cliente(dni, nombre, direccion, telefono, email, fecha_nacimiento); //, estado_civil, nif);
        Cliente* cliente_existente = banco.buscar_cliente(dni);
        if (cliente_existente) {
            cliente_existente->agregar_cuenta(cuenta);
            delete cliente_existente;
        } else {
            cliente.agregar_cuenta(cuenta);
            banco.agregar_cliente(cliente);
        }
        std::cout << "Cuenta creada exitosamente.\n";
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al abrir cuenta: " << e.what() << std::endl;
        system("pause");
    }
}

void realizar_deposito(Banco& banco) {
    string dni,nombre,direccion,telefono,email;//estado_civil,nif;
    try {
        system("cls");
        visibilidad_cursor(true);
         dni = ingresar_dni("Ingrese el DNI del cliente");
        int id_cuenta = ingresar_enteros("Ingrese el ID de la cuenta");
        double monto = ingresar_reales("Ingrese el monto a depositar");
        Fecha fecha;
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);
        if (!cuenta) {
            delete cliente;
            throw std::runtime_error("Cuenta no encontrada");
        }
        cuenta->depositar(monto, fecha);
        std::cout << "Depósito realizado exitosamente.\n";
        delete cliente;
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al realizar depósito: " << e.what() << std::endl;
        system("pause");
    }
}

void realizar_retiro(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string dni = ingresar_dni("Ingrese el DNI del cliente");
        int id_cuenta = ingresar_enteros("Ingrese el ID de la cuenta");
        double monto = ingresar_reales("Ingrese el monto a retirar");
        Fecha fecha;
        Cliente* cliente = banco.buscar_cliente(dni);
        if (!cliente) throw std::runtime_error("Cliente no encontrado");
        Cuenta* cuenta = cliente->buscar_cuenta(id_cuenta);
        if (!cuenta) {
            delete cliente;
            throw std::runtime_error("Cuenta no encontrada");
        }
        if (cuenta->retirar(monto, fecha)) {
            std::cout << "Retiro realizado exitosamente.\n";
        } else {
            std::cout << "Fallo en el retiro.\n";
        }
        delete cliente;
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al realizar retiro: " << e.what() << std::endl;
        system("pause");
    }
}

void consultar_movimientos(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string dni = ingresar_dni("Ingrese el DNI del cliente (o deje vacío)");
        std::string fecha_inicio_str = ingresar_alfabetico("Ingrese la fecha de inicio (YYYY-MM-DD)");
        std::string fecha_fin_str = ingresar_alfabetico("Ingrese la fecha de fin (YYYY-MM-DD)");
        Fecha fecha_inicio;
        Fecha fecha_fin;
        fecha_inicio.string_to_fecha(fecha_inicio_str);
        fecha_fin.string_to_fecha(fecha_fin_str);
        banco.consultar_movimientos_rango(dni, fecha_inicio, fecha_fin);
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar movimientos: " << e.what() << std::endl;
        system("pause");
    }
}

void consultar_cuentas(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string dni = ingresar_dni("Ingrese el DNI del cliente (o deje vacío)");
        std::string nombre = ingresar_alfabetico("Ingrese el nombre del cliente (o deje vacío)");
        banco.consultar_cuentas_cliente(dni, nombre);
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar cuentas: " << e.what() << std::endl;
        system("pause");
    }
}

void calcular_intereses(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        int id_cuenta = ingresar_enteros("Ingrese el ID de la cuenta");
        std::string fecha_hasta_str = ingresar_alfabetico("Ingrese la fecha hasta (YYYY-MM-DD)");
        Fecha fecha_hasta;
        fecha_hasta.string_to_fecha(fecha_hasta_str);
        banco.calcular_intereses_cuenta(id_cuenta, fecha_hasta);
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al calcular intereses: " << e.what() << std::endl;
        system("pause");
    }
}

void guardar_datos(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string archivo = ingresar_alfabetico("Ingrese el nombre del archivo para guardar");
        banco.guardar_datos_binario(archivo);
        std::cout << "Datos guardados exitosamente.\n";
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al guardar datos: " << e.what() << std::endl;
        system("pause");
    }
}

void cargar_datos(Banco& banco) {
    try {
        system("cls");
        visibilidad_cursor(true);
        std::string archivo = ingresar_alfabetico("Ingrese el nombre del archivo para cargar");
        banco.cargar_datos_binario(archivo);
        std::cout << "Datos cargados exitosamente.\n";
        system("pause");
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
        system("pause");
    }
}

void menu_cuenta(Banco& banco) {
    system("chcp 65001 > nul");
    const int NUM_OPCIONES = 2;
    const char* OPCIONES[NUM_OPCIONES] = {
        "Cuenta de Ahorros",
        "Cuenta Corriente"
    };
    int opcion;
    do {
        system("cls");
        opcion = desplegar_menu(OPCIONES, NUM_OPCIONES);
        switch (opcion) {
            case 1: abrir_cuenta(banco,1); break;
            case 2: abrir_cuenta(banco,2); break;
        }
    } while (opcion != NUM_OPCIONES + 1);
}