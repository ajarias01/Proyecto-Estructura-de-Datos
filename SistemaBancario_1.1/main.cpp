/**
 * @file main.cpp
 * @brief Función principal del sistema bancario. Inicializa el sistema y lanza el menú principal.
 *
 * Este archivo contiene el punto de entrada del programa. Se encarga de cargar los datos iniciales,
 * manejar excepciones de carga y mostrar el menú principal de operaciones bancarias.
 */
#include <iostream>
#include <conio.h>
#include "Menus.h"

/**
 * @brief Función principal del sistema bancario.
 *
 * - Cambia la codificación de la consola a UTF-8.
 * - Instancia el objeto Banco y carga los datos desde archivo.
 * - Maneja excepciones de carga de datos.
 * - Llama al menú principal para interacción con el usuario.
 *
 * @return int Código de retorno del programa (0 si finaliza correctamente).
 */
int main() {
    system("chcp 65001 > nul");
    
    Banco banco;
    try {
        banco.cargar_datos_binario("datos.txt");
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
    } 
    
    getch();
    menu_principal(banco);
    
    return 0;
}