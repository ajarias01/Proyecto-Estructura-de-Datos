/**
 * @file QR.cpp
 * @brief Implementación de la clase QR para generar códigos QR y documentos PDF.
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la implementación de la clase QR que permite generar
 * códigos QR con información de clientes y cuentas bancarias, así como documentos PDF
 * que incluyen tanto la información del cliente como el código QR generado.
 */

#include "QR.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Para system()

/**
 * @brief Constructor de la clase QR.
 * @param clientName Nombre completo del cliente.
 * @param accountNumber Número de cuenta bancaria del cliente.
 * @details Inicializa una instancia de QR con la información del cliente y número de cuenta
 * que será utilizada para generar el código QR y los documentos PDF asociados.
 */
QR::QR(const std::string& clientName, const std::string& accountNumber)
    : clientName(clientName), accountNumber(accountNumber) {}

/**
 * @brief Genera una imagen PNG con código QR conteniendo información del cliente.
 * @details Esta función crea un código QR que contiene el nombre del cliente y número de cuenta
 * separados por el carácter '|'. Utiliza ImageMagick para generar la imagen QR en formato PNG.
 * 
 * El proceso incluye:
 * - Construcción de datos concatenados (nombre|cuenta)
 * - Generación del nombre de archivo único basado en el número de cuenta
 * - Ejecución del comando ImageMagick con parámetros específicos
 * - Verificación de la creación exitosa del archivo
 * - Manejo de errores y mensajes informativos
 * 
 * @note Requiere ImageMagick instalado en la ruta especificada.
 * @warning Si ImageMagick no está instalado o la ruta es incorrecta, la función fallará.
 * @see https://imagemagick.org/ para información sobre ImageMagick
 */
void QR::generateQRImage() const {
    // Construcción de datos para el código QR: formato "nombre|numeroCuenta"
    std::string data = clientName + "|" + accountNumber;
    
    // Generación del nombre de archivo único basado en el número de cuenta
    std::string pngFilename = "qr_" + accountNumber + ".png";
    
    // Información de depuración: mostrar comando que se ejecutará
    std::cout << "Intentando generar QR con comando: \"C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI\\magick.exe\" -size 400x400 -density 400 QR:\"" << data << "\" " << pngFilename << std::endl;
    
    // Construcción del comando ImageMagick con parámetros específicos
    std::string command = "\"C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI\\magick.exe\" "
                         "-size 400x400 "      // Tamaño de imagen 400x400 píxeles
                         "-density 400 "       // Densidad de resolución
                         "QR:\"" + data + "\" " // Datos para el código QR
                         + pngFilename;         // Archivo de salida

    // Ejecución del comando del sistema
    int result = system(command.c_str());
    std::cout << "Comando ejecutado. Código de retorno: " << result << std::endl;
    
    // Verificación del éxito de la operación
    if (result == 0) {
        std::cout << "Imagen QR generada exitosamente como " << pngFilename << " en " << std::string(getenv("CD")) << std::endl;
        
        // Verificación adicional: comprobar que el archivo fue creado
        std::ifstream imgFile(pngFilename);
        if (imgFile.good()) {
            imgFile.close();
            std::cout << "Archivo verificado: " << pngFilename << " creado correctamente." << std::endl;
        } else {
            std::cerr << "Advertencia: El archivo " << pngFilename << " no se creó correctamente a pesar del éxito del comando." << std::endl;
        }
    } else {
        // Manejo de errores: informar sobre posibles causas del fallo
        std::cerr << "Error al generar el QR. Código de retorno: " << result << ". Esto puede indicar que ImageMagick no tiene soporte para QR o la ruta es incorrecta." << std::endl;
        std::cerr << "Por favor, verifica la instalación de ImageMagick o considera usar qrencode." << std::endl;
    }
}

/**
 * @brief Genera un documento PDF que incluye información del cliente y el código QR.
 * @details Esta función crea un documento PDF profesional utilizando LaTeX que contiene:
 * - Información del cliente (nombre y número de cuenta)
 * - Código QR previamente generado
 * - Formato profesional con encabezados y pie de página
 * 
 * El proceso incluye:
 * - Creación de archivo LaTeX (.tex) con formato predefinido
 * - Inclusión de paquetes LaTeX para UTF-8, geometría y gráficos
 * - Configuración de encabezados y pie de página
 * - Inserción de información del cliente y código QR
 * - Compilación a PDF usando latexmk
 * 
 * @note Requiere LaTeX (TeX Live) instalado en el sistema.
 * @warning El código QR debe haberse generado previamente con generateQRImage().
 * @see https://www.latex-project.org/ para información sobre LaTeX
 */
void QR::generatePDF() const {
    std::string texFilename = "QR_" + clientName + "_" + accountNumber + ".tex";
    std::ofstream texFile(texFilename);

    if (texFile.is_open()) {
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
        texFile << "\\section*{Información del Cliente}\n";
        texFile << "Nombre: " << clientName << "\n";
        texFile << "Número de Cuenta: " << accountNumber << "\n";
        texFile << "\\vspace{1cm}\n";
        texFile << "\\includegraphics[width=0.5\\textwidth]{qr_" << accountNumber << ".png}\n";
        texFile << "\\end{document}\n";
        texFile.close();

        std::string pdfCommand = "latexmk -pdf " + texFilename;
        int latexResult = system(pdfCommand.c_str());
        if (latexResult == 0) {
            std::cout << "PDF generado exitosamente como QR_" << clientName << "_" << accountNumber << ".pdf" << std::endl;
        } else {
            std::cerr << "Error al generar el PDF. Código de retorno: " << latexResult << ". Asegúrese de que latexmk esté instalado (instale TeX Live)." << std::endl;
        }
    } else {
        std::cerr << "No se pudo crear el archivo " << texFilename << std::endl;
    }
}

void QR::generateQRAndPDF() const {
    generateQRImage();
    generatePDF();
}