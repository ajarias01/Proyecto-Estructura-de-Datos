/**
 * @file QR.h
 * @brief Definición de la clase QR para generar códigos QR e informes en PDF
 * @author Sistema Bancario
 * @date 2024
 * @details Este archivo contiene la declaración de la clase QR que permite la generación
 *          de códigos QR personalizados para clientes bancarios y la creación de informes
 *          en formato PDF que incluyen información del cliente y el código QR generado.
 */

#ifndef QR_H
#define QR_H

#include <string>

/**
 * @class QR
 * @brief Clase para generar códigos QR e informes PDF para clientes bancarios
 * @details Esta clase permite crear códigos QR personalizados basados en información
 *          del cliente (nombre y número de cuenta) y generar documentos PDF que incluyen
 *          tanto la información del cliente como el código QR asociado.
 */
class QR {
private:
    /**
     * @brief Nombre del cliente para el cual se genera el código QR
     * @details Almacena el nombre completo del cliente que será incluido en el código QR
     *          y en el documento PDF generado
     */
    std::string clientName;
    
    /**
     * @brief Número de cuenta del cliente
     * @details Almacena el número de cuenta bancaria del cliente que será codificado
     *          en el código QR y mostrado en el documento PDF
     */
    std::string accountNumber;

public:
    /**
     * @brief Constructor de la clase QR
     * @param clientName Nombre del cliente
     * @param accountNumber Número de cuenta del cliente
     * @details Inicializa una instancia de QR con la información del cliente necesaria
     *          para generar códigos QR y documentos PDF personalizados
     */
    QR(const std::string& clientName, const std::string& accountNumber);
    
    /**
     * @brief Genera una imagen del código QR
     * @details Crea un archivo de imagen PNG con el código QR que contiene la información
     *          del cliente. La imagen se guarda en el directorio actual con un nombre
     *          basado en el número de cuenta del cliente.
     * @warning Requiere ImageMagick instalado en el sistema para la conversión de formatos
     */
    void generateQRImage() const;
    
    /**
     * @brief Genera un documento PDF con información del cliente
     * @details Crea un documento PDF que incluye la información del cliente, el código QR
     *          generado y formato profesional para presentación. El PDF se guarda con
     *          un nombre basado en el número de cuenta del cliente.
     * @warning Requiere LaTeX instalado en el sistema para la compilación del PDF
     */
    void generatePDF() const;
    
    /**
     * @brief Genera tanto el código QR como el documento PDF
     * @details Función de conveniencia que combina la generación del código QR y el
     *          documento PDF en una sola llamada. Ejecuta secuencialmente generateQRImage()
     *          y generatePDF().
     * @warning Requiere tanto ImageMagick como LaTeX instalados en el sistema
     */
    void generateQRAndPDF() const;
};

#endif