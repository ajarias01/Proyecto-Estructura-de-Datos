/**
 * @file pdf_generator.cpp
 * @brief Implementación del generador de reportes PDF para el sistema bancario.
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la implementación de funciones para generar reportes
 * en formato PDF con información de clientes y cuentas bancarias utilizando la librería Haru PDF.
 */

#include "pdf_generator.h"
#include <iostream>
#include <vector>
#include "ListaDoble.h" // Incluye la definición de ListaDoble
#include "Cliente.h"    // Incluye la definición de Cliente
#include "Cuenta.h"     // Incluye la definición de Cuenta
#include "hpdf.h"

using namespace std;

/**
 * @brief Genera un reporte PDF con datos de clientes y sus cuentas bancarias.
 * @param clientes Puntero constante a la lista doblemente enlazada de clientes.
 * @details Esta función crea un documento PDF que contiene un reporte tabular con
 * información de todos los clientes y sus cuentas bancarias. El reporte incluye:
 * - Nombre completo del cliente (nombres + apellidos)
 * - ID de cada cuenta bancaria
 * - Saldo actual de cada cuenta
 * 
 * El PDF se genera con formato A4 en orientación vertical, usando fuente Helvetica
 * de 12 puntos. Incluye paginación automática cuando el contenido excede el espacio
 * disponible en una página.
 * 
 * @note La función utiliza la librería Haru PDF (libhpdf) para la generación del documento.
 * @warning Si no se pueden cargar las fuentes o crear el PDF, la función termina prematuramente.
 */
void generateClientDataPDF(const ListaDoble<Cliente*>* clientes) {
    // Inicialización del documento PDF
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cerr << "Error al crear el PDF" << endl;
        return;
    }

    // Creación de la primera página con formato A4 vertical
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Configuración de la fuente Helvetica con tamaño 12
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    if (!font) {
        cerr << "Error al cargar la fuente Helvetica" << endl;
        HPDF_Free(pdf);
        return;
    }
    HPDF_Page_SetFontAndSize(page, font, 12);

    cout << "Procesando datos de clientes desde memoria..." << endl;

    // Configuración de posiciones iniciales para el contenido
    float yPosition = 750;  // Posición vertical inicial (desde arriba)
    float xPosition = 50;   // Margen izquierdo

    // Creación del encabezado de la tabla
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, xPosition, yPosition, "Nombre");
    HPDF_Page_TextOut(page, xPosition + 150, yPosition, "ID Cuenta");
    HPDF_Page_TextOut(page, xPosition + 300, yPosition, "Saldo");
    HPDF_Page_EndText(page);

    // Dibujo de línea separadora bajo el encabezado
    yPosition -= 20;
    HPDF_Page_MoveTo(page, xPosition, yPosition);
    HPDF_Page_LineTo(page, xPosition + 400, yPosition);
    HPDF_Page_Stroke(page);

    // Contador para el número total de entradas procesadas
    int clientCount = 0;
    
    // Procesamiento de todos los clientes y sus cuentas usando lambdas anidadas
    clientes->recorrer([&](Cliente* c) {
        // Para cada cliente, procesar todas sus cuentas
        c->get_cuentas()->recorrer([&](Cuenta* cuenta) {
            clientCount++;
            yPosition -= 20;
            
            // Verificación de espacio en página y creación de nueva página si es necesario
            if (yPosition < 100) {
                // Crear nueva página cuando se agota el espacio
                page = HPDF_AddPage(pdf);
                HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
                HPDF_Page_SetFontAndSize(page, font, 12);
                yPosition = 750;
                
                // Recrear encabezado en la nueva página
                HPDF_Page_BeginText(page);
                HPDF_Page_TextOut(page, xPosition, yPosition, "Nombre");
                HPDF_Page_TextOut(page, xPosition + 150, yPosition, "ID Cuenta");
                HPDF_Page_TextOut(page, xPosition + 300, yPosition, "Saldo");
                HPDF_Page_EndText(page);
                yPosition -= 20;
                
                // Recrear línea separadora
                HPDF_Page_MoveTo(page, xPosition, yPosition);
                HPDF_Page_LineTo(page, xPosition + 400, yPosition);
                HPDF_Page_Stroke(page);
                yPosition -= 20;
            }

            // Escritura de datos del cliente y cuenta en la página
            HPDF_Page_BeginText(page);
            string fullName = c->get_nombres() + " " + c->get_apellidos();
            const char* name = fullName.c_str();
            const char* account = cuenta->get_id_cuenta().c_str();
            string saldoStr = to_string(cuenta->get_saldo());
            const char* balance = saldoStr.c_str();
            
            // Debug: mostrar información que se está escribiendo
            cout << "Escribiendo: Nombre=" << name << ", ID=" << account << ", Saldo=" << balance << endl;
            
            // Posicionar texto en las columnas correspondientes
            HPDF_Page_TextOut(page, xPosition, yPosition, name);           // Columna nombre
            HPDF_Page_TextOut(page, xPosition + 150, yPosition, account);  // Columna ID cuenta
            HPDF_Page_TextOut(page, xPosition + 300, yPosition, balance);  // Columna saldo
            HPDF_Page_EndText(page);        });
    });

    // Verificación de datos procesados
    if (clientCount == 0) {
        cout << "No se encontraron datos en memoria" << endl;
    }

    // Guardado del documento PDF en archivo
    HPDF_SaveToFile(pdf, "clientes.pdf");
    
    // Liberación de memoria del documento PDF
    HPDF_Free(pdf);

    // Confirmación de éxito en la generación
    cout << "PDF de clientes generado exitosamente como clientes.pdf" << endl;
}