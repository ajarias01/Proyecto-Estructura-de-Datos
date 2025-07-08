/**
 * @file Fecha.cpp
 * @brief Implementación de la clase Fecha para el manejo de fechas y horas.
 *
 * Este archivo contiene la implementación de la clase Fecha, que permite manipular fechas,
 * validar días hábiles, feriados, conversiones y operaciones de comparación.
 */

#include "Fecha.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

/**
 * @brief Constructor por defecto. Inicializa la fecha y hora actual del sistema.
 */
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

/**
 * @brief Constructor de copia.
 * @param otro Fecha a copiar
 */
Fecha::Fecha(const Fecha& otro)
    : dia(otro.dia), mes(otro.mes), anuario(otro.anuario),
      hora(otro.hora), minutos(otro.minutos), segundos(otro.segundos) {}

/**
 * @brief Verifica si una fecha es válida.
 * @param anio Año
 * @param mes Mes
 * @param dia Día
 * @return true si la fecha es válida, false en caso contrario
 */
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

/**
 * @brief Establece la hora completa (hora, minutos, segundos).
 * @param _hora Hora
 * @param _minutos Minutos
 * @param _segundos Segundos
 */
void Fecha::set_hora_completa(int _hora, int _minutos, int _segundos) {
    hora = _hora;
    minutos = _minutos;
    segundos = _segundos;
}

/**
 * @brief Valida si la fecha actual es válida.
 * @return true si la fecha es válida, false en caso contrario
 */
bool Fecha::validar() const{
    return es_fecha_valida(anuario, mes, dia);
} 

/**
 * @brief Constructor parametrizado (solo fecha).
 * @param _anuario Año
 * @param _mes Mes
 * @param _dia Día
 */
Fecha::Fecha(int _anuario, int _mes, int _dia) {
    anuario = _anuario;
    mes = _mes;
    dia = _dia;
    hora = 0;
    minutos = 0;
    segundos = 1;
}

/**
 * @brief Constructor parametrizado (fecha y hora).
 * @param _anuario Año
 * @param _mes Mes
 * @param _dia Día
 * @param _hora Hora
 * @param _minutos Minutos
 * @param _segundos Segundos
 */
Fecha::Fecha(int _anuario, int _mes, int _dia, int _hora, int _minutos, int _segundos) {
    anuario = _anuario;
    mes = _mes;
    dia = _dia;
    hora = _hora;
    minutos = _minutos;
    segundos = _segundos;
}

/**
 * @brief Obtiene el año.
 * @return Año
 */
int Fecha::get_anuario() const { return anuario; }
/**
 * @brief Establece el año.
 * @param nuevo_anuario Nuevo año
 */
void Fecha::set_anuario(int nuevo_anuario) { anuario = nuevo_anuario; }
/**
 * @brief Obtiene el mes.
 * @return Mes
 */
int Fecha::get_mes() const { return mes; }
/**
 * @brief Establece el mes, ajustando automáticamente si es necesario.
 * @param nuevo_mes Nuevo mes
 */
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

/**
 * @brief Establece el día, ajustando automáticamente si es necesario.
 * @param nuevo_dia Nuevo día
 */
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

/**
 * @brief Obtiene el día.
 * @return Día
 */
int Fecha::get_dia() const { return dia; }

/**
 * @brief Devuelve la cantidad de días en un mes y año dados.
 * @param anio Año
 * @param mes Mes
 * @return Número de días en el mes
 */
int Fecha::dias_en_mes(int anio, int mes) {
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) {
        return 29;
    }
    return dias[mes - 1];
}

/**
 * @brief Obtiene la hora.
 * @return Hora
 */
int Fecha::get_hora() { return hora; }
/**
 * @brief Establece la hora.
 * @param nueva_hora Nueva hora
 */
void Fecha::set_hora(int nueva_hora) { hora = nueva_hora; }
/**
 * @brief Obtiene los minutos.
 * @return Minutos
 */
int Fecha::get_minutos() { return minutos; }
/**
 * @brief Establece los minutos.
 * @param nuevo_minutos Nuevos minutos
 */
void Fecha::set_minutos(int nuevo_minutos) { minutos = nuevo_minutos; }
/**
 * @brief Obtiene los segundos.
 * @return Segundos
 */
int Fecha::get_segundos() { return segundos; }
/**
 * @brief Establece los segundos.
 * @param nuevo_segundos Nuevos segundos
 */
void Fecha::set_segundos(int nuevo_segundos) { segundos = nuevo_segundos; }

/**
 * @brief Devuelve la fecha en formato string (YYYY-MM-DD).
 * @return Fecha como string
 */
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

/**
 * @brief Devuelve la fecha y hora en formato string para documentos (YYYY-MM-DDT_HH-MM-SSZ).
 * @return Fecha y hora como string
 */
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

/**
 * @brief Convierte un string a time_t (YYYY-MM-DD).
 * @param dateTime Fecha en string
 * @return time_t correspondiente
 */
std::time_t Fecha::string_to_fecha(const std::string& dateTime) {
    std::tm tm = {};
    std::istringstream ss(dateTime);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::mktime(&tm); // Retorna -1 si falla, sin imprimir errores
}

/**
 * @brief Imprime la fecha en formato DD/MM/YYYY.
 */
void Fecha::imprimir() {
    printf("%02d/%02d/%04d", dia, mes, anuario);
}

/**
 * @brief Verifica si la fecha es un día hábil (no fin de semana ni feriado).
 * @return true si es día hábil, false en caso contrario
 */
bool Fecha::es_dia_habil() {
    return !(es_finde() || es_feriado());
}

/**
 * @brief Verifica si la fecha está vacía (sin datos).
 * @return true si la fecha está vacía, false en caso contrario
 */
bool Fecha::empty() const {
    return dia == 0 || mes == 0 || anuario == 0;
}

/**
 * @brief Verifica si la fecha es un feriado nacional.
 * @return true si es feriado, false en caso contrario
 */
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

/**
 * @brief Verifica si la fecha es fin de semana.
 * @return true si es sábado o domingo, false en caso contrario
 */
bool Fecha::es_finde() {
    std::tm tm_fecha = {};
    tm_fecha.tm_mday = dia;
    tm_fecha.tm_mon = mes - 1;
    tm_fecha.tm_year = anuario - 1900;
    std::mktime(&tm_fecha);
    int dia_semana = tm_fecha.tm_wday;
    return (dia_semana == 0 || dia_semana == 6);
}

/**
 * @brief Operador menor que para comparar fechas.
 * @param other Fecha a comparar
 * @return true si esta fecha es menor que la otra
 */
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

/**
 * @brief Operador mayor que para comparar fechas.
 * @param other Fecha a comparar
 * @return true si esta fecha es mayor que la otra
 */
bool Fecha::operator>(const Fecha& other) {
    return !(*this < other || *this == other);
}

/**
 * @brief Operador de igualdad para comparar fechas.
 * @param other Fecha a comparar
 * @return true si las fechas son iguales
 */
bool Fecha::operator==(const Fecha& other) {
    return anuario == other.anuario && mes == other.mes && dia == other.dia &&
           hora == other.hora && minutos == other.minutos && segundos == other.segundos;
}

/**
 * @brief Operador menor o igual que para comparar fechas.
 * @param other Fecha a comparar
 * @return true si esta fecha es menor o igual que la otra
 */
bool Fecha::operator<=(const Fecha& other) {
    return *this < other || *this == other;
}

/**
 * @brief Operador mayor o igual que para comparar fechas.
 * @param other Fecha a comparar
 * @return true si esta fecha es mayor o igual que la otra
 */
bool Fecha::operator>=(const Fecha& other) {
    return *this > other || *this == other;
}

/**
 * @brief Operador de inserción para imprimir la fecha en un flujo de salida.
 * @param os Flujo de salida
 * @param fecha Fecha a imprimir
 * @return Referencia al flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Fecha& fecha) {
    os << fecha.to_string();
    return os;
}