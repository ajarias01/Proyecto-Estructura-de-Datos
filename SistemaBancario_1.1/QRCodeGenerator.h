#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <string>
#include <vector>
#include "ListaDoble.h"
#include "Cuenta.h"

class QRCodeGenerator {
private:
    std::string nombre;
    std::vector<std::pair<std::string, std::string>> cuentas;
    void generateQRImage(const std::string& data, std::vector<uint8_t>& buffer, int& size);

public:
    QRCodeGenerator(const std::string& nombre, const ListaDoble<Cuenta*>* cuentas);
    void generateQRAndPDF();
};

#endif