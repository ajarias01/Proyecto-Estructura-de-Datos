/**
 * @file QRCodeGenerator.h
 * @brief Definición de la clase QRCodeGenerator para generar códigos QR y PDFs
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo contiene la definición de la clase QRCodeGenerator que utiliza
 *          la biblioteca qrcodegen (versión en C) para generar códigos QR y libharu
 *          para crear un documento PDF con los códigos QR de todas las cuentas de un cliente.
 *          Cada página del PDF incluye el código QR y la información del cliente (nombre,
 *          número de cuenta, y tipo de cuenta).
 */

#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <string>
#include <vector>
#include "ListaDoble.h"
#include "Cuenta.h"

class QRCodeGenerator {
private:
    std::string nombre; // Nombre del cliente
    std::vector<std::pair<std::string, std::string>> cuentas; // Lista de cuentas (ID y datos del QR)
    /**
     * @brief Genera una imagen QR a partir de los datos proporcionados
     * @param data Datos a codificar en el código QR
     * @param buffer Vector para almacenar la imagen QR como bitmap
     * @param size Tamaño del código QR generado
     */
    void generateQRImage(const std::string& data, std::vector<uint8_t>& buffer, int& size);

public:
    /**
     * @brief Constructor de la clase QRCodeGenerator
     * @param nombre Nombre completo del cliente
     * @param cuentas Lista de cuentas del cliente
     * @details Inicializa una instancia del generador con el nombre del cliente
     *          y la lista de cuentas para crear códigos QR para cada cuenta
     */
    QRCodeGenerator(const std::string& nombre, const ListaDoble<Cuenta*>* cuentas);

    /**
     * @brief Genera códigos QR y un documento PDF
     * @details Crea un código QR para cada cuenta del cliente usando qrcodegen y
     *          genera un documento PDF con libharu que incluye los códigos QR y
     *          la información del cliente (nombre, número de cuenta, tipo de cuenta).
     * @warning Requiere las bibliotecas qrcodegen y libharu instaladas
     */
    void generateQRAndPDF();
};

#endif