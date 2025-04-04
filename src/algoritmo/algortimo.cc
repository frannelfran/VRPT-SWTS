#include "algoritmo.h"

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @param vehiculo Vehículo que se va a mover
 * @return Zona más cercana al vehículo con su distancia
 */
pair<Zona&, double> Algoritmo::zonaMasCercana(const Vehiculo& vehiculo) {
  vector<vector<double>> distancias = datos_.distancias;
  Zona zonaActual = vehiculo.getPosicion();
  Zona* zonaCercana = nullptr; // Inicializo la zona más cercana
  double minDistancia = INFINITY;
  auto it = find(datos_.zonas.begin(), datos_.zonas.end(), zonaActual); // Obtengo la posicion de la zona

  int index = distance(datos_.zonas.begin(), it);
  for (size_t i = 0; i < distancias[index].size(); i++) {
    if (distancias[index][i] < minDistancia &&
        find(vehiculo.getZonasVisitadas().begin(), vehiculo.getZonasVisitadas().end(), datos_.zonas[i]) == vehiculo.getZonasVisitadas().end() &&
        find(datos_.zonasRecoleccion.begin(), datos_.zonasRecoleccion.end(), datos_.zonas[i]) != datos_.zonasRecoleccion.end()) {
      minDistancia = distancias[index][i];
      zonaCercana = &datos_.zonas[i];
    }
  }
  return pair<Zona&, double>(*zonaCercana, minDistancia);
}

/**
 * @brief Método para obtener la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @return SWTS más cercana al vehículo con su distancia
 */
pair<Zona&, double> Algoritmo::swtsMasCercana(const Vehiculo& vehiculo) {
  vector<vector<double>> distancias = datos_.distancias;
  Zona zonaActual = vehiculo.getPosicion();
  Zona* swtsCercana = nullptr;
  double minDistancia = INFINITY;

  auto it = find(datos_.zonas.begin(), datos_.zonas.end(), zonaActual);

  int posicion = distance(datos_.zonas.begin(), it);

  for (size_t i = 0; i < distancias[posicion].size(); i++) {
    Zona& zona = datos_.zonas[i];
    if (!zona.esSWTS()) continue;
    else {
      if (distancias[posicion][i] < minDistancia) {
        minDistancia = distancias[posicion][i];
        swtsCercana = &datos_.zonas[i];
      }
    }
  }
  return pair<Zona&, double>(*swtsCercana, minDistancia);
}

/**
 * @brief Método para obtener el tiempo en que tarda el vehículo en volver al depósito pasando por las zonas y la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @return Tiempo que tarda en volver al depósito
 */
int Algoritmo::TiempoVolverDeposito(Vehiculo vehiculo) {
  int tiempo = 0;
  pair<Zona&, double> zonaTransferenciaCercana = swtsMasCercana(vehiculo);
  pair<Zona&, double> zonaCercana = zonaMasCercana(vehiculo);

  Vehiculo vehiculoAux = vehiculo;
  vehiculoAux.setPosicion(zonaTransferenciaCercana.first);
  // Tiempo que tarda en llegar a la zona más cercana
  tiempo += vehiculo.calcularTiempo(zonaCercana.second);
  // Tiempo que tarda en procesar la zona más cercana
  tiempo += zonaCercana.first.getTiempoDeProcesado();
  // Tiempo que tarda en llegar a la swts más cercana
  tiempo += vehiculo.calcularTiempo(zonaTransferenciaCercana.second);
  // Tiempo que tarda en volver al depósito desde la swts más cercana
  tiempo += vehiculoAux.calcularTiempo(datos_.zonas[0].getDistancia(zonaTransferenciaCercana.first));

  return tiempo;
}