#include "Marquesina.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

Marquesina::Marquesina() : ejecutando(false), pausado(false), ancho_consola(80), posicion_actual(0) {
    obtener_tamaño_consola();
}

Marquesina::~Marquesina() {
    detener();
}

void Marquesina::iniciar(const std::string& texto) {
    if (ejecutando.load()) {
        detener();
    }
    
    texto_marquesina = " " + texto + " ";  // Agregar espacios para mejor visualización
    ejecutando = true;
    pausado = false;
    posicion_actual = ancho_consola;
    
    hilo_marquesina = std::thread(&Marquesina::ejecutar_marquesina, this);
}

void Marquesina::detener() {
    ejecutando = false;
    if (hilo_marquesina.joinable()) {
        hilo_marquesina.join();
    }
    limpiar_linea_marquesina();
}

void Marquesina::pausar() {
    pausado = true;
}

void Marquesina::reanudar() {
    pausado = false;
}

void Marquesina::ejecutar_marquesina() {
    while (ejecutando.load()) {
        if (!pausado.load()) {
            actualizar_buffer_marquesina();
            renderizar_marquesina_segura();
            
            // Actualizar posición
            posicion_actual--;
            
            // Si el texto sale completamente por la izquierda, reiniciar desde la derecha
            if (posicion_actual < -(int)texto_marquesina.length()) {
                posicion_actual = ancho_consola;
            }
        }
        
        // Controlar la velocidad de la marquesina
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void Marquesina::obtener_tamaño_consola() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        ancho_consola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    } else {
        ancho_consola = 80; // Valor por defecto
    }
}

void Marquesina::actualizar_buffer_marquesina() {
    buffer_marquesina = std::string(ancho_consola, ' ');
    
    // Calcular qué parte del texto se puede mostrar
    int inicio_texto = std::max(0, -posicion_actual);
    int inicio_pantalla = std::max(0, posicion_actual);
    int longitud_visible = std::min((int)texto_marquesina.length() - inicio_texto, 
                                   ancho_consola - inicio_pantalla);
    
    if (longitud_visible > 0) {
        buffer_marquesina.replace(inicio_pantalla, longitud_visible, 
                                 texto_marquesina.substr(inicio_texto, longitud_visible));
    }
}

void Marquesina::renderizar_marquesina_segura() {
    std::lock_guard<std::mutex> lock(mutex_pantalla);
    
    // Guardar la posición actual del cursor y atributos de color
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD posicion_original = csbi.dwCursorPosition;
    WORD atributos_originales = csbi.wAttributes;
    
    // Ir a la línea superior
    COORD pos_superior = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos_superior);
    
    // Establecer colores para la marquesina
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                           FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE);
    
    // Escribir el buffer de la marquesina
    std::cout << buffer_marquesina << std::flush;
    
    // Restaurar colores y posición originales inmediatamente
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), atributos_originales);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicion_original);
}

void Marquesina::limpiar_linea_marquesina() {
    std::lock_guard<std::mutex> lock(mutex_pantalla);
    
    // Guardar la posición actual del cursor y atributos de color
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD posicion_original = csbi.dwCursorPosition;
    WORD atributos_originales = csbi.wAttributes;
    
    // Ir a la línea superior
    COORD pos_superior = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos_superior);
    
    // Limpiar la línea con espacios
    std::string linea_vacia(ancho_consola, ' ');
    std::cout << linea_vacia << std::flush;
    
    // Restaurar colores y posición originales
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), atributos_originales);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicion_original);
}
