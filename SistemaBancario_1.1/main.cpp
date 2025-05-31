#include <iostream>
#include "Menus.h"

int main() {
    Banco banco;
    banco.cargar_datos_binario("datos.bin"); 
    menu_principal(banco);
    return 0;
}