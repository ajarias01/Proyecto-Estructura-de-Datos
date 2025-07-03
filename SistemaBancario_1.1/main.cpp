#include <iostream>
#include <conio.h>
#include "Menus.h"

int main() {
    system("chcp 65001 > nul");
    
    Banco banco;
    try {
        banco.cargar_datos_binario("datos.bin");
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar datos: " << e.what() << std::endl;
    } 
    
    getch();
    menu_principal(banco);
    
    return 0;
}