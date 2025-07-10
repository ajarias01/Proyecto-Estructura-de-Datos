/**
 * @file Ubicacion.cpp
 * @brief Implementación del sistema de geolocalización y gestión de sucursales bancarias
 * @author Alan Arias, Sebastián Charij
 * @date 2025
 * @details Este archivo implementa las funcionalidades del sistema de geolocalización bancaria,
 *          incluyendo la gestión de ubicaciones, sucursales, cálculo de distancias, horarios
 *          bancarios y sistema de colas. Utiliza coordenadas geográficas para localizar
 *          sucursales cercanas al usuario en la ciudad de Quito, Ecuador.
 */

#include "Ubicacion.h"
#include <iostream>
#include <iomanip>
#include <chrono>   
#include <thread>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <ctime>
#include <cstring>
#include <fstream>

// Declaración forward de la función que necesitamos sin incluir todo Menus.h
extern int seleccionar_opcion(const char *titulo, const char *opciones[], int n, int fila_inicio);

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Constructor de la clase Location que inicializa las coordenadas geográficas
 * @param lat Latitud de la ubicación (coordenada en grados)
 * @param lon Longitud de la ubicación (coordenada en grados)
 * @param c Nombre de la ciudad
 * @param co Nombre del país
 * @details Constructor que permite crear una instancia de Location con coordenadas
 *          geográficas específicas y datos de localización. Utiliza lista de 
 *          inicialización para optimizar la construcción.
 */
Location::Location(double lat, double lon, const string& c, const string& co) 
    : latitude(lat), longitude(lon), city(c), country(co) {}

/**
 * @brief Constructor de la clase Branch que inicializa una sucursal bancaria
 * @param id Identificador único de la sucursal
 * @param name Nombre de la sucursal
 * @param address Dirección física de la sucursal
 * @param lat Latitud de la ubicación de la sucursal
 * @param lon Longitud de la ubicación de la sucursal
 * @param queue Posición inicial en la cola de espera
 * @details Constructor que crea una instancia de Branch con todos los datos
 *          necesarios para identificar y localizar una sucursal bancaria.
 *          Inicializa la ubicación geográfica y el estado de la cola.
 */
Branch::Branch(int id, const string& name, const string& address, 
               double lat, double lon, int queue)
    : id(id), name(name), address(address), location(lat, lon), queue_position(queue) {}

/**
 * @brief Constructor de la clase SimpleGeolocationSystem que inicializa el sistema
 * @details Constructor que crea una instancia del sistema de geolocalización
 *          e inicializa automáticamente las sucursales bancarias disponibles
 *          con sus respectivas ubicaciones y configuraciones iniciales.
 */
SimpleGeolocationSystem::SimpleGeolocationSystem() {
    initializeBranches();
}

/**
 * @brief Inicializa las sucursales bancarias con ubicaciones reales en Ecuador
 * @details Esta función carga las sucursales bancarias predefinidas en el sistema
 *          con sus coordenadas geográficas reales en la ciudad de Quito, Ecuador.
 *          Cada sucursal incluye ID, nombre, dirección, coordenadas y cola inicial.
 * @note Las coordenadas corresponden a ubicaciones reales en Quito y alrededores
 * @warning Las coordenadas deben estar en formato decimal (grados decimales)
 */
void SimpleGeolocationSystem::initializeBranches() {
    branches = {
        Branch(1, "Banco Central Norte", "Av. 10 de Agosto y Mariana de Jesús", -0.1650, -78.4850, 0),
        Branch(2, "Banco Centro Histórico", "Av. 12 de Octubre y Veintimilla", -0.2028, -78.4957, 0),
        Branch(3, "Banco Sur", "Av. Morán Valverde y Rumichaca", -0.2850, -78.5200, 0),
        Branch(4, "Banco Sangolquí", "Av. General Enríquez 681", -0.3342, -78.4486, 0),
        Branch(5, "Banco Valle", "Av. Ilaló y Conocoto", -0.3100, -78.4300, 0),
        Branch(6, "Banco Cumbayá", "Av. Francisco de Orellana", -0.2058, -78.4264, 0),
        Branch(7, "Banco Tumbaco", "Av. Interoceánica Km 12", -0.2089, -78.4003, 0)
    };
}

/**
 * @brief Obtiene las coordenadas geográficas precisas para un barrio específico de Quito
 * @param barrio Nombre del barrio para el cual se solicitan las coordenadas
 * @return Objeto Location con las coordenadas del barrio especificado
 * @details Esta función mapea barrios específicos de Quito a sus coordenadas
 *          geográficas aproximadas. Incluye barrios del norte, centro y sur de la ciudad.
 *          Si el barrio no está en la base de datos, retorna coordenadas del centro de Quito.
 * @note Las coordenadas están en formato decimal (grados decimales)
 * @warning Si el barrio no existe en la base de datos, se retorna una ubicación por defecto
 */
Location SimpleGeolocationSystem::getLocationForBarrio(const string& barrio) {
    // Norte
    if (barrio == "Carcelén") return Location(-0.04, -78.485, "Quito", "Ecuador");
    else if (barrio == "La Victoria") return Location(-0.06, -78.475, "Quito", "Ecuador");
    else if (barrio == "Cotocollao") return Location(-0.08, -78.485, "Quito", "Ecuador");
    else if (barrio == "Comité del Pueblo") return Location(-0.10, -78.475, "Quito", "Ecuador");
    else if (barrio == "El Rosario") return Location(-0.12, -78.465, "Quito", "Ecuador");
    else if (barrio == "San Carlos") return Location(-0.14, -78.475, "Quito", "Ecuador");
    else if (barrio == "Kennedy") return Location(-0.16, -78.465, "Quito", "Ecuador");
    else if (barrio == "El Bosque") return Location(-0.18, -78.455, "Quito", "Ecuador");
    else if (barrio == "Iñaquito") return Location(-0.20, -78.475, "Quito", "Ecuador");
    else if (barrio == "El Batán") return Location(-0.15, -78.485, "Quito", "Ecuador");
    else if (barrio == "Naciones Unidas") return Location(-0.17, -78.475, "Quito", "Ecuador");
    else if (barrio == "La Carolina") return Location(-0.17, -78.485, "Quito", "Ecuador");
    else if (barrio == "Rumipamba") return Location(-0.13, -78.475, "Quito", "Ecuador");
    else if (barrio == "Bellavista") return Location(-0.19, -78.495, "Quito", "Ecuador");
    else if (barrio == "Ponceano") return Location(-0.11, -78.465, "Quito", "Ecuador");
    
    // Centro
    else if (barrio == "Mariscal Sucre") return Location(-0.19, -78.495, "Quito", "Ecuador");
    else if (barrio == "La Floresta") return Location(-0.21, -78.495, "Quito", "Ecuador");
    else if (barrio == "La Vicentina") return Location(-0.23, -78.505, "Quito", "Ecuador");
    else if (barrio == "El Ejido") return Location(-0.22, -78.505, "Quito", "Ecuador");
    else if (barrio == "América") return Location(-0.23, -78.51, "Quito", "Ecuador");
    else if (barrio == "San Juan") return Location(-0.24, -78.52, "Quito", "Ecuador");
    else if (barrio == "La Colón") return Location(-0.21, -78.485, "Quito", "Ecuador");
    else if (barrio == "La Pradera") return Location(-0.23, -78.475, "Quito", "Ecuador");
    else if (barrio == "Guápulo") return Location(-0.21, -78.445, "Quito", "Ecuador");
    else if (barrio == "Miraflores") return Location(-0.23, -78.455, "Quito", "Ecuador");
    else if (barrio == "Ferroviaria") return Location(-0.22, -78.505, "Quito", "Ecuador");
    else if (barrio == "San Bartolo") return Location(-0.23, -78.505, "Quito", "Ecuador");
    
    // Sur - Zona Solanda y alrededores
    else if (barrio == "Solanda Centro") return Location(-0.28, -78.53, "Quito", "Ecuador");
    else if (barrio == "Turubamba Bajo") return Location(-0.305, -78.55, "Quito", "Ecuador");
    else if (barrio == "Turubamba Alto") return Location(-0.29, -78.54, "Quito", "Ecuador");
    else if (barrio == "Tachuco") return Location(-0.25, -78.525, "Quito", "Ecuador");
    else if (barrio == "La Loma") return Location(-0.26, -78.515, "Quito", "Ecuador");
    else if (barrio == "La Colmena") return Location(-0.27, -78.505, "Quito", "Ecuador");
    else if (barrio == "Santa Ana") return Location(-0.28, -78.515, "Quito", "Ecuador");
    else if (barrio == "Luluncoto") return Location(-0.29, -78.525, "Quito", "Ecuador");
    else if (barrio == "Chimbacalle") return Location(-0.25, -78.505, "Quito", "Ecuador");
    else if (barrio == "Alpahuasi") return Location(-0.30, -78.535, "Quito", "Ecuador");
    else if (barrio == "La Magdalena") return Location(-0.26, -78.515, "Quito", "Ecuador");
    else if (barrio == "Villa Flora") return Location(-0.24, -78.505, "Quito", "Ecuador");
    else if (barrio == "Marcopamba") return Location(-0.315, -78.56, "Quito", "Ecuador");
    else if (barrio == "Atahualpa") return Location(-0.325, -78.57, "Quito", "Ecuador");
    else if (barrio == "El Pintado") return Location(-0.335, -78.58, "Quito", "Ecuador");
    else if (barrio == "Chinyacu") return Location(-0.345, -78.59, "Quito", "Ecuador");
    else if (barrio == "Tarqui") return Location(-0.355, -78.60, "Quito", "Ecuador");
    else if (barrio == "Guajalo") return Location(-0.365, -78.61, "Quito", "Ecuador");
    else if (barrio == "Santa Rita") return Location(-0.375, -78.62, "Quito", "Ecuador");
    else if (barrio == "Chillogallo") return Location(-0.385, -78.63, "Quito", "Ecuador");
    else if (barrio == "Ecuatoriana") return Location(-0.395, -78.64, "Quito", "Ecuador");
    
    // Valles
    else if (barrio == "Cumbayá") return Location(-0.20, -78.425, "Quito", "Ecuador");
    else if (barrio == "Tumbaco") return Location(-0.21, -78.405, "Quito", "Ecuador");
    else if (barrio == "Conocoto") return Location(-0.31, -78.435, "Quito", "Ecuador");
    else if (barrio == "Sangolquí") return Location(-0.33, -78.445, "Quito", "Ecuador");
    else if (barrio == "San Rafael") return Location(-0.32, -78.455, "Quito", "Ecuador");
    
    // Por defecto, centro de Quito
    return Location(-0.2028, -78.4957, "Quito", "Ecuador");
}

/**
 * @brief Permite seleccionar una ubicación específica a través de menús interactivos
 * @return Objeto Location con las coordenadas de la ubicación seleccionada
 * @details Esta función guía al usuario a través de menús jerárquicos para seleccionar
 *          su ubicación en Quito. Primero se selecciona el sector (Norte, Centro, Sur, Valles)
 *          y luego el barrio específico dentro de ese sector.
 * @note Utiliza el sistema de menús con flechas para una mejor experiencia de usuario
 * @warning Requiere que la función seleccionar_opcion esté disponible externamente
 */
Location SimpleGeolocationSystem::seleccionarUbicacionPorMenu() {
    system("cls");
    cout << "\n=== SELECCIÓN DE UBICACIÓN ===" << endl;
    cout << "Para brindarle el mejor servicio, seleccione su ubicación:" << endl;
    
    // Paso 1: Seleccionar sector usando menú con flechas
    const char* sectores[] = {
        "Zona Norte",
        "Zona Centro", 
        "Zona Sur",
        "Valles (Cumbayá, Tumbaco, etc.)"
    };
    
    int sector = seleccionar_opcion("===== SELECCIONE SU SECTOR DE QUITO =====", sectores, 4, 4);
    
    // Paso 2: Seleccionar barrio usando menú con flechas
    int barrio_num;
    
    // Mapear selección a nombre de barrio
    string barrio_seleccionado;
    switch(sector) {
        case 0: { // Norte
            const char* barrios_norte[] = {
                "Carcelén", "La Victoria", "Cotocollao", "Comité del Pueblo", "El Rosario",
                "San Carlos", "Kennedy", "El Bosque", "Iñaquito", "El Batán",
                "Naciones Unidas", "La Carolina", "Rumipamba", "Bellavista", "Ponceano"
            };
            barrio_num = seleccionar_opcion("===== SELECCIONE SU BARRIO - ZONA NORTE =====", barrios_norte, 15, 4);
            vector<string> nombres_barrios_norte = {
                "Carcelén", "La Victoria", "Cotocollao", "Comité del Pueblo", "El Rosario",
                "San Carlos", "Kennedy", "El Bosque", "Iñaquito", "El Batán",
                "Naciones Unidas", "La Carolina", "Rumipamba", "Bellavista", "Ponceano"
            };
            barrio_seleccionado = nombres_barrios_norte[barrio_num];
            break;
        }
        case 1: { // Centro
            const char* barrios_centro[] = {
                "Mariscal Sucre", "La Floresta", "La Vicentina", "El Ejido", "América",
                "San Juan", "La Colón", "La Pradera", "Guápulo", "Miraflores",
                "Ferroviaria", "San Bartolo"
            };
            barrio_num = seleccionar_opcion("===== SELECCIONE SU BARRIO - ZONA CENTRO =====", barrios_centro, 12, 4);
            vector<string> nombres_barrios_centro = {
                "Mariscal Sucre", "La Floresta", "La Vicentina", "El Ejido", "América",
                "San Juan", "La Colón", "La Pradera", "Guápulo", "Miraflores",
                "Ferroviaria", "San Bartolo"
            };
            barrio_seleccionado = nombres_barrios_centro[barrio_num];
            break;
        }
        case 2: { // Sur
            const char* barrios_sur[] = {
                "Solanda Centro", "Turubamba Bajo", "Turubamba Alto", "Tachuco", "La Loma",
                "La Colmena", "Santa Ana", "Luluncoto", "Chimbacalle", "Alpahuasi",
                "La Magdalena", "Villa Flora", "Marcopamba", "Atahualpa", "El Pintado",
                "Chinyacu", "Tarqui", "Guajalo", "Santa Rita", "Chillogallo", "Ecuatoriana"
            };
            barrio_num = seleccionar_opcion("===== SELECCIONE SU BARRIO - ZONA SUR =====", barrios_sur, 21, 4);
            vector<string> nombres_barrios_sur = {
                "Solanda Centro", "Turubamba Bajo", "Turubamba Alto", "Tachuco", "La Loma",
                "La Colmena", "Santa Ana", "Luluncoto", "Chimbacalle", "Alpahuasi",
                "La Magdalena", "Villa Flora", "Marcopamba", "Atahualpa", "El Pintado",
                "Chinyacu", "Tarqui", "Guajalo", "Santa Rita", "Chillogallo", "Ecuatoriana"
            };
            barrio_seleccionado = nombres_barrios_sur[barrio_num];
            break;
        }
        case 3: { // Valles
            const char* barrios_valles[] = {
                "Cumbayá", "Tumbaco", "Conocoto", "Sangolquí", "San Rafael"
            };
            barrio_num = seleccionar_opcion("===== SELECCIONE SU BARRIO - VALLES =====", barrios_valles, 5, 4);
            vector<string> nombres_barrios_valles = {
                "Cumbayá", "Tumbaco", "Conocoto", "Sangolquí", "San Rafael"
            };
            barrio_seleccionado = nombres_barrios_valles[barrio_num];
            break;
        }
    }
    
    Location ubicacion_precisa = getLocationForBarrio(barrio_seleccionado);
    
    system("cls");
    cout << "\nUbicación seleccionada: " << barrio_seleccionado << ", " 
         << ubicacion_precisa.city << ", " << ubicacion_precisa.country << endl;
    
    return ubicacion_precisa;
}

/**
 * @brief Calcula la distancia real entre dos puntos geográficos usando la fórmula de Haversine
 * @param point1 Primera ubicación geográfica
 * @param point2 Segunda ubicación geográfica
 * @return Distancia en kilómetros entre los dos puntos
 * @details Esta función utiliza la fórmula de Haversine para calcular la distancia
 *          más corta entre dos puntos en la superficie de una esfera (la Tierra).
 *          Es más precisa que calcular distancias euclidianas para coordenadas geográficas.
 * @note La fórmula considera la curvatura de la Tierra para mayor precisión
 * @warning Las coordenadas deben estar en formato decimal (grados decimales)
 */
double SimpleGeolocationSystem::calculateDistance(const Location& point1, const Location& point2) {
    // Calcular cada componente de la fórmula de Haversine
    double lat1_rad = point1.latitude * M_PI / 180.0;
    double lat2_rad = point2.latitude * M_PI / 180.0;
    double delta_lat = (point2.latitude - point1.latitude) * M_PI / 180.0;
    double delta_lon = (point2.longitude - point1.longitude) * M_PI / 180.0;
    
    // Aplicar la fórmula de Haversine
    double a = sin(delta_lat/2) * sin(delta_lat/2) +
              cos(lat1_rad) * cos(lat2_rad) *
              sin(delta_lon/2) * sin(delta_lon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    // Retornar la distancia final multiplicada por el radio terrestre
    return EARTH_RADIUS_KM * c;
}

/**
 * @brief Encuentra las 2 sucursales bancarias más cercanas a una ubicación específica
 * @param user_location Ubicación del usuario para la cual se buscan sucursales cercanas
 * @return Vector de pares conteniendo las sucursales y sus distancias, ordenadas por proximidad
 * @details Esta función calcula la distancia de todas las sucursales disponibles
 *          respecto a la ubicación del usuario, las ordena por proximidad y retorna
 *          las 2 más cercanas para facilitar la selección del usuario.
 * @note Las distancias se calculan usando la fórmula de Haversine para mayor precisión
 * @warning Si hay menos de 2 sucursales registradas, retornará todas las disponibles
 */
vector<pair<Branch, double>> SimpleGeolocationSystem::findNearestBranches(const Location& user_location) {
    vector<pair<Branch, double>> branch_distances;
    
    // Calcular distancias desde la ubicación del usuario a todas las sucursales
    for (auto& branch : branches) {
        double distance = calculateDistance(user_location, branch.location);
        branch_distances.push_back({branch, distance});
    }
    
    // Ordenar sucursales por distancia (de menor a mayor)
    sort(branch_distances.begin(), branch_distances.end(),
         [](const pair<Branch, double>& a, const pair<Branch, double>& b) {
             return a.second < b.second;
         });
    
    // Retornar solo las 2 sucursales más cercanas
    return vector<pair<Branch, double>>(branch_distances.begin(), branch_distances.begin() + 2);
}

/**
 * @brief Verifica si el día actual es válido para operaciones bancarias
 * @param time_info Estructura tm con información de fecha y hora
 * @return true si es un día válido para operaciones bancarias, false en caso contrario
 * @details Esta función verifica si el día de la semana es apropiado para brindar
 *          servicios bancarios. Los domingos no se atiende al público.
 * @note Los días válidos son de lunes a sábado (1-6 en tm_wday)
 * @warning El domingo (0 en tm_wday) no es válido para operaciones bancarias
 */
bool SimpleGeolocationSystem::isValidBankingDay(const struct tm* time_info) {
    // Validar día de la semana (0=domingo, 1=lunes, ..., 6=sábado)
    int dia_semana = time_info->tm_wday;
    
    // Los domingos el banco está cerrado
    if (dia_semana == 0) {
        return false;
    }
    
    // Lunes a sábado son días válidos para operaciones bancarias
    return true;
}

/**
 * @brief Verifica si la hora actual es válida para operaciones bancarias
 * @param time_info Estructura tm con información de fecha y hora
 * @return true si es una hora válida para operaciones bancarias, false en caso contrario
 * @details Esta función verifica si la hora actual está dentro del horario de atención
 *          bancaria. Los horarios varían según el día de la semana:
 *          - Lunes a viernes: 9:00 a 17:00
 *          - Sábado: 9:00 a 12:00
 *          - Domingo: Cerrado
 * @note El horario se evalúa considerando tanto la hora como los minutos
 * @warning Los domingos no hay atención bancaria (retorna false)
 */
bool SimpleGeolocationSystem::isValidBankingHour(const struct tm* time_info) {
    // Extraer información temporal
    int dia_semana = time_info->tm_wday; // 0=domingo, 1=lunes, ..., 6=sábado
    int hora = time_info->tm_hour;
    int minutos = time_info->tm_min;
    
    // Los domingos no hay atención bancaria
    if (dia_semana == 0) {
        return false;
    }
    
    // Horario especial de sábado: 9:00 AM - 12:00 PM
    if (dia_semana == 6) {
        if (hora < 9 || (hora >= 12)) {
            return false;
        }
        if (hora == 12 && minutos > 0) {
            return false;
        }
    }
    
    // Horario de lunes a viernes: 9:00 AM - 3:30 PM
    if (dia_semana >= 1 && dia_semana <= 5) {
        if (hora < 9 || hora > 15) {
            return false;
        }
        if (hora == 15 && minutos > 30) {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Genera horarios válidos para operaciones bancarias basados en la posición en cola
 * @param queue_position Posición actual en la cola de espera
 * @return Vector de strings con los horarios disponibles para citas
 * @details Esta función calcula los próximos horarios disponibles para atención bancaria
 *          considerando la posición en cola y el tiempo de servicio. Busca hasta 5 horarios
 *          válidos dentro de las próximas 2 semanas, respetando los días y horas de atención.
 * @note Cada posición en cola equivale a SERVICE_TIME_MINUTES minutos de espera
 * @warning Los horarios generados respetan los días y horas de atención bancaria
 */
vector<string> SimpleGeolocationSystem::generateValidBankingTimeSlots(int queue_position) {
    vector<string> slots;
    auto now = chrono::system_clock::now();
    
    int wait_minutes = queue_position * SERVICE_TIME_MINUTES;
    auto current_time = now + chrono::minutes(wait_minutes);
    
    int slots_found = 0;
    int days_checked = 0;
    const int max_days_to_check = 14; // Buscar hasta 2 semanas adelante
    
    while (slots_found < 5 && days_checked < max_days_to_check) {
        auto slot_time_t = chrono::system_clock::to_time_t(current_time);
        struct tm* tm_info = localtime(&slot_time_t);
        
        if (isValidBankingDay(tm_info) && isValidBankingHour(tm_info)) {
            char buffer[20];
            char day_name[10];
            
            // Obtener nombre del día en español
            const char* dias[] = {"Dom", "Lun", "Mar", "Mié", "Jue", "Vie", "Sáb"};
            strcpy(day_name, dias[tm_info->tm_wday]);
            
            strftime(buffer, sizeof(buffer), "%d/%m %H:%M", tm_info);
            string slot_info = string(day_name) + " " + string(buffer);
            slots.push_back(slot_info);
            slots_found++;
        }
        
        // Avanzar al siguiente slot de tiempo
        if (tm_info->tm_wday == 6 && tm_info->tm_hour >= 11) {
            // Si es sábado y ya pasó las 11, saltar al lunes a las 9:00
            current_time += chrono::hours(24 * 2); // Saltar al lunes
            auto monday_time_t = chrono::system_clock::to_time_t(current_time);
            struct tm* monday_tm = localtime(&monday_time_t);
            monday_tm->tm_hour = 9;
            monday_tm->tm_min = 0;
            monday_tm->tm_sec = 0;
            current_time = chrono::system_clock::from_time_t(mktime(monday_tm));
            days_checked += 2;
        } else if (tm_info->tm_wday >= 1 && tm_info->tm_wday <= 5 && tm_info->tm_hour >= 15) {
            // Si es entre lunes y viernes y ya pasó las 15:00, saltar al siguiente día a las 9:00
            current_time += chrono::hours(24);
            auto next_day_time_t = chrono::system_clock::to_time_t(current_time);
            struct tm* next_day_tm = localtime(&next_day_time_t);
            next_day_tm->tm_hour = 9;
            next_day_tm->tm_min = 0;
            next_day_tm->tm_sec = 0;
            current_time = chrono::system_clock::from_time_t(mktime(next_day_tm));
            days_checked++;
        } else {
            // Avanzar 40 minutos (tiempo de servicio)
            current_time += chrono::minutes(SERVICE_TIME_MINUTES);
        }
    }
    
    // Si no encontramos suficientes slots, completar con mensaje informativo
    while (slots.size() < 5) {
        slots.push_back("No disponible");
    }
    
    return slots;
}

/**
 * @brief Genera horarios reales basados en la cola actual (función de compatibilidad)
 * @param queue_position Posición actual en la cola de espera
 * @return Vector de strings con los horarios disponibles para citas
 * @details Esta función mantiene compatibilidad con versiones anteriores del sistema
 *          y delega la generación de horarios a la función con validaciones bancarias.
 * @note Función mantenida para compatibilidad con código existente
 * @see generateValidBankingTimeSlots() para la implementación completa
 */
vector<string> SimpleGeolocationSystem::generateTimeSlots(int queue_position) {
    // Usar la nueva función con validaciones bancarias
    return generateValidBankingTimeSlots(queue_position);
}

/**
 * @brief Muestra información detallada de las sucursales más cercanas
 * @param nearest_branches Vector de pares con las sucursales y sus distancias
 * @details Esta función presenta al usuario información completa sobre las sucursales
 *          más cercanas, incluyendo nombre, dirección, distancia, estado de la cola,
 *          tiempo de espera y horarios disponibles. También muestra los horarios
 *          generales de atención bancaria.
 * @note Incluye validación de horarios bancarios y estados de cola en tiempo real
 * @warning Muestra advertencias si se está fuera del horario de atención
 */
void SimpleGeolocationSystem::displayBranches(const vector<pair<Branch, double>>& nearest_branches) {
    system("cls");
    cout << "\n=== SUCURSALES MÁS CERCANAS ===" << endl;
    cout << "================================" << endl;
    cout << "HORARIOS DE ATENCIÓN:" << endl;
    cout << "• Lunes a Viernes: 9:00 AM - 3:30 PM" << endl;
    cout << "• Sábados: 9:00 AM - 12:00 PM" << endl;
    cout << "• Domingos: CERRADO" << endl;
    cout << "================================" << endl;
    
    for (size_t i = 0; i < nearest_branches.size(); i++) {
        const auto& branch = nearest_branches[i].first;
        double distance = nearest_branches[i].second;
        
        cout << "\n" << (i + 1) << ". " << branch.name << endl;
        cout << "   Dirección: " << branch.address << endl;
        cout << "   Distancia: " << fixed << setprecision(2) << distance << " km" << endl;
        
        if (branch.queue_position == 0) {
            cout << "   Estado: ¡Sin cola! - Atención inmediata disponible" << endl;
            cout << "   Tiempo de espera: 0 minutos" << endl;
        } else {
            cout << "   Personas en cola: " << branch.queue_position << endl;
            int wait_time = branch.queue_position * SERVICE_TIME_MINUTES;
            cout << "   Tiempo de espera: " << wait_time << " minutos" << endl;
        }
        
        auto slots = generateValidBankingTimeSlots(branch.queue_position);
        cout << "   Horarios disponibles: ";
        for (size_t j = 0; j < slots.size(); j++) {
            if (slots[j] != "No disponible") {
                cout << slots[j];
                if (j < slots.size() - 1 && slots[j+1] != "No disponible") cout << ", ";
            }
        }
        cout << endl;
        
        // Verificar si estamos fuera del horario bancario
        auto now = chrono::system_clock::now();
        auto now_time_t = chrono::system_clock::to_time_t(now);
        struct tm* current_tm = localtime(&now_time_t);
        
        if (!isValidBankingDay(current_tm)) {
            cout << "Hoy no hay atención bancaria (Domingo)" << endl;
        } else if (!isValidBankingHour(current_tm)) {
            if (current_tm->tm_wday == 6) {
                cout << "Fuera del horario de sábado (9:00 AM - 12:00 PM)" << endl;
            } else {
                cout << "Fuera del horario de atención (9:00 AM - 3:30 PM)" << endl;
            }
        }
    }
}

/**
 * @brief Obtiene la fecha actual formateada en formato dd/mm/yyyy
 * @return String con la fecha actual en formato dd/mm/yyyy
 * @details Esta función utiliza el sistema de tiempo del sistema para obtener
 *          la fecha actual y la formatea en un formato legible para el usuario.
 *          Útil para mostrar información temporal en reportes y citas.
 * @note La fecha se formatea usando strftime con formato dd/mm/yyyy
 * @warning Depende del reloj del sistema para obtener la fecha correcta
 */
string SimpleGeolocationSystem::getCurrentDate() {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    struct tm* tm_info = localtime(&time_t);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info);
    return string(buffer);
}

/**
 * @brief Función principal que ejecuta el sistema completo de geolocalización
 * @return Par con la sucursal más cercana y su distancia
 * @details Esta función integra todo el proceso de geolocalización:
 *          1. Permite al usuario seleccionar su ubicación
 *          2. Busca las sucursales más cercanas
 *          3. Muestra la información detallada de las sucursales
 *          4. Retorna la sucursal más cercana encontrada
 * @note Incluye una pausa para simular el proceso de búsqueda
 * @warning Requiere interacción del usuario para seleccionar la ubicación
 */
pair<Branch, double> SimpleGeolocationSystem::runGeolocationSystem() {
    Location user_location = seleccionarUbicacionPorMenu();
    
    cout << "\nBuscando sucursales cercanas..." << endl;
    this_thread::sleep_for(chrono::milliseconds(1500));
    
    auto nearest_branches = findNearestBranches(user_location);
    displayBranches(nearest_branches);
    
    return nearest_branches[0]; // Retornar la más cercana por defecto
}

/**
 * @brief Actualiza la posición de la cola de una sucursal específica
 * @param branchId Identificador único de la sucursal a actualizar
 * @param newQueuePosition Nueva posición de la cola para la sucursal
 * @details Esta función permite actualizar dinámicamente el estado de la cola
 *          de una sucursal específica, simulando la llegada o salida de clientes.
 *          Busca la sucursal por su ID y actualiza su posición de cola.
 * @note Útil para simular cambios en tiempo real en el sistema de colas
 * @warning Si el branchId no existe, no se realizará ningún cambio
 */
void SimpleGeolocationSystem::updateBranchQueue(int branchId, int newQueuePosition) {
    // Buscar la sucursal por ID y actualizar su cola
    for (auto& branch : branches) {
        if (branch.id == branchId) {
            branch.queue_position = newQueuePosition;
            break;
        }
    }
}

/**
 * @brief Constructor de la estructura CitaAgendada
 * @param dni DNI del cliente
 * @param nombre Nombre completo del cliente
 * @param tel Teléfono del cliente
 * @param suc_id ID de la sucursal
 * @param suc_nombre Nombre de la sucursal
 * @param sec Sector de la sucursal
 * @param fecha_h Fecha y hora de la cita
 * @param num_conf Número de confirmación
 * @param fecha_ag Fecha de agendamiento
 * @param act Estado activo de la cita
 */
CitaAgendada::CitaAgendada(const string& dni, const string& nombre, const string& tel,
                           int suc_id, const string& suc_nombre, const string& sec,
                           const string& fecha_h, const string& num_conf,
                           const string& fecha_ag, bool act)
    : dni(dni), nombre_completo(nombre), telefono(tel), sucursal_id(suc_id),
      sucursal_nombre(suc_nombre), sector(sec), fecha_hora(fecha_h),
      numero_confirmacion(num_conf), fecha_agendamiento(fecha_ag), activa(act) {}

/**
 * @brief Verifica si un cliente ya tiene una cita agendada activa
 * @param dni DNI del cliente a verificar
 * @return true si ya tiene una cita activa, false en caso contrario
 */
bool SimpleGeolocationSystem::clienteTieneCitaActiva(const string& dni) {
    for (const auto& cita : citas_agendadas) {
        if (cita.dni == dni && cita.activa) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Agrega una nueva cita al sistema
 * @param cita Objeto CitaAgendada con la información de la cita
 * @return true si se agregó exitosamente, false en caso contrario
 */
bool SimpleGeolocationSystem::agregarCita(const CitaAgendada& cita) {
    try {
        citas_agendadas.push_back(cita);
        guardarCitas(); // Guardar automáticamente
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

/**
 * @brief Cancela una cita existente
 * @param dni DNI del cliente
 * @param numero_confirmacion Número de confirmación de la cita
 * @return true si se canceló exitosamente, false si no se encontró
 */
bool SimpleGeolocationSystem::cancelarCita(const string& dni, const string& numero_confirmacion) {
    for (auto& cita : citas_agendadas) {
        string numero_cita_upper = cita.numero_confirmacion;
        transform(numero_cita_upper.begin(), numero_cita_upper.end(), numero_cita_upper.begin(), ::toupper);
        string numero_input_upper = numero_confirmacion;
        transform(numero_input_upper.begin(), numero_input_upper.end(), numero_input_upper.begin(), ::toupper);
        
        if (cita.dni == dni && numero_cita_upper == numero_input_upper && cita.activa) {
            cita.activa = false;
            guardarCitas(); // Guardar cambios
            return true;
        }
    }
    return false;
}

/**
 * @brief Obtiene todas las citas agendadas por sector
 * @param sector Sector específico a consultar (vacío para ver todos)
 * @return Vector con las citas del sector especificado
 */
vector<CitaAgendada> SimpleGeolocationSystem::obtenerCitasPorSector(const string& sector) {
    vector<CitaAgendada> citas_filtradas;
    
    for (const auto& cita : citas_agendadas) {
        if (cita.activa && (sector.empty() || cita.sector == sector)) {
            citas_filtradas.push_back(cita);
        }
    }
    
    return citas_filtradas;
}

/**
 * @brief Guarda las citas en un archivo para persistencia
 * @param archivo Nombre del archivo donde guardar
 * @return true si se guardó exitosamente, false en caso contrario
 */
bool SimpleGeolocationSystem::guardarCitas(const string& archivo) {
    try {
        ofstream file(archivo);
        if (!file.is_open()) {
            return false;
        }
        
        for (const auto& cita : citas_agendadas) {
            file << cita.dni << "|"
                 << cita.nombre_completo << "|"
                 << cita.telefono << "|"
                 << cita.sucursal_id << "|"
                 << cita.sucursal_nombre << "|"
                 << cita.sector << "|"
                 << cita.fecha_hora << "|"
                 << cita.numero_confirmacion << "|"
                 << cita.fecha_agendamiento << "|"
                 << (cita.activa ? "1" : "0") << endl;
        }
        
        file.close();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

/**
 * @brief Carga las citas desde un archivo
 * @param archivo Nombre del archivo desde donde cargar
 * @return true si se cargó exitosamente, false en caso contrario
 */
bool SimpleGeolocationSystem::cargarCitas(const string& archivo) {
    try {
        ifstream file(archivo);
        if (!file.is_open()) {
            return false; // Archivo no existe, es normal al inicio
        }
        
        citas_agendadas.clear();
        string line;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            vector<string> campos;
            string campo = "";
            
            for (char c : line) {
                if (c == '|') {
                    campos.push_back(campo);
                    campo = "";
                } else {
                    campo += c;
                }
            }
            campos.push_back(campo); // Último campo
            
            if (campos.size() == 10) {
                CitaAgendada cita(
                    campos[0], campos[1], campos[2],
                    stoi(campos[3]), campos[4], campos[5],
                    campos[6], campos[7], campos[8],
                    campos[9] == "1"
                );
                citas_agendadas.push_back(cita);
            }
        }
        
        file.close();
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

/**
 * @brief Obtiene el sector de una sucursal por su ID
 * @param sucursal_id ID de la sucursal
 * @return String con el nombre del sector
 */
string SimpleGeolocationSystem::obtenerSectorPorSucursal(int sucursal_id) {
    for (const auto& branch : branches) {
        if (branch.id == sucursal_id) {
            // Determinar sector basado en el nombre de la sucursal
            string nombre = branch.name;
            if (nombre.find("Centro") != string::npos) {
                return "Centro Histórico";
            } else if (nombre.find("Norte") != string::npos) {
                return "Norte de Quito";
            } else if (nombre.find("Sur") != string::npos) {
                return "Sur de Quito";
            } else if (nombre.find("Valle") != string::npos || nombre.find("Cumbayá") != string::npos || 
                       nombre.find("Tumbaco") != string::npos || nombre.find("Sangolquí") != string::npos) {
                return "Valles de Quito";
            }
        }
    }
    return "Sector Desconocido";
}
