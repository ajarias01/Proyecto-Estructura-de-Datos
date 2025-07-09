
/**
 * @file QRCodeGenerator.cpp
 * @brief Implementación de la clase QRCodeGenerator para generar códigos QR usando Nayuki
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la implementación de la clase QRCodeGenerator que utiliza
 *          la biblioteca qrcodegen de Nayuki para generar códigos QR de alta calidad.
 *          Incluye funcionalidad para crear imágenes PNG usando stb_image_write y
 *          generar documentos PDF profesionales con LaTeX.
 */

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "qrcodegen.h"     // Reemplaza qrcode.h con la biblioteca Nayuki
#include <cstdint>         // Para uint8_t
#include "QRCodeGenerator.h" // Para la definición de la clase
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "qrcodegen.h" // Versión en C
#include "hpdf.h"
#include "ListaDoble.h"
#include "Cuenta.h"
#include "Ahorro.h"   // Añadido para definiciones de clase
#include "Corriente.h" // Añadido para definiciones de clase

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
/**
 * @brief Constructor de la clase QRCodeGenerator
 * @param clientName Nombre del cliente
 * @param accountNumber Número de cuenta del cliente
 * @details Inicializa una instancia del generador con la información del cliente
 *          necesaria para crear códigos QR y documentos PDF personalizados
 */
QRCodeGenerator::QRCodeGenerator(const std::string& clientName, const std::string& accountNumber)
    : clientName(clientName), accountNumber(accountNumber) {}

/**
 * @brief Genera código QR e informe PDF usando la biblioteca Nayuki
 * @details Crea un código QR de alta calidad usando qrcodegen de Nayuki y genera
 *          un documento PDF profesional. El proceso incluye:
 *          1. Codificación de datos del cliente en formato QR
 *          2. Generación de imagen PNG usando stb_image_write
 *          3. Creación de documento LaTeX con información del cliente
 *          4. Compilación del PDF usando latexmk
 * @warning Requiere las bibliotecas qrcodegen (Nayuki), stb_image_write y LaTeX
 *          instaladas en el sistema para funcionar correctamente
 */
void QRCodeGenerator::generateQRAndPDF() const {
    // Preparar datos del cliente para codificación QR
    std::string data = clientName + "|" + accountNumber;
    
    // Inicializar buffers para la generación del código QR usando Nayuki
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    
    // Generar código QR con configuración de corrección de errores baja
    bool ok = qrcodegen_encodeText(data.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_LOW,
                                  qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
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
    // Generar imagen PNG del código QR
    int size = qrcodegen_getSize(qrcode);
    unsigned char* image = new unsigned char[size * size];
    
    // Convertir datos QR a imagen monocromática (0=negro, 255=blanco)
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

    
    // Guardar imagen como archivo PNG
    std::string pngFilename = "qr_" + accountNumber + ".png";
    stbi_write_png(pngFilename.c_str(), size, size, 1, image, size);

    // Verificar creación exitosa del archivo PNG
    std::ifstream imgFile(pngFilename);
    if (imgFile.good()) {
        std::cout << "Imagen QR generada exitosamente como " << pngFilename << std::endl;
        imgFile.close();
    } else {
        std::cerr << "Advertencia: El archivo " << pngFilename << " no se creó correctamente." << std::endl;
    }
    
    // Liberar memoria de la imagen generada
    delete[] image;

    // Generar documento PDF con LaTeX
    std::string texFilename = "QR_" + clientName + "_" + accountNumber + ".tex";
    std::ofstream texFile(texFilename);
    if (texFile.is_open()) {
        // Escribir encabezado del documento LaTeX con configuración profesional
        texFile << "\\documentclass[a4paper]{article}\n";
        texFile << "\\usepackage[utf8]{inputenc}\n";
        texFile << "\\usepackage{geometry}\n";
        texFile << "\\geometry{a4paper, margin=1in}\n";
        texFile << "\\usepackage{graphicx}\n";
        texFile << "\\usepackage{fancyhdr}\n";
        texFile << "\\pagestyle{fancy}\n";
        texFile << "\\fancyhf{}\n";
        texFile << "\\lhead{Reporte QR - Banco}\n";
        texFile << "\\rhead{\\today}\n";
        texFile << "\\cfoot{Página \\thepage}\n";
        texFile << "\\begin{document}\n\n";
        
        // Contenido del documento con información del cliente
        texFile << "\\section*{Información del Cliente}\n";
        texFile << "Nombre: " << clientName << "\n";
        texFile << "Número de Cuenta: " << accountNumber << "\n";
        texFile << "\\vspace{1cm}\n";
        texFile << "\\includegraphics[width=0.5\\textwidth]{qr_" << accountNumber << ".png}\n";
        texFile << "\\end{document}\n";
        texFile.close();

        // Compilar documento LaTeX a PDF usando latexmk
        std::string pdfCommand = "latexmk -pdf " + texFilename;
        int result = system(pdfCommand.c_str());
        if (result == 0) {
            std::cout << "PDF generado exitosamente como QR_" << clientName << "_" + accountNumber << ".pdf" << std::endl;
        } else {
            std::cerr << "Error al generar el PDF. Código de retorno: " << result << ". Asegúrese de que latexmk esté instalado." << std::endl;
        }
    }

    // Limpiar el nombre para el nombre del archivo
    string pdf_name = nombre;
    replace(pdf_name.begin(), pdf_name.end(), ' ', '_');
    pdf_name = "qr_cliente_" + pdf_name + ".pdf";

    HPDF_SaveToFile(pdf, pdf_name.c_str());
    HPDF_Free(pdf);
    cout << "PDF con QRs generado como " << pdf_name << endl;
    // Nota: No hay necesidad de liberar memoria adicional para qrcodegen de Nayuki
    // ya que utiliza buffers estáticos proporcionados por el usuario
}