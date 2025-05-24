#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

class Fecha {
public:
    Fecha();
    Fecha(int _anuario, int _mes, int _dia);
    Fecha(int _anuario, int _mes, int _dia, int _hora, int _minutos, int _segundos);
    static bool es_fecha_valida(int anio, int mes, int dia);
    int get_anuario();
    void set_anuario(int);
    int dias_en_mes(int anio, int mes);
    int get_mes();
    void set_mes(int);
    int get_dia();
    void set_dia(int);
    int get_hora();
    void set_hora(int);
    int get_minutos();
    void set_minutos(int);
    int get_segundos();
    void set_segundos(int);
    bool es_dia_habil();
    bool es_feriado();
    bool es_finde();
    bool validar() const;
    bool empty() const;
    std::string to_string() const; // Añadido const
    std::string to_string_documento();
    std::time_t string_to_fecha(const std::string& dateTime);
    void imprimir();
    bool operator<(const Fecha& other);
    bool operator>(const Fecha& other);
    bool operator==(const Fecha& other);
    bool operator<=(const Fecha& other); // Añadido
    bool operator>=(const Fecha& other); // Añadido
    friend std::ostream& operator<<(std::ostream& os, const Fecha& fecha); // Añadido
private:
    int dia;
    int mes;
    int anuario;
    int hora;
    int minutos;
    int segundos;
};

#endif