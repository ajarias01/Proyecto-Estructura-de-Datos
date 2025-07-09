/**
 * @file QRCodeGenerator.h
 * @brief Definición de la clase QRCodeGenerator para generar códigos QR usando la biblioteca Nayuki
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la declaración de la clase QRCodeGenerator que utiliza
 *          la biblioteca qrcodegen de Nayuki para generar códigos QR de alta calidad
 *          y crear documentos PDF profesionales para clientes bancarios.
 */

#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <string>
#include <vector>
#include "ListaDoble.h"
#include "Cuenta.h"

/**
 * @class QRCodeGenerator
 * @brief Generador de códigos QR usando la biblioteca Nayuki qrcodegen
 * @details Esta clase proporciona una implementación alternativa para generar códigos QR
 *          utilizando la biblioteca qrcodegen de Nayuki, que ofrece mayor control sobre
 *          la generación y mejor calidad de códigos QR. Incluye funcionalidad para
 *          generar imágenes PNG y documentos PDF con información del cliente.
 */
class QRCodeGenerator {
private:
<<<<<<< HEAD
    std::string nombre;
    std::vector<std::pair<std::string, std::string>> cuentas;
    void generateQRImage(const std::string& data, std::vector<uint8_t>& buffer, int& size);

public:
    QRCodeGenerator(const std::string& nombre, const ListaDoble<Cuenta*>* cuentas);
    void generateQRAndPDF();
    /**
     * @brief Nombre del cliente para el cual se genera el código QR
     * @details Almacena el nombre completo del cliente que será codificado en el QR
     *          y mostrado en el documento PDF generado
     */
    std::string clientName;
    
    /**
     * @brief Número de cuenta del cliente
     * @details Almacena el número de cuenta bancaria que será codificado en el QR
     *          y utilizado para nombrar los archivos generados
     */
    std::string accountNumber;

public:
    /**
     * @brief Constructor de la clase QRCodeGenerator
     * @param clientName Nombre del cliente
     * @param accountNumber Número de cuenta del cliente
     * @details Inicializa una instancia del generador con la información del cliente
     *          necesaria para crear códigos QR y documentos PDF personalizados
     */
    QRCodeGenerator(const std::string& clientName, const std::string& accountNumber);
    
    /**
     * @brief Genera código QR e informe PDF usando la biblioteca Nayuki
     * @details Crea un código QR de alta calidad usando qrcodegen de Nayuki y genera
     *          un documento PDF profesional que incluye la información del cliente
     *          y el código QR. La imagen se guarda como PNG y el PDF se compila con LaTeX.
     * @warning Requiere las bibliotecas qrcodegen (Nayuki), stb_image_write y LaTeX
     *          instaladas en el sistema para funcionar correctamente
     */
    void generateQRAndPDF() const;
};

#endif