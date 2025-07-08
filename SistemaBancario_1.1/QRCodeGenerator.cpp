#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "qrcodegen.h"     // Reemplaza qrcode.h con la biblioteca Nayuki
#include <cstdint>         // Para uint8_t
#include "QRCodeGenerator.h" // Para la definición de la clase
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

QRCodeGenerator::QRCodeGenerator(const std::string& clientName, const std::string& accountNumber)
    : clientName(clientName), accountNumber(accountNumber) {}

void QRCodeGenerator::generateQRAndPDF() const {
    std::string data = clientName + "|" + accountNumber;
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(data.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_LOW,
                                  qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (!ok) {
        std::cerr << "Error al codificar el codigo QR" << std::endl;
        return;
    }

    // Generar imagen PNG
    int size = qrcodegen_getSize(qrcode);
    unsigned char* image = new unsigned char[size * size];
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            image[y * size + x] = qrcodegen_getModule(qrcode, x, y) ? 0 : 255; // 0 = negro, 255 = blanco
        }
    }
    std::string pngFilename = "qr_" + accountNumber + ".png";
    stbi_write_png(pngFilename.c_str(), size, size, 1, image, size);

    // Verificar y liberar memoria de la imagen
    std::ifstream imgFile(pngFilename);
    if (imgFile.good()) {
        std::cout << "Imagen QR generada exitosamente como " << pngFilename << std::endl;
        imgFile.close();
    } else {
        std::cerr << "Advertencia: El archivo " << pngFilename << " no se creó correctamente." << std::endl;
    }
    delete[] image;

    // Generar PDF
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
        int result = system(pdfCommand.c_str());
        if (result == 0) {
            std::cout << "PDF generado exitosamente como QR_" << clientName << "_" + accountNumber << ".pdf" << std::endl;
        } else {
            std::cerr << "Error al generar el PDF. Código de retorno: " << result << ". Asegúrese de que latexmk esté instalado." << std::endl;
        }
    } else {
        std::cerr << "No se pudo crear el archivo " << texFilename << std::endl;
    }

    // No hay qrcodeBytes que liberar, ya que usamos buffers de Nayuki
}