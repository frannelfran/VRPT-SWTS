#include "algoritmo.h"

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @param vehiculo Vehículo que se va a mover
 * @return Zona más cercana al vehículo con su distancia
 */
pair<Zona, double> Algoritmo::zonaMasCercana(const Vehiculo& vehiculo) {
  vector<vector<double>> distancias = datos_.distancias;
  Zona zonaActual = vehiculo.getPosicion(), zonaCercana;
  double minDistancia = INFINITY;
  auto it = find(datos_.zonas.begin(), datos_.zonas.end(), zonaActual); // Obtengo la posicion de la zona

  int index = distance(datos_.zonas.begin(), it);
  for (size_t i = 0; i < distancias[index].size(); i++) {
    if (distancias[index][i] < minDistancia && find(vehiculo.getZonasVisitadas().begin(), vehiculo.getZonasVisitadas().end(), datos_.zonas[i]) == vehiculo.getZonasVisitadas().end()) {
      minDistancia = distancias[index][i];
      zonaCercana = datos_.zonas[i];
    }
  }
  return make_pair(zonaCercana, minDistancia);
}

/**
 * @brief Método para obtener la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @return SWTS más cercana al vehículo con su distancia
 */
pair<Zona, double> Algoritmo::swtsMasCercana(const Vehiculo& vehiculo) {
  static double distanciaTotal = 0.0; // Variable estática para acumular la distancia total
  vector<vector<double>> distancias = datos_.distancias;
  Zona zonaActual = vehiculo.getPosicion();
  Vehiculo vehiculoAux = vehiculo;
  pair<Zona, double> zonaCercana = zonaMasCercana(vehiculoAux); // Calculo la zona más cercana
  // Sumo las distancias
  distanciaTotal += zonaCercana.second;
  // Muevo el vehículo a la zona más cercana
  vehiculoAux.moverVehiculo(zonaCercana.first, zonaCercana.second);
  // Compruebo si ha llegado a una swts
  if (vehiculoAux.getPosicion().getId() == "IF" || vehiculoAux.getPosicion().getId() == "IF1") {
    cout << distanciaTotal << endl;
    double resultado = distanciaTotal; // Guardar el valor antes de reiniciar
    distanciaTotal = 0.0; // Reiniciar la distancia total para futuras llamadas
    return make_pair(zonaCercana.first, resultado);
  }
  else {
    return swtsMasCercana(vehiculoAux);
  }
}

/**
 * @brief Método para obtener el tiempo en que tarda el vehículo en volver al depósito pasando por las zonas y la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @return Tiempo que tarda en volver al depósito
 */
int Algoritmo::TiempoVolverDeposito(Vehiculo vehiculo) {
  int tiempo = 0;
  pair<Zona, double> zonaTransferenciaCercana = swtsMasCercana(vehiculo), zonaCercana = zonaMasCercana(vehiculo);
  Vehiculo vehiculoAux = vehiculo.setPosicion(zonaTransferenciaCercana.first);
  // Tiempo que tarda en llegar a la zona más cercana
  tiempo += vehiculo.calcularTiempo(zonaCercana.second);
  // Tiempo que tarda en llegar a la swts más cercana
  tiempo += vehiculo.calcularTiempo(zonaTransferenciaCercana.second);
  // Tiempo que tarda en volver al depósito desde la swts más cercana
  tiempo += vehiculoAux.calcularTiempo(datos_.zonas[0].getDistancia(zonaTransferenciaCercana.first));

  return tiempo;
}