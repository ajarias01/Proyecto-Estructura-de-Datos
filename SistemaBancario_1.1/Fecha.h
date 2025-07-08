/**
 * @file Fecha.h
 * @brief Definición de la clase Fecha para el manejo de fechas y horas.
 *
 * Esta clase permite manipular fechas, horas y realizar validaciones, conversiones y comparaciones.
 */

#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

/**
 * @class Fecha
 * @brief Clase para el manejo y validación de fechas y horas.
 *
 * Permite crear, comparar, validar y convertir fechas y horas, así como detectar días hábiles, feriados y fines de semana.
 */
class Fecha {
public:
    /**
     * @brief Constructor por defecto. Inicializa la fecha y hora actual del sistema.
     */
    Fecha();
    /**
     * @brief Constructor de copia.
     * @param otro Fecha a copiar
     */
    Fecha(const Fecha& otro);
    /**
     * @brief Constructor parametrizado (solo fecha).
     * @param _anuario Año
     * @param _mes Mes
     * @param _dia Día
     */
    Fecha(int _anuario, int _mes, int _dia);
    /**
     * @brief Constructor parametrizado (fecha y hora).
     * @param _anuario Año
     * @param _mes Mes
     * @param _dia Día
     * @param _hora Hora
     * @param _minutos Minutos
     * @param _segundos Segundos
     */
    Fecha(int _anuario, int _mes, int _dia, int _hora, int _minutos, int _segundos);
    /**
     * @brief Verifica si una fecha es válida.
     * @param anio Año
     * @param mes Mes
     * @param dia Día
     * @return true si la fecha es válida, false en caso contrario
     */
    static bool es_fecha_valida(int anio, int mes, int dia);
    /**
     * @brief Obtiene el año.
     * @return Año
     */
    int get_anuario() const;
    /**
     * @brief Establece el año.
     * @param nuevo_anuario Nuevo año
     */
    void set_anuario(int);
    /**
     * @brief Devuelve la cantidad de días en un mes y año dados.
     * @param anio Año
     * @param mes Mes
     * @return Número de días en el mes
     */
    int dias_en_mes(int anio, int mes);
    /**
     * @brief Obtiene el mes.
     * @return Mes
     */
    int get_mes() const;
    /**
     * @brief Establece el mes, ajustando automáticamente si es necesario.
     * @param nuevo_mes Nuevo mes
     */
    void set_mes(int);
    /**
     * @brief Obtiene el día.
     * @return Día
     */
    int get_dia() const;
    /**
     * @brief Establece el día, ajustando automáticamente si es necesario.
     * @param nuevo_dia Nuevo día
     */
    void set_dia(int);
    /**
     * @brief Obtiene la hora.
     * @return Hora
     */
    int get_hora();
    /**
     * @brief Establece la hora.
     * @param nueva_hora Nueva hora
     */
    void set_hora(int);
    /**
     * @brief Obtiene los minutos.
     * @return Minutos
     */
    int get_minutos();
    /**
     * @brief Establece los minutos.
     * @param nuevo_minutos Nuevos minutos
     */
    void set_minutos(int);
    /**
     * @brief Obtiene los segundos.
     * @return Segundos
     */
    int get_segundos();
    /**
     * @brief Establece los segundos.
     * @param nuevo_segundos Nuevos segundos
     */
    void set_segundos(int);
    /**
     * @brief Verifica si la fecha es un día hábil (no fin de semana ni feriado).
     * @return true si es día hábil, false en caso contrario
     */
    bool es_dia_habil();
    /**
     * @brief Verifica si la fecha es un feriado nacional.
     * @return true si es feriado, false en caso contrario
     */
    bool es_feriado();
    /**
     * @brief Verifica si la fecha es fin de semana.
     * @return true si es sábado o domingo, false en caso contrario
     */
    bool es_finde();
    /**
     * @brief Valida si la fecha actual es válida.
     * @return true si la fecha es válida, false en caso contrario
     */
    bool validar() const;
    /**
     * @brief Verifica si la fecha está vacía (sin datos).
     * @return true si la fecha está vacía, false en caso contrario
     */
    bool empty() const;
    /**
     * @brief Establece la hora completa (hora, minutos, segundos).
     * @param _hora Hora
     * @param _minutos Minutos
     * @param _segundos Segundos
     */
    void set_hora_completa(int _hora, int _minutos, int _segundos);
    /**
     * @brief Devuelve la fecha en formato string (YYYY-MM-DD).
     * @return Fecha como string
     */
    std::string to_string() const;
    /**
     * @brief Devuelve la fecha y hora en formato string para documentos (YYYY-MM-DDT_HH-MM-SSZ).
     * @return Fecha y hora como string
     */
    std::string to_string_documento();
    /**
     * @brief Convierte un string a time_t (YYYY-MM-DD).
     * @param dateTime Fecha en string
     * @return time_t correspondiente
     */
    std::time_t string_to_fecha(const std::string& dateTime);
    /**
     * @brief Imprime la fecha en formato DD/MM/YYYY.
     */
    void imprimir();
    /**
     * @brief Operador menor que para comparar fechas.
     * @param other Fecha a comparar
     * @return true si esta fecha es menor que la otra
     */
    bool operator<(const Fecha& other);
    /**
     * @brief Operador mayor que para comparar fechas.
     * @param other Fecha a comparar
     * @return true si esta fecha es mayor que la otra
     */
    bool operator>(const Fecha& other);
    /**
     * @brief Operador de igualdad para comparar fechas.
     * @param other Fecha a comparar
     * @return true si las fechas son iguales
     */
    bool operator==(const Fecha& other);
    /**
     * @brief Operador menor o igual que para comparar fechas.
     * @param other Fecha a comparar
     * @return true si esta fecha es menor o igual que la otra
     */
    bool operator<=(const Fecha& other);
    /**
     * @brief Operador mayor o igual que para comparar fechas.
     * @param other Fecha a comparar
     * @return true si esta fecha es mayor o igual que la otra
     */
    bool operator>=(const Fecha& other);
    /**
     * @brief Operador de inserción para imprimir la fecha en un flujo de salida.
     * @param os Flujo de salida
     * @param fecha Fecha a imprimir
     * @return Referencia al flujo de salida
     */
    friend std::ostream& operator<<(std::ostream& os, const Fecha& fecha);
private:
    int dia; ///< Día del mes
    int mes; ///< Mes del año
    int anuario; ///< Año
    int hora; ///< Hora
    int minutos; ///< Minutos
    int segundos; ///< Segundos
};

#endif