/**
 * @class Clase para realizar las busquedas locales
 */

#ifndef BUSQUEDA_LOCAL_H
#define BUSQUEDA_LOCAL_H

#include "../../../tools/tools.h"
#include "../grasp.h"
#include <algorithm>

class BusquedaLocal {
  public:
    // Constructor
    BusquedaLocal() = default;
    BusquedaLocal(const vector<Recoleccion>& vehiculos) : vehiculos_(vehiculos) {}

    // Método para realizar la búsqueda local
    bool mejorarRutas();

    // Operadores para realizar la búsuqeda local
    bool intercambioZonas();
    bool insercionZonas() {}
    bool reubicacionSWTS() {}

    // Métodos auxiliares
    double calcularCostoRuta(const Recoleccion& vehiculo);
    double calcularCostoTotal(); // Calcular el coste total de un conjunto de rutas
    bool esFactible(const Recoleccion& vehiculo) { return true; } // Comprobar si la ruta del vehículo es factible

    // Setters
    void setVehiculos(const vector<Recoleccion>& vehiculos) { vehiculos_ = vehiculos; }

  private:
    vector<Recoleccion> vehiculos_; // Vector de vehículos
};

#endif