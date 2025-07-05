#include "Ubicacion.h"
#include <iostream>
#include <iomanip>
#include <chrono>   
#include <thread>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// ==================== IMPLEMENTACIÓN DE LA CLASE Location ====================
Location::Location(double lat, double lon, const string& c, const string& co) 
    : latitude(lat), longitude(lon), city(c), country(co) {}

// ==================== IMPLEMENTACIÓN DE LA CLASE Branch ====================
Branch::Branch(int id, const string& name, const string& address, 
               double lat, double lon, int queue)
    : id(id), name(name), address(address), location(lat, lon), queue_position(queue) {}

// ==================== IMPLEMENTACIÓN DE LA CLASE SimpleGeolocationSystem ====================
SimpleGeolocationSystem::SimpleGeolocationSystem() {
    initializeBranches();
}

// Inicializar sucursales en Ecuador con colas vacías
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

// Mostrar menú de sectores de Quito
void SimpleGeolocationSystem::mostrarSectores() {
    cout << "\n=== SELECCIONE SU SECTOR DE QUITO ===" << endl;
    cout << "1. Zona Norte" << endl;
    cout << "2. Zona Centro" << endl;
    cout << "3. Zona Sur" << endl;
    cout << "4. Valles (Cumbayá, Tumbaco, etc.)" << endl;
}

// Mostrar barrios por sector seleccionado
void SimpleGeolocationSystem::mostrarBarriosPorSector(int sector) {
    cout << "\n=== SELECCIONE SU BARRIO ===" << endl;
    
    switch(sector) {
        case 1: // Norte
            cout << "1. Carcelén" << endl;
            cout << "2. La Victoria" << endl;
            cout << "3. Cotocollao" << endl;
            cout << "4. Comité del Pueblo" << endl;
            cout << "5. El Rosario" << endl;
            cout << "6. San Carlos" << endl;
            cout << "7. Kennedy" << endl;
            cout << "8. El Bosque" << endl;
            cout << "9. Iñaquito" << endl;
            cout << "10. El Batán" << endl;
            cout << "11. Naciones Unidas" << endl;
            cout << "12. La Carolina" << endl;
            cout << "13. Rumipamba" << endl;
            cout << "14. Bellavista" << endl;
            cout << "15. Ponceano" << endl;
            break;
            
        case 2: // Centro
            cout << "1. Mariscal Sucre" << endl;
            cout << "2. La Floresta" << endl;
            cout << "3. La Vicentina" << endl;
            cout << "4. El Ejido" << endl;
            cout << "5. América" << endl;
            cout << "6. San Juan" << endl;
            cout << "7. La Colón" << endl;
            cout << "8. La Pradera" << endl;
            cout << "9. Guápulo" << endl;
            cout << "10. Miraflores" << endl;
            cout << "11. Ferroviaria" << endl;
            cout << "12. San Bartolo" << endl;
            break;
            
        case 3: // Sur
            cout << "1. Solanda Centro" << endl;
            cout << "2. Turubamba Bajo" << endl;
            cout << "3. Turubamba Alto" << endl;
            cout << "4. Tachuco" << endl;
            cout << "5. La Loma" << endl;
            cout << "6. La Colmena" << endl;
            cout << "7. Santa Ana" << endl;
            cout << "8. Luluncoto" << endl;
            cout << "9. Chimbacalle" << endl;
            cout << "10. Alpahuasi" << endl;
            cout << "11. La Magdalena" << endl;
            cout << "12. Villa Flora" << endl;
            cout << "13. Marcopamba" << endl;
            cout << "14. Atahualpa" << endl;
            cout << "15. El Pintado" << endl;
            cout << "16. Chinyacu" << endl;
            cout << "17. Tarqui" << endl;
            cout << "18. Guajalo" << endl;
            cout << "19. Santa Rita" << endl;
            cout << "20. Chillogallo" << endl;
            cout << "21. Ecuatoriana" << endl;
            break;
            
        case 4: // Valles
            cout << "1. Cumbayá" << endl;
            cout << "2. Tumbaco" << endl;
            cout << "3. Conocoto" << endl;
            cout << "4. Sangolquí" << endl;
            cout << "5. San Rafael" << endl;
            break;
    }
}

// Obtener coordenadas precisas para un barrio específico
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

// Seleccionar ubicación a través de menús
Location SimpleGeolocationSystem::seleccionarUbicacionPorMenu() {
    cout << "\n=== SELECCIÓN DE UBICACIÓN ===" << endl;
    cout << "Para brindarle el mejor servicio, seleccione su ubicación:" << endl;
    
    // Paso 1: Seleccionar sector
    mostrarSectores();
    int sector;
    do {
        cout << "\nSeleccione su sector (1-4): ";
        cin >> sector;
        cin.ignore();
    } while (sector < 1 || sector > 4);
    
    // Paso 2: Mostrar barrios del sector y seleccionar
    mostrarBarriosPorSector(sector);
    
    int maxBarrios;
    switch(sector) {
        case 1: maxBarrios = 15; break; // Norte
        case 2: maxBarrios = 12; break; // Centro
        case 3: maxBarrios = 21; break; // Sur
        case 4: maxBarrios = 5; break;  // Valles
        default: maxBarrios = 1; break;
    }
    
    int barrio_num;
    do {
        cout << "\nSeleccione su barrio (1-" << maxBarrios << "): ";
        cin >> barrio_num;
        cin.ignore();
    } while (barrio_num < 1 || barrio_num > maxBarrios);
    
    // Mapear selección a nombre de barrio
    string barrio_seleccionado;
    switch(sector) {
        case 1: { // Norte
            vector<string> barrios_norte = {
                "Carcelén", "La Victoria", "Cotocollao", "Comité del Pueblo", "El Rosario",
                "San Carlos", "Kennedy", "El Bosque", "Iñaquito", "El Batán",
                "Naciones Unidas", "La Carolina", "Rumipamba", "Bellavista", "Ponceano"
            };
            barrio_seleccionado = barrios_norte[barrio_num - 1];
            break;
        }
        case 2: { // Centro
            vector<string> barrios_centro = {
                "Mariscal Sucre", "La Floresta", "La Vicentina", "El Ejido", "América",
                "San Juan", "La Colón", "La Pradera", "Guápulo", "Miraflores",
                "Ferroviaria", "San Bartolo"
            };
            barrio_seleccionado = barrios_centro[barrio_num - 1];
            break;
        }
        case 3: { // Sur
            vector<string> barrios_sur = {
                "Solanda Centro", "Turubamba Bajo", "Turubamba Alto", "Tachuco", "La Loma",
                "La Colmena", "Santa Ana", "Luluncoto", "Chimbacalle", "Alpahuasi",
                "La Magdalena", "Villa Flora", "Marcopamba", "Atahualpa", "El Pintado",
                "Chinyacu", "Tarqui", "Guajalo", "Santa Rita", "Chillogallo", "Ecuatoriana"
            };
            barrio_seleccionado = barrios_sur[barrio_num - 1];
            break;
        }
        case 4: { // Valles
            vector<string> barrios_valles = {
                "Cumbayá", "Tumbaco", "Conocoto", "Sangolquí", "San Rafael"
            };
            barrio_seleccionado = barrios_valles[barrio_num - 1];
            break;
        }
    }
    
    Location ubicacion_precisa = getLocationForBarrio(barrio_seleccionado);
    
    cout << "\nUbicación seleccionada: " << barrio_seleccionado << ", " 
         << ubicacion_precisa.city << ", " << ubicacion_precisa.country << endl;
    
    return ubicacion_precisa;
}

// Calcular distancia real usando fórmula de Haversine
double SimpleGeolocationSystem::calculateDistance(const Location& point1, const Location& point2) {
    double lat1_rad = point1.latitude * M_PI / 180.0;
    double lat2_rad = point2.latitude * M_PI / 180.0;
    double delta_lat = (point2.latitude - point1.latitude) * M_PI / 180.0;
    double delta_lon = (point2.longitude - point1.longitude) * M_PI / 180.0;
    
    double a = sin(delta_lat/2) * sin(delta_lat/2) +
              cos(lat1_rad) * cos(lat2_rad) *
              sin(delta_lon/2) * sin(delta_lon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return EARTH_RADIUS_KM * c;
}

// Encontrar las 2 sucursales más cercanas
vector<pair<Branch, double>> SimpleGeolocationSystem::findNearestBranches(const Location& user_location) {
    vector<pair<Branch, double>> branch_distances;
    
    for (auto& branch : branches) {
        double distance = calculateDistance(user_location, branch.location);
        branch_distances.push_back({branch, distance});
    }
    
    sort(branch_distances.begin(), branch_distances.end(),
         [](const pair<Branch, double>& a, const pair<Branch, double>& b) {
             return a.second < b.second;
         });
    
    return vector<pair<Branch, double>>(branch_distances.begin(), branch_distances.begin() + 2);
}

// Generar horarios reales basados en la cola actual
vector<string> SimpleGeolocationSystem::generateTimeSlots(int queue_position) {
    vector<string> slots;
    auto now = chrono::system_clock::now();
    
    int wait_minutes = queue_position * SERVICE_TIME_MINUTES;
    
    for (int i = 0; i < 5; i++) {
        auto slot_time = now + chrono::minutes(wait_minutes + (i * SERVICE_TIME_MINUTES));
        auto slot_time_t = chrono::system_clock::to_time_t(slot_time);
        
        struct tm* tm_info = localtime(&slot_time_t);
        char buffer[6];
        strftime(buffer, sizeof(buffer), "%H:%M", tm_info);
        slots.push_back(string(buffer));
    }
    
    return slots;
}

// Mostrar información detallada de sucursales
void SimpleGeolocationSystem::displayBranches(const vector<pair<Branch, double>>& nearest_branches) {
    cout << "\n=== SUCURSALES MÁS CERCANAS ===" << endl;
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
        
        auto slots = generateTimeSlots(branch.queue_position);
        cout << "   Horarios disponibles: ";
        for (size_t j = 0; j < slots.size(); j++) {
            cout << slots[j];
            if (j < slots.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// Obtener fecha actual formateada
string SimpleGeolocationSystem::getCurrentDate() {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    struct tm* tm_info = localtime(&time_t);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm_info);
    return string(buffer);
}

// Función principal del sistema
pair<Branch, double> SimpleGeolocationSystem::runGeolocationSystem() {
    Location user_location = seleccionarUbicacionPorMenu();
    
    cout << "\nBuscando sucursales cercanas..." << endl;
    this_thread::sleep_for(chrono::milliseconds(1500));
    
    auto nearest_branches = findNearestBranches(user_location);
    displayBranches(nearest_branches);
    
    return nearest_branches[0]; // Retornar la más cercana por defecto
}

// Actualizar cola de una sucursal (simular llegada de cliente)
void SimpleGeolocationSystem::updateBranchQueue(int branchId, int newQueuePosition) {
    for (auto& branch : branches) {
        if (branch.id == branchId) {
            branch.queue_position = newQueuePosition;
            break;
        }
    }
}
