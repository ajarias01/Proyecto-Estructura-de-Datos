#include "pdf_generator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "hpdf.h"

using namespace std;

// Función para dividir una línea por el delimitador ';'
vector<string> split(const string &line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Función para crear el PDF con datos de clientes
void generateClientDataPDF(const std::string& inputFile) {
    // Crea el objeto de documento PDF
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cerr << "Error al crear el PDF" << endl;
        return;
    }

    // Crear una página
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Establecer la fuente
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    // Leer el archivo datos.txt
    ifstream file(inputFile);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo " << inputFile << endl;
        HPDF_Free(pdf);
        return;
    }

    // Establecer posición inicial en la página
    float yPosition = 750;
    float xPosition = 50;

    // Títulos de las columnas (ajustados para datos bancarios)
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, xPosition, yPosition, "Nombre");
    HPDF_Page_TextOut(page, xPosition + 150, yPosition, "ID Cuenta");
    HPDF_Page_TextOut(page, xPosition + 300, yPosition, "Saldo");
    HPDF_Page_EndText(page);

    // Dibuja la línea que separa los títulos de los datos
    yPosition -= 20;
    HPDF_Page_MoveTo(page, xPosition, yPosition);
    HPDF_Page_LineTo(page, xPosition + 400, yPosition);
    HPDF_Page_Stroke(page);

    // Leer las líneas del archivo datos.txt y escribir en el PDF
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> fields = split(line, ';');

        // Ajustar para datos bancarios (suponiendo Nombre;ID_Cuenta;Saldo)
        yPosition -= 20;
        if (yPosition < 100) {
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetFontAndSize(page, font, 12);
            yPosition = 750;
            // Repetir títulos en nuevas páginas
            HPDF_Page_BeginText(page);
            HPDF_Page_TextOut(page, xPosition, yPosition, "Nombre");
            HPDF_Page_TextOut(page, xPosition + 150, yPosition, "ID Cuenta");
            HPDF_Page_TextOut(page, xPosition + 300, yPosition, "Saldo");
            HPDF_Page_EndText(page);
            yPosition -= 20;
            HPDF_Page_MoveTo(page, xPosition, yPosition);
            HPDF_Page_LineTo(page, xPosition + 400, yPosition);
            HPDF_Page_Stroke(page);
            yPosition -= 20;
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, xPosition, yPosition, fields.size() > 0 ? fields[0].c_str() : "N/A");
        HPDF_Page_TextOut(page, xPosition + 150, yPosition, fields.size() > 1 ? fields[1].c_str() : "N/A");
        HPDF_Page_TextOut(page, xPosition + 300, yPosition, fields.size() > 2 ? fields[2].c_str() : "N/A");
        HPDF_Page_EndText(page);
    }

    file.close();

    // Guardar el PDF
    HPDF_SaveToFile(pdf, "clientes.pdf");

    // Liberar el objeto PDF
    HPDF_Free(pdf);

    cout << "PDF de clientes generado exitosamente como clientes.pdf" << endl;
}