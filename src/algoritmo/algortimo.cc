#include "algoritmo.h"

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @param zonas Zonas del sistema
 * @param vehiculo Vehículo que se va a mover
 * @return Zona más cercana al vehículo con su distancia
 */
pair<Zona, double> Algoritmo::zonaMasCercana(const vector<Zona>& zonas, const Vehiculo& vehiculo) {
  double distancia = 0.0, minDistancia = INFINITY;
  Zona zonaMasCercana = zonas[0];
  for (const Zona& zona : zonas) {
    if (find(vehiculo.getZonasVisitadas().begin(), vehiculo.getZonasVisitadas().end(), zona) == vehiculo.getZonasVisitadas().end()) {
      distancia = sqrt(pow(vehiculo.getPosicion().first - zona.getPosicion().first, 2) + pow(vehiculo.getPosicion().second - zona.getPosicion().second, 2));
      if (distancia < minDistancia) {
        minDistancia = distancia;
        zonaMasCercana = zona;
      }
    }
  }
  return make_pair(zonaMasCercana, minDistancia);
}

/**
 * @brief Método para obtener la swts más cercana
 * @param zonas Zonas del sistema
 * @param vehiculo Vehículo que se va a mover
 * @return SWTS más cercana al vehículo con su distancia
 */
pair<Zona, double> Algoritmo::swtsMasCercana(const vector<Zona>& zonas, const Vehiculo& vehiculo) {
  double distancia = 0.0, total = 0.0, minDistancia = INFINITY;
  Zona zonaCercana = zonas[0];
  for (const Zona& zona : zonas) {
    if (find(vehiculo.getZonasVisitadas().begin(), vehiculo.getZonasVisitadas().end(), zona) == vehiculo.getZonasVisitadas().end()) {
      distancia = sqrt(pow(vehiculo.getPosicion().first - zona.getPosicion().first, 2) + pow(vehiculo.getPosicion().second - zona.getPosicion().second, 2));
      if (distancia < minDistancia) {
        minDistancia = distancia;
        // Voy sumando las distancias
        total += minDistancia;
        zonaCercana = zona;
        cout << "Zona: " << zona.getId() << " Distancia: " << minDistancia << endl;
        if (zona.getId() == "IF" || zona.getId() == "IF1") {
          break;
        }
      }
    }
  }
  return make_pair(zonaCercana, minDistancia);
}

/**
 * @brief Método para obtener el tiempo en que tarda el vehículo en volver al depósito pasando por las zonas y la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @return Tiempo que tarda en volver al depósito
 */

int Algoritmo::TiempoVolverDeposito(Vehiculo vehiculo, Zona zonaCercana) {
  int tiempo = 0;
  tiempo += vehiculo.calcularTiempo(zonaCercana);
  tiempo += vehiculo.calcularTiempo(swtsMasCercana(datos_.zonas, vehiculo).first);
  return tiempo;
}