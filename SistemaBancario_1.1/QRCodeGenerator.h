#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <string>

class QRCodeGenerator {
private:
    std::string clientName;
    std::string accountNumber;

public:
    QRCodeGenerator(const std::string& clientName, const std::string& accountNumber);
    void generateQRAndPDF() const;
};

#endif