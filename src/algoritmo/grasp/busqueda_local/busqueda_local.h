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

    // Método para realizar la búsqueda local
    void mejorarRutas();

    // Operadores para realizar la búsuqeda local
    bool swapInter();
    bool swapIntra();
    bool reinsertIntra();
    bool reinsertInter();

    // Métodos auxiliares
    double calcularCostoRuta(const Recoleccion& vehiculo);
    bool esFactible(const Recoleccion& vehiculo); // Comprobar si la ruta del vehículo es factible

    // Setters
    void setVehiculos(vector<Recoleccion>& vehiculos) { vehiculos_ = &vehiculos; }

  private:
    vector<Recoleccion>* vehiculos_; // Vector de vehículos
};

#endif