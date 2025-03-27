#include "algoritmo.h"

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @param zonas Zonas del sistema
 * @param vehiculo Vehículo que se va a mover
 * @return Zona más cercana al vehículo con su distancia
 */
pair<Zona, double> Algoritmo::zonaMasCercana(const vector<Zona>& zonas, const Vehiculo& vehiculo) {
  double distancia = 0.0;
  double minDistancia = INFINITY;
  Zona zonaMasCercana = zonas[0];
  for (const Zona& zona : zonas) {
    if (find(vehiculo.getZonasVisitadas().begin(), vehiculo.getZonasVisitadas().end(), zona) == vehiculo.getZonasVisitadas().end()) {
      distancia = sqrt(pow(vehiculo.getPosicion().first - zona.getPosicion().first, 2) + pow(vehiculo.getPosicion().second - zona.getPosicion().second, 2));
      if (distancia < minDistancia) {
        cout << "Distancia" << " " << distancia << "\n" << endl;
        cout << "MinDistancia" << " " << minDistancia << endl;
        minDistancia = distancia;
        zonaMasCercana = zona;
      }
    }
  }
  return make_pair(zonaMasCercana, minDistancia);
}