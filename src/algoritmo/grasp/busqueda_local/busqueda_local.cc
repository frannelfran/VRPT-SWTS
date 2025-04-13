#include "busqueda_local.h"

/**
 * @brief Método para realizar la búsqueda local
 * @return true si se ha mejorado alguna ruta, false en caso contrario
 */
bool BusquedaLocal::mejorarRutas() {
  bool mejorado = false;
  // Recorremos los vehículos
  for (size_t i = 0; i < vehiculos_.size(); i++) {
    // Intentamos mejorar la ruta del vehículo
    if (intercambioZonas() || insercionZonas() || reubicacionSWTS()) {
      mejorado = true;
    }
  }
  return mejorado;
}

/**
 * @brief Método para realizar la búsqueda local por intercambio de zonas
 * @return true si se ha mejorado alguna ruta, false en caso contrario
 */
bool BusquedaLocal::intercambioZonas() {
  bool mejorado = false;
  double costoInicial = calcularCostoTotal();
  // Recorremos los vehículos
  for (size_t i = 0; i < vehiculos_.size(); i++) {
    for (size_t j = i + 1; j < vehiculos_.size(); j++) {
      // Obtengo las zonas de los vehículos
      Recoleccion& ruta1 = vehiculos_[i];
      Recoleccion& ruta2 = vehiculos_[j];

      // Para cada zzona en la primera ruta
      for (size_t k = 1; k < ruta1.getZonasVisitadas().size() - 1; k++) {
        if (!ruta1.getZonasVisitadas()[k].esSWTS() && !ruta1.getZonasVisitadas()[k].esDeposito()) {
          // Para cada zona en la segunda ruta
          for (size_t l = 1; l < ruta2.getZonasVisitadas().size() - 1; l++) {
            if (!ruta2.getZonasVisitadas()[l].esSWTS() && !ruta2.getZonasVisitadas()[l].esDeposito()) {
              Recoleccion ruta1Copia = ruta1;
              Recoleccion ruta2Copia = ruta2;
              // Intercambio las zonas
              auto temp = ruta1Copia.getZonasVisitadas()[k];
              
              
            }
          }
        }
      }
    }
    
  }
  return mejorado;
}

/**
 * @brief Método para calcular el costo total de un conjunto de rutas
 * @param vehiculos Conjunto de vehículos de recolección
 * @return Costo total de las rutas
 */
double BusquedaLocal::calcularCostoTotal() {
  double costoTotal = 0.0;
  for (const auto& vehiculo : vehiculos_) {
    costoTotal += calcularCostoRuta(vehiculo);
  }
  return costoTotal;
}

/**
 * @brief Método para calcular el costo de una ruta
 * @param vehiculo Vehiculo de recolección
 * @return Costo de la ruta
 */
double BusquedaLocal::calcularCostoRuta(const Recoleccion& vehiculo) {
  double costo = 0.0;
  // Calcular el costo de la ruta del vehículo
  for (size_t i = 0; i < vehiculo.getZonasVisitadas().size() - 1; i++) {
    costo += vehiculo.getZonasVisitadas()[i].getDistancia(vehiculo.getZonasVisitadas()[i + 1]);
  }
  return costo;
}