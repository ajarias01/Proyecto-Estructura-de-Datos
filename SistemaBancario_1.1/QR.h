#ifndef QR_H
#define QR_H

#include <string>

class QR {
private:
    std::string clientName;
    std::string accountNumber;

public:
    QR(const std::string& clientName, const std::string& accountNumber);
    void generateQRImage() const;
    void generatePDF() const;
    void generateQRAndPDF() const;
};

#endif