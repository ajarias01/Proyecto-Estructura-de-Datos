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
 * - Instancia el objeto Banco y verifica/recupera los datos desde archivo.
 * - Si datos.txt no existe, permite seleccionar un backup para recuperar.
 * - Llama al menú principal para interacción con el usuario.
 *
 * @return int Código de retorno del programa (0 si finaliza correctamente).
 */
int main() {
    system("chcp 65001 > nul");
    
    Banco banco;
    
    // Verificar y recuperar datos usando la nueva función
    verificar_y_recuperar_datos(banco);
    
    getch();
    menu_principal(banco);
    
    return 0;
}