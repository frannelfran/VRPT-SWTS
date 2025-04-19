#include "busqueda_local.h"

/**
 * @brief Método para realizar la búsqueda local
 * @return true si se ha mejorado alguna ruta, false en caso contrario
 */
void BusquedaLocal::mejorarRutas() {
  bool mejorado = true;
  while (mejorado) {
    mejorado = false;
    mejorado |= swapInter();
    mejorado |= swapIntra();
    mejorado |= reinsertIntra();
    mejorado |= reinsertInter();
  }
}

/**
 * @brief Método para realizar la búsqueda local por intercambio de zonas
 * @return true si se ha mejorado alguna ruta, false en caso contrario
 */
bool BusquedaLocal::swapInter() {
  bool mejorado = false;
  // Recorremos los vehículos
  for (size_t i = 0; i < vehiculos_->size(); i++) {
    for (size_t j = i + 1; j < vehiculos_->size(); j++) {
      // Obtengo las zonas de los vehículos
      Recoleccion& ruta1 = (*vehiculos_)[i];
      Recoleccion& ruta2 = (*vehiculos_)[j];

      // Para cada zzona en la primera ruta
      for (size_t k = 1; k < ruta1.getZonasVisitadas().size() - 1; k++) {
        if (!ruta1.getZonasVisitadas()[k].esSWTS() && !ruta1.getZonasVisitadas()[k].esDeposito()) {
          // Para cada zona en la segunda ruta
          for (size_t l = 1; l < ruta2.getZonasVisitadas().size() - 1; l++) {
            if (!ruta2.getZonasVisitadas()[l].esSWTS() && !ruta2.getZonasVisitadas()[l].esDeposito()) {
              Recoleccion ruta1Copia = ruta1;
              Recoleccion ruta2Copia = ruta2;
              // Intercambio las zonas
              swap(ruta1Copia.getZonasVisitadas()[k], ruta2Copia.getZonasVisitadas()[l]);

              // Verifico que las rutas sean factibles
              if (esFactible(ruta1Copia) && esFactible(ruta2Copia)) {
                // Calculo el costo total
                double costoNuevo = calcularCostoRuta(ruta1Copia) + calcularCostoRuta(ruta2Copia);
                double costoAntiguo = calcularCostoRuta(ruta1) + calcularCostoRuta(ruta2);
                if (costoNuevo < costoAntiguo) {
                  // Si el costo nuevo es menor, actualizo las rutas
                  (*vehiculos_)[i] = ruta1Copia;
                  (*vehiculos_)[j] = ruta2Copia;
                  mejorado = true;
                }
              }
            }
          }
        }
      }
    }
  }
  return mejorado;
}

/**
 * @brief Implementación del método swapIntra
 * @return true si se ha mejorado alguna ruta, false en caso contrario
 */
bool BusquedaLocal::swapIntra() {
  bool mejorado = false;
  for (size_t i = 0; i < vehiculos_->size(); ++i) {
    Recoleccion& ruta = (*vehiculos_)[i];
    // Recorremos las zonas de la ruta
    vector<Zona>& zonas = ruta.getZonasVisitadas();
    for (size_t j = 1; j < zonas.size() - 1; ++j) {
      if (zonas[j].esSWTS() || zonas[j].esDeposito()) continue; // Ignoramos SWTS y depósitos
      for (size_t k = j + 1; k < zonas.size() - 1; ++k) {
        if (zonas[k].esSWTS() || zonas[k].esDeposito()) continue; // Ignoramos SWTS y depósitos
        // Intercambiamos las zonas
        Recoleccion copia = ruta;
        swap(copia.getZonasVisitadas()[j], copia.getZonasVisitadas()[k]);
        if (esFactible(copia) && calcularCostoRuta(copia) < calcularCostoRuta(ruta)) {
          // Si la ruta es factible y el costo es menor, actualizamos la ruta
          (*vehiculos_)[i] = copia;
          mejorado = true;
        }
      }
    }
  }
  return mejorado;
}

/**
 * @brief Implementación del método reinsertIntra
 * @return true si se ha mejorado alguna ruta, false en caso contrario
 */
bool BusquedaLocal::reinsertIntra() {
  bool mejorado = false;
  for (size_t i = 0; i < vehiculos_->size(); ++i) {
    Recoleccion& ruta = (*vehiculos_)[i];
    vector<Zona>& zonas = ruta.getZonasVisitadas();
    for (size_t j = 1; j < zonas.size() - 1; ++j) {
      if (zonas[j].esSWTS() || zonas[j].esDeposito()) continue; // Ignoramos SWTS y depósitos
      // Probar todas las opciones disponibles
      for (size_t k = 1; k < zonas.size() - 1; ++k) {
        if (j == k) continue; // No intercambiar la misma zona
        Recoleccion copia = ruta;
        Zona zona = copia.getZonasVisitadas()[j];
        // Mover la zona a la posición k
        copia.getZonasVisitadas().erase(copia.getZonasVisitadas().begin() + j);
        copia.getZonasVisitadas().insert(copia.getZonasVisitadas().begin() + k, zona);
        if (esFactible(copia) && calcularCostoRuta(copia) < calcularCostoRuta(ruta)) {
          // Si la ruta es factible y el costo es menor, actualizamos la ruta
          (*vehiculos_)[i] = copia;
          mejorado = true;
        }
      }
    }
  }
  return mejorado;
}

/**
 * @brief Método para verificar si la ruta del vehículo es factible
 * @param vehiculo Vehículo de recolección
 * @return true si la ruta es factible, false en caso contrario
 */
bool BusquedaLocal::esFactible(const Recoleccion& vehiculo) {
  // Verifico que el vehículo no exceda su capacidad
  double contenidoTotal = 0.0;
  const vector<Zona>& zonasVisitadas = vehiculo.getZonasVisitadas();
  for (const auto& zona : zonasVisitadas) {
    if (!zona.esSWTS() && !zona.esDeposito()) {
      contenidoTotal += zona.getContenido();
      if (contenidoTotal > vehiculo.getCapacidad()) {
        return false; // La ruta no es factible
      }
    }
    if (zona.esSWTS()) {
      contenidoTotal = 0.0; // Reseteamos el contenido
    }
  }
  // Verificamos la duración
  int tiempoTotal = 0;
  for (size_t i = 0; i < zonasVisitadas.size() - 1; i++) {
    tiempoTotal += vehiculo.calcularTiempo(zonasVisitadas[i].getDistancia(zonasVisitadas[i + 1]));
    tiempoTotal += zonasVisitadas[i].getTiempoDeProcesado();
    // Si se supera el tiempo, la ruta no es factible
    if (tiempoTotal > vehiculo.getDuracion()) {
      return false;
    }
  }
  return true; // La ruta es factible
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