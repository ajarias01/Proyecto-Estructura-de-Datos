#include "QR.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // Para system()

QR::QR(const std::string& clientName, const std::string& accountNumber)
    : clientName(clientName), accountNumber(accountNumber) {}

void QR::generateQRImage() const {
    std::string data = clientName + "|" + accountNumber;
    std::string pngFilename = "qr_" + accountNumber + ".png";
    
    std::cout << "Intentando generar QR con comando: \"C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI\\magick.exe\" -size 400x400 -density 400 QR:\"" << data << "\" " << pngFilename << std::endl;
    
    std::string command = "\"C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI\\magick.exe\" "
                         "-size 400x400 "
                         "-density 400 "
                         "QR:\"" + data + "\" "
                         + pngFilename;

    int result = system(command.c_str());
    std::cout << "Comando ejecutado. Código de retorno: " << result << std::endl; // Mostrar el código de retorno
    if (result == 0) {
        std::cout << "Imagen QR generada exitosamente como " << pngFilename << " en " << std::string(getenv("CD")) << std::endl;
        std::ifstream imgFile(pngFilename);
        if (imgFile.good()) {
            imgFile.close();
            std::cout << "Archivo verificado: " << pngFilename << " creado correctamente." << std::endl;
        } else {
            std::cerr << "Advertencia: El archivo " << pngFilename << " no se creó correctamente a pesar del éxito del comando." << std::endl;
        }
    } else {
        std::cerr << "Error al generar el QR. Código de retorno: " << result << ". Esto puede indicar que ImageMagick no tiene soporte para QR o la ruta es incorrecta." << std::endl;
        std::cerr << "Por favor, verifica la instalación de ImageMagick o considera usar qrencode." << std::endl;
    }
}

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