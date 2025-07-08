// pdf_generator.h
#ifndef PDF_GENERATOR_H
#define PDF_GENERATOR_H

#include "ListaDoble.h" // Asegúrate de incluir esto
#include "Cliente.h"    // Para la declaración de Cliente
#include "Cuenta.h"     // Para la declaración de Cuenta
#include "hpdf.h"

void generateClientDataPDF(const ListaDoble<Cliente*>* clientes);

#endif