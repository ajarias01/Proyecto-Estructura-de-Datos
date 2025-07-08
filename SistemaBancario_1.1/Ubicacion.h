/**
 * @file Ubicacion.h
 * @brief Cabecera del sistema de geolocalización y gestión de sucursales bancarias
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo define las clases y funciones necesarias para implementar
 *          un sistema de geolocalización bancaria que permite localizar sucursales
 *          cercanas al usuario, gestionar colas de atención, calcular distancias
 *          geográficas y validar horarios bancarios en la ciudad de Quito, Ecuador.
 */

#ifndef UBICACION_H
#define UBICACION_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief Declaración forward para la función de menú con navegación por flechas
 * @param titulo Título del menú a mostrar
 * @param opciones Array de strings con las opciones del menú
 * @param n Número de opciones disponibles
 * @param fila_inicio Fila inicial para mostrar el menú
 * @return Índice de la opción seleccionada por el usuario
 * @note Esta función está implementada en el módulo Menus.cpp
 */
int seleccionar_opcion(const char *titulo, const char *opciones[], int n, int fila_inicio);

/**
 * @brief Clase para almacenar coordenadas de ubicación geográfica
 * @details Esta clase encapsula las coordenadas geográficas de un punto específico
 *          en el planeta, incluyendo latitud, longitud y datos de localización
 *          como ciudad y país. Utiliza coordenadas en formato decimal.
 */
class Location {
public:
    double latitude;    ///< Latitud en grados decimales (positivo = Norte, negativo = Sur)
    double longitude;   ///< Longitud en grados decimales (positivo = Este, negativo = Oeste)
    string city;        ///< Nombre de la ciudad
    string country;     ///< Nombre del país
    
    /**
     * @brief Constructor de la clase Location
     * @param lat Latitud en grados decimales (por defecto 0.0)
     * @param lon Longitud en grados decimales (por defecto 0.0)
     * @param c Nombre de la ciudad (por defecto cadena vacía)
     * @param co Nombre del país (por defecto cadena vacía)
     * @details Constructor que permite inicializar una ubicación geográfica
     *          con coordenadas específicas y datos de localización opcionales
     */
    Location(double lat = 0.0, double lon = 0.0, const string& c = "", const string& co = "");
};

/**
 * @brief Clase para almacenar información completa de sucursales bancarias
 * @details Esta clase encapsula toda la información relevante de una sucursal
 *          bancaria, incluyendo identificación, ubicación geográfica, estado
 *          de la cola de atención y horarios disponibles para citas.
 */
class Branch {
public:
    int id;                        ///< Identificador único de la sucursal
    string name;                   ///< Nombre de la sucursal
    string address;                ///< Dirección física de la sucursal
    Location location;             ///< Ubicación geográfica de la sucursal
    int queue_position;            ///< Número de personas en cola de espera
    vector<string> available_slots; ///< Horarios disponibles para citas
    
    /**
     * @brief Constructor de la clase Branch
     * @param id Identificador único de la sucursal
     * @param name Nombre de la sucursal
     * @param address Dirección física de la sucursal
     * @param lat Latitud de la ubicación de la sucursal
     * @param lon Longitud de la ubicación de la sucursal
     * @param queue Número inicial de personas en cola (por defecto 0)
     * @details Constructor que inicializa una sucursal bancaria con toda
     *          la información necesaria para su identificación y localización
     */
    Branch(int id, const string& name, const string& address, 
           double lat, double lon, int queue = 0);
};

/**
 * @brief Sistema de geolocalización y gestión de sucursales bancarias
 * @details Esta clase implementa un sistema completo de geolocalización para
 *          sucursales bancarias que permite localizar las sucursales más cercanas
 *          al usuario, gestionar colas de atención, validar horarios bancarios
 *          y generar citas disponibles. Utiliza coordenadas geográficas reales
 *          de la ciudad de Quito, Ecuador.
 */
class SimpleGeolocationSystem {
private:
    vector<Branch> branches;              ///< Lista de sucursales bancarias disponibles
    const double EARTH_RADIUS_KM = 6371.0; ///< Radio de la Tierra en kilómetros para cálculos
    const int SERVICE_TIME_MINUTES = 40;   ///< Tiempo promedio de atención por cliente en minutos
    
    /**
     * @brief Obtiene coordenadas geográficas para un barrio específico de Quito
     * @param barrio Nombre del barrio para el cual se solicitan las coordenadas
     * @return Objeto Location con las coordenadas del barrio especificado
     * @details Método privado que mapea barrios específicos de Quito a sus
     *          coordenadas geográficas aproximadas
     */
    Location getLocationForBarrio(const string& barrio);
    
    /**
     * @brief Valida si un día es válido para operaciones bancarias
     * @param time_info Estructura tm con información de fecha y hora
     * @return true si es un día válido para operaciones bancarias, false en caso contrario
     * @details Método privado que verifica si el día de la semana permite
     *          realizar operaciones bancarias (lunes a sábado)
     */
    bool isValidBankingDay(const struct tm* time_info);
    
    /**
     * @brief Valida si una hora es válida para operaciones bancarias
     * @param time_info Estructura tm con información de fecha y hora
     * @return true si es una hora válida para operaciones bancarias, false en caso contrario
     * @details Método privado que verifica si la hora actual está dentro del
     *          horario de atención bancaria según el día de la semana
     */
    bool isValidBankingHour(const struct tm* time_info);
    
public:
    /**
     * @brief Constructor de la clase SimpleGeolocationSystem
     * @details Constructor que inicializa el sistema de geolocalización
     *          y carga automáticamente las sucursales bancarias disponibles
     */
    SimpleGeolocationSystem();
    
    /**
     * @brief Inicializa las sucursales bancarias en el sistema
     * @details Carga las sucursales bancarias predefinidas con sus ubicaciones
     *          geográficas reales en Quito, Ecuador, incluyendo coordenadas,
     *          direcciones y configuración inicial de colas
     */
    void initializeBranches();
    
    /**
     * @brief Permite seleccionar una ubicación a través de menús interactivos
     * @return Objeto Location con las coordenadas de la ubicación seleccionada
     * @details Guía al usuario a través de menús jerárquicos para seleccionar
     *          su ubicación en Quito (sector y barrio específico)
     */
    Location seleccionarUbicacionPorMenu();
    
    /**
     * @brief Calcula la distancia entre dos puntos geográficos
     * @param point1 Primera ubicación geográfica
     * @param point2 Segunda ubicación geográfica
     * @return Distancia en kilómetros entre los dos puntos
     * @details Utiliza la fórmula de Haversine para calcular la distancia
     *          más corta entre dos puntos en la superficie terrestre
     */
    double calculateDistance(const Location& point1, const Location& point2);
    
    /**
     * @brief Encuentra las sucursales más cercanas a una ubicación
     * @param user_location Ubicación del usuario
     * @return Vector de pares con las sucursales y sus distancias, ordenadas por proximidad
     * @details Calcula las distancias a todas las sucursales y retorna
     *          las 2 más cercanas al usuario
     */
    vector<pair<Branch, double>> findNearestBranches(const Location& user_location);
    
    /**
     * @brief Genera horarios disponibles basados en la posición en cola
     * @param queue_position Posición actual en la cola de espera
     * @return Vector de strings con los horarios disponibles
     * @details Función de compatibilidad que delega a generateValidBankingTimeSlots
     */
    vector<string> generateTimeSlots(int queue_position);
    
    /**
     * @brief Genera horarios válidos para operaciones bancarias
     * @param queue_position Posición actual en la cola de espera
     * @return Vector de strings con los horarios válidos disponibles
     * @details Calcula los próximos horarios disponibles considerando la cola
     *          y validando días y horas de atención bancaria
     */
    vector<string> generateValidBankingTimeSlots(int queue_position);
    
    /**
     * @brief Muestra información detallada de las sucursales
     * @param nearest_branches Vector de pares con las sucursales y sus distancias
     * @details Presenta al usuario información completa sobre las sucursales
     *          más cercanas, incluyendo ubicación, distancia, cola y horarios
     */
    void displayBranches(const vector<pair<Branch, double>>& nearest_branches);
    
    /**
     * @brief Obtiene la fecha actual en formato string
     * @return Fecha actual en formato DD/MM/YYYY
     * @details Función utilitaria que retorna la fecha del sistema
     *          en formato legible para el usuario
     */
    string getCurrentDate();
    
    /**
     * @brief Función principal que ejecuta el sistema de geolocalización
     * @return Par con la sucursal más cercana y su distancia
     * @details Integra todo el proceso: selección de ubicación, búsqueda
     *          de sucursales cercanas, visualización y retorno de resultados
     */
    pair<Branch, double> runGeolocationSystem();
    
    /**
     * @brief Actualiza la posición de la cola de una sucursal
     * @param branchId Identificador único de la sucursal
     * @param newQueuePosition Nueva posición de la cola
     * @details Permite actualizar dinámicamente el estado de la cola
     *          de una sucursal específica para simular cambios en tiempo real
     */
    void updateBranchQueue(int branchId, int newQueuePosition);
};

#endif
