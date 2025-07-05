#ifndef UBICACION_H
#define UBICACION_H

#include <string>
#include <vector>

using namespace std;

// Clase para almacenar coordenadas de ubicación
class Location {
public:
    double latitude;
    double longitude;
    string city;
    string country;
    
    Location(double lat = 0.0, double lon = 0.0, const string& c = "", const string& co = "");
};

// Clase para almacenar información de sucursales
class Branch {
public:
    int id;
    string name;
    string address;
    Location location;
    int queue_position;
    vector<string> available_slots;
    
    Branch(int id, const string& name, const string& address, 
           double lat, double lon, int queue = 0);
};

// Sistema de geolocalización y gestión de sucursales
class SimpleGeolocationSystem {
private:
    vector<Branch> branches;
    const double EARTH_RADIUS_KM = 6371.0;
    const int SERVICE_TIME_MINUTES = 40;
    
    // Métodos para selección manual de ubicación
    void mostrarSectores();
    void mostrarBarriosPorSector(int sector);
    Location getLocationForBarrio(const string& barrio);
    
public:
    SimpleGeolocationSystem();
    
    // Inicialización
    void initializeBranches();
    
    // Selección de ubicación por menús
    Location seleccionarUbicacionPorMenu();
    
    // Cálculos de distancia y sucursales
    double calculateDistance(const Location& point1, const Location& point2);
    vector<pair<Branch, double>> findNearestBranches(const Location& user_location);
    
    // Generación de horarios y visualización
    vector<string> generateTimeSlots(int queue_position);
    void displayBranches(const vector<pair<Branch, double>>& nearest_branches);
    string getCurrentDate();
    
    // Función principal del sistema
    pair<Branch, double> runGeolocationSystem();
    
    // Gestión de colas
    void updateBranchQueue(int branchId, int newQueuePosition);
};

#endif
