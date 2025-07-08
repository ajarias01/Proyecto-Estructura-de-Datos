/**
 * @file pdf_generator.h
 * @brief Declaraciones para el generador de reportes PDF del sistema bancario.
 * @author Sistema Bancario
 * @date 2024
 * @details Este archivo contiene las declaraciones de funciones para generar reportes
 * en formato PDF con información de clientes y cuentas bancarias.
 */

#ifndef PDF_GENERATOR_H
#define PDF_GENERATOR_H

#include "ListaDoble.h" // Asegúrate de incluir esto
#include "Cliente.h"    // Para la declaración de Cliente
#include "Cuenta.h"     // Para la declaración de Cuenta
#include "hpdf.h"

/**
 * @brief Genera un reporte PDF con datos de clientes y sus cuentas bancarias.
 * @param clientes Puntero constante a la lista doblemente enlazada de clientes.
 * @details Función que crea un documento PDF tabular conteniendo información
 * completa de todos los clientes del sistema y sus respectivas cuentas bancarias.
 * El reporte incluye nombre completo, ID de cuenta y saldo actual.
 * 
 * @note Utiliza la librería Haru PDF para la generación del documento.
 * @note El archivo se guarda como "clientes.pdf" en el directorio de trabajo.
 * @warning Requiere que la librería libhpdf esté correctamente instalada y enlazada.
 * 
 * Características del PDF generado:
 * - Formato: A4 vertical
 * - Fuente: Helvetica 12pt
 * - Paginación automática
 * - Encabezados en cada página
 * - Formato tabular con columnas alineadas
 */
void generateClientDataPDF(const ListaDoble<Cliente*>* clientes);

#endif