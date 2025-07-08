#include "pdf_generator.h"
  #include <iostream>
  #include <vector>
  #include "ListaDoble.h" // Incluye la definición de ListaDoble
  #include "Cliente.h"    // Incluye la definición de Cliente
  #include "Cuenta.h"     // Incluye la definición de Cuenta
  #include "hpdf.h"

  using namespace std;

  // Función para crear el PDF con datos de clientes desde la lista en memoria
  void generateClientDataPDF(const ListaDoble<Cliente*>* clientes) {
      HPDF_Doc pdf = HPDF_New(NULL, NULL);
      if (!pdf) {
          cerr << "Error al crear el PDF" << endl;
          return;
      }

      HPDF_Page page = HPDF_AddPage(pdf);
      HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

      HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
      if (!font) {
          cerr << "Error al cargar la fuente Helvetica" << endl;
          HPDF_Free(pdf);
          return;
      }
      HPDF_Page_SetFontAndSize(page, font, 12);

      cout << "Procesando datos de clientes desde memoria..." << endl;

      float yPosition = 750;
      float xPosition = 50;

      HPDF_Page_BeginText(page);
      HPDF_Page_TextOut(page, xPosition, yPosition, "Nombre");
      HPDF_Page_TextOut(page, xPosition + 150, yPosition, "ID Cuenta");
      HPDF_Page_TextOut(page, xPosition + 300, yPosition, "Saldo");
      HPDF_Page_EndText(page);

      yPosition -= 20;
      HPDF_Page_MoveTo(page, xPosition, yPosition);
      HPDF_Page_LineTo(page, xPosition + 400, yPosition);
      HPDF_Page_Stroke(page);

      int clientCount = 0;
      clientes->recorrer([&](Cliente* c) {
          c->get_cuentas()->recorrer([&](Cuenta* cuenta) {
              clientCount++;
              yPosition -= 20;
              if (yPosition < 100) {
                  page = HPDF_AddPage(pdf);
                  HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
                  HPDF_Page_SetFontAndSize(page, font, 12);
                  yPosition = 750;
                  HPDF_Page_BeginText(page);
                  HPDF_Page_TextOut(page, xPosition, yPosition, "Nombre");
                  HPDF_Page_TextOut(page, xPosition + 150, yPosition, "ID Cuenta");
                  HPDF_Page_TextOut(page, xPosition + 300, yPosition, "Saldo");
                  HPDF_Page_EndText(page);
                  yPosition -= 20;
                  HPDF_Page_MoveTo(page, xPosition, yPosition);
                  HPDF_Page_LineTo(page, xPosition + 400, yPosition);
                  HPDF_Page_Stroke(page);
                  yPosition -= 20;
              }

              HPDF_Page_BeginText(page);
              string fullName = c->get_nombres() + " " + c->get_apellidos();
              const char* name = fullName.c_str();
              const char* account = cuenta->get_id_cuenta().c_str();
              string saldoStr = to_string(cuenta->get_saldo());
              const char* balance = saldoStr.c_str();
              cout << "Escribiendo: Nombre=" << name << ", ID=" << account << ", Saldo=" << balance << endl;
              HPDF_Page_TextOut(page, xPosition, yPosition, name);
              HPDF_Page_TextOut(page, xPosition + 150, yPosition, account);
              HPDF_Page_TextOut(page, xPosition + 300, yPosition, balance);
              HPDF_Page_EndText(page);
          });
      });

      if (clientCount == 0) {
          cout << "No se encontraron datos en memoria" << endl;
      }

      HPDF_SaveToFile(pdf, "clientes.pdf");
      HPDF_Free(pdf);

      cout << "PDF de clientes generado exitosamente como clientes.pdf" << endl;
  }