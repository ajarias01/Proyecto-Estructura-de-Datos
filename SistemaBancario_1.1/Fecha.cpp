#include "Fecha.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

Fecha::Fecha() {
    std::time_t tiempo_actual = std::time(nullptr);
    std::tm tm_actual = *std::localtime(&tiempo_actual);
    std::mktime(&tm_actual);
    dia = tm_actual.tm_mday;
    mes = tm_actual.tm_mon + 1;
    anuario = tm_actual.tm_year + 1900;
    hora = tm_actual.tm_hour;
    minutos = tm_actual.tm_min;
    segundos = tm_actual.tm_sec;
}
Fecha::Fecha(const Fecha& otro)
    : dia(otro.dia), mes(otro.mes), anuario(otro.anuario),
      hora(otro.hora), minutos(otro.minutos), segundos(otro.segundos) {}
bool Fecha::es_fecha_valida(int anio, int mes, int dia) {
    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    
    int dias_en_mes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) {
        dias_en_mes[1] = 29;
    }

    if (dia < 1 || dia > dias_en_mes[mes - 1]) return false;

    return true;
}
void Fecha::set_hora_completa(int _hora, int _minutos, int _segundos) {
    hora = _hora;
    minutos = _minutos;
    segundos = _segundos;
}

bool Fecha::validar() const{
    return es_fecha_valida(anuario, mes, dia);
} 

Fecha::Fecha(int _anuario, int _mes, int _dia) {
    anuario = _anuario;
    mes = _mes;
    dia = _dia;
    hora = 0;
    minutos = 0;
    segundos = 1;
}

Fecha::Fecha(int _anuario, int _mes, int _dia, int _hora, int _minutos, int _segundos) {
    anuario = _anuario;
    mes = _mes;
    dia = _dia;
    hora = _hora;
    minutos = _minutos;
    segundos = _segundos;
}

int Fecha::get_anuario() const { return anuario; }
void Fecha::set_anuario(int nuevo_anuario) { anuario = nuevo_anuario; }
int Fecha::get_mes() const { return mes; }

void Fecha::set_mes(int nuevo_mes) {
    if (nuevo_mes < 1 || nuevo_mes > 12) {
        // Ajusta automáticamente el mes (ej: 13 → 1, año +1)
        int meses_pasados = nuevo_mes / 12;
        mes = nuevo_mes % 12;
        if (mes == 0) mes = 12;
        anuario += meses_pasados;
    } else {
        mes = nuevo_mes;
    }
}

void Fecha::set_dia(int nuevo_dia) {
    int dias_en_mes = Fecha::dias_en_mes(anuario, mes);
    if (nuevo_dia > dias_en_mes || nuevo_dia < 1) {
        // Ajusta automáticamente el día (ej: 32 → 1, mes +1)
        dia = nuevo_dia - dias_en_mes;
        set_mes(mes + 1);
    } else {
        dia = nuevo_dia;
    }
}

int Fecha::get_dia() const { return dia; }

int Fecha::dias_en_mes(int anio, int mes) {
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) {
        return 29;
    }
    return dias[mes - 1];
}

int Fecha::get_hora() { return hora; }
void Fecha::set_hora(int nueva_hora) { hora = nueva_hora; }
int Fecha::get_minutos() { return minutos; }
void Fecha::set_minutos(int nuevo_minutos) { minutos = nuevo_minutos; }
int Fecha::get_segundos() { return segundos; }
void Fecha::set_segundos(int nuevo_segundos) { segundos = nuevo_segundos; }

std::string Fecha::to_string() const {
    std::tm tm_fecha = {};
    tm_fecha.tm_mday = dia;
    tm_fecha.tm_mon = mes - 1;
    tm_fecha.tm_year = anuario - 1900;
    std::mktime(&tm_fecha);
    std::ostringstream oss;
    oss << std::put_time(&tm_fecha, "%Y-%m-%d");
    return oss.str();
}

std::string Fecha::to_string_documento() {
    std::tm tm_fecha = {};
    tm_fecha.tm_mday = dia;
    tm_fecha.tm_mon = mes - 1;
    tm_fecha.tm_year = anuario - 1900;
    tm_fecha.tm_hour = hora;
    tm_fecha.tm_min = minutos;
    tm_fecha.tm_sec = segundos;
    std::mktime(&tm_fecha);
    std::ostringstream oss;
    oss << std::put_time(&tm_fecha, "%Y-%m-%dT_%H-%M-%SZ");
    return oss.str();
}

std::time_t Fecha::string_to_fecha(const std::string& dateTime) {
    std::tm tm = {};
    std::istringstream ss(dateTime);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::mktime(&tm); // Retorna -1 si falla, sin imprimir errores
}

void Fecha::imprimir() {
    printf("%02d/%02d/%04d", dia, mes, anuario);
}

bool Fecha::es_dia_habil() {
    return !(es_finde() || es_feriado());
}
bool Fecha::empty() const {
    return dia == 0 || mes == 0 || anuario == 0;
}

bool Fecha::es_feriado() {
    if (mes == 1 && dia == 1) return true;
    if (mes == 2 && (dia == 12 || dia == 13)) return true;
    if (mes == 3 && dia == 29) return true;
    if (mes == 5 && (dia == 1 || dia == 24)) return true;
    if (mes == 8 && dia == 10) return true;
    if (mes == 9 && dia == 26) return true;
    if (mes == 10 && dia == 9) return true;
    if (mes == 11 && (dia == 2 || dia == 3)) return true;
    if (mes == 12 && (dia == 24 || dia == 25 || dia == 31)) return true;
    return false;
}

bool Fecha::es_finde() {
    std::tm tm_fecha = {};
    tm_fecha.tm_mday = dia;
    tm_fecha.tm_mon = mes - 1;
    tm_fecha.tm_year = anuario - 1900;
    std::mktime(&tm_fecha);
    int dia_semana = tm_fecha.tm_wday;
    return (dia_semana == 0 || dia_semana == 6);
}

bool Fecha::operator<(const Fecha& other) {
    if (anuario < other.anuario) return true;
    if (anuario == other.anuario) {
        if (mes < other.mes) return true;
        if (mes == other.mes) {
            if (dia < other.dia) return true;
            if (dia == other.dia) {
                if (hora < other.hora) return true;
                if (hora == other.hora) {
                    if (minutos < other.minutos) return true;
                    if (minutos == other.minutos) {
                        return segundos < other.segundos;
                    }
                }
            }
        }
    }
    return false;
}

bool Fecha::operator>(const Fecha& other) {
    return !(*this < other || *this == other);
}

bool Fecha::operator==(const Fecha& other) {
    return anuario == other.anuario && mes == other.mes && dia == other.dia &&
           hora == other.hora && minutos == other.minutos && segundos == other.segundos;
}

bool Fecha::operator<=(const Fecha& other) {
    return *this < other || *this == other;
}

bool Fecha::operator>=(const Fecha& other) {
    return *this > other || *this == other;
}

std::ostream& operator<<(std::ostream& os, const Fecha& fecha) {
    os << fecha.to_string();
    return os;
}