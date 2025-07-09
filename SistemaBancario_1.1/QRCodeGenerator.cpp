/**
 * @file QRCodeGenerator.cpp
 * @brief Implementación de la clase QRCodeGenerator para generar códigos QR y PDFs
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la implementación de la clase QRCodeGenerator que utiliza
 *          la biblioteca qrcodegen (versión en C) para generar códigos QR de alta calidad
 *          y libharu para crear un documento PDF con los códigos QR de todas las cuentas
 *          de un cliente. Cada página del PDF incluye el código QR y la información del
 *          cliente (nombre, número de cuenta, y tipo de cuenta: Ahorros o Corriente).
 */

#include "QRCodeGenerator.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "qrcodegen.h" // Versión en C
#include "hpdf.h"
#include "ListaDoble.h"
#include "Cuenta.h"
#include "Ahorro.h"
#include "Corriente.h"

using namespace std;

// Implementación de generateQRImage para la versión en C
void QRCodeGenerator::generateQRImage(const string& data, vector<uint8_t>& buffer, int& size) {
    // Buffer para el código QR
    uint8_t qr_buffer[qrcodegen_BUFFER_LEN_MAX];
    uint8_t temp_buffer[qrcodegen_BUFFER_LEN_MAX];

    // Generar el código QR
    bool ok = qrcodegen_encodeText(data.c_str(), temp_buffer, qr_buffer, qrcodegen_Ecc_MEDIUM, 
                                   qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, 
                                   qrcodegen_Mask_AUTO, true);
    if (!ok) {
        cerr << "Error al generar el código QR" << endl;
        size = 0;
        return;
    }

    // Obtener el tamaño del QR
    size = qrcodegen_getSize(qr_buffer);
    buffer.clear();
    buffer.resize(size * size);

    // Convertir a bitmap (1 = negro, 0 = blanco)
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            buffer[y * size + x] = qrcodegen_getModule(qr_buffer, x, y) ? 1 : 0;
        }
    }
}

// Constructor
QRCodeGenerator::QRCodeGenerator(const string& nombre, const ListaDoble<Cuenta*>* cuentas) : nombre(nombre) {
    cuentas->recorrer([&](Cuenta* cuenta) {
        string qr_data = nombre + " | Cuenta: " + cuenta->get_id_cuenta() + " | Tipo: " + cuenta->get_tipo();
        this->cuentas.emplace_back(cuenta->get_id_cuenta(), qr_data);
    });
}

// Implementación de generateQRAndPDF
void QRCodeGenerator::generateQRAndPDF() {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cerr << "Error al crear el PDF" << endl;
        return;
    }

    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    if (!font) {
        cerr << "Error al cargar la fuente Helvetica" << endl;
        HPDF_Free(pdf);
        return;
    }

    for (const auto& cuenta : cuentas) {
        HPDF_Page page = HPDF_AddPage(pdf);
        HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
        HPDF_Page_SetFontAndSize(page, font, 12);

        float xPosition = 50;
        float yPosition = 750;

        // Escribir información en el PDF
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, xPosition, yPosition, ("Cuenta: " + cuenta.first).c_str());
        HPDF_Page_TextOut(page, xPosition, yPosition - 20, ("Cliente: " + nombre).c_str());
        string tipo = cuenta.second.substr(cuenta.second.rfind("Tipo: ") + 6);
        HPDF_Page_TextOut(page, xPosition, yPosition - 40, ("Tipo de cuenta: " + tipo).c_str());
        HPDF_Page_EndText(page);

        // Generar el código QR
        vector<uint8_t> qr_buffer;
        int qr_size;
        generateQRImage(cuenta.second, qr_buffer, qr_size);

        // Dibujar el QR en el PDF
        float qr_x = xPosition;
        float qr_y = yPosition - 200;
        float module_size = 3.0; // Tamaño de cada módulo en puntos
        HPDF_Page_SetRGBFill(page, 0, 0, 0); // Color negro

        for (int y = 0; y < qr_size; y++) {
            for (int x = 0; x < qr_size; x++) {
                if (qr_buffer[y * qr_size + x]) {
                    HPDF_Page_Rectangle(page, qr_x + x * module_size, qr_y - y * module_size, module_size, module_size);
                    HPDF_Page_Fill(page);
                }
            }
        }
    }

    // Limpiar el nombre para el nombre del archivo
    string pdf_name = nombre;
    replace(pdf_name.begin(), pdf_name.end(), ' ', '_');
    pdf_name = "qr_cliente_" + pdf_name + ".pdf";

    HPDF_SaveToFile(pdf, pdf_name.c_str());
    HPDF_Free(pdf);
    cout << "PDF con QRs generado como " << pdf_name << endl;
}