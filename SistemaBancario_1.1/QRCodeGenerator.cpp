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
#include <cstdlib>

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
        std::cerr << "Error al codificar el codigo QR" << std::endl;
        return;
    }

    // Generar imagen PNG del código QR
    int size = qrcodegen_getSize(qrcode);
    unsigned char* image = new unsigned char[size * size];
    
    // Convertir datos QR a imagen monocromática (0=negro, 255=blanco)
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            image[y * size + x] = qrcodegen_getModule(qrcode, x, y) ? 0 : 255; // 0 = negro, 255 = blanco
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
    } else {
        std::cerr << "No se pudo crear el archivo " << texFilename << std::endl;
    }

    // Nota: No hay necesidad de liberar memoria adicional para qrcodegen de Nayuki
    // ya que utiliza buffers estáticos proporcionados por el usuario
}