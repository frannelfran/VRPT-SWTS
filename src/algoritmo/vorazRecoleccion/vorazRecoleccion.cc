#include "vorazRecoleccion.h"

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @param vehiculo Vehículo que se va a mover
 * @return Zona más cercana al vehículo con su distancia
 */
pair<Zona&, double> VorazRecoleccion::zonaMasCercana(const Vehiculo& vehiculo) {
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
pair<Zona&, double> VorazRecoleccion::swtsMasCercana(const Vehiculo& vehiculo) {
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
int VorazRecoleccion::TiempoVolverDeposito(Vehiculo vehiculo) {
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

/**
 * @brief Método para construir las rutas de los vehículos de recolección
 * @return vector<Vehiculo> Rutas de los vehículos de recolección
 */
vector<Vehiculo> VorazRecoleccion::ejecutar() {
  vector<Vehiculo> rutasDeVehiculos;
  vector<Zona>& zonasPendientes = datos_.zonasRecoleccion;

  while (!zonasPendientes.empty()) {
    // Creamos el vehículo
    Recoleccion vehiculo(datos_.capacidadRecoleccion, datos_.velocidad, datos_.zonas[0], datos_.duracionRecoleccion);
    do {
      if (zonasPendientes.empty()) break;
      pair<Zona&, double> zonaCercana = zonaMasCercana(vehiculo);
      int tiempoEnVolverAlDeposito = TiempoVolverDeposito(vehiculo);
      // Si el contendio de la zona es menor a la capacidad del vehículo y le da tiempo a volver al deposito
      if (vehiculo.llenarVehiculo(zonaCercana.first.getContenido()) && tiempoEnVolverAlDeposito <= vehiculo.getDuracion()) {
        vehiculo.moverVehiculo(zonaCercana.first, zonaCercana.second);
        vehiculo.vaciarZona(zonaCercana.first);
        zonasPendientes.erase(remove(zonasPendientes.begin(), zonasPendientes.end(), zonaCercana.first), zonasPendientes.end());
      } else if (tiempoEnVolverAlDeposito <= vehiculo.getDuracion()) {
        // Si no puede recoger la zona, buscamos la swts más cercana
        pair<Zona&, double> swtsCercana = swtsMasCercana(vehiculo);
        vehiculo.moverVehiculo(swtsCercana.first, swtsCercana.second);
        vehiculo.vaciarVehiculo(swtsCercana.first);
      } else {
        break; // No se puede añadir más zonas
      }
    } while (true);
    // Si la última zona visitada no es una swts, buscamos la más cercana
    if (!vehiculo.getPosicion().esSWTS()) {
      pair<Zona&, double> swtsCercana = swtsMasCercana(vehiculo);
      vehiculo.moverVehiculo(swtsCercana.first, swtsCercana.second);
      vehiculo.vaciarVehiculo(swtsCercana.first);
      vehiculo.volverAlInicio();
    }
    else {
      vehiculo.volverAlInicio();
    }
    rutasDeVehiculos.push_back(vehiculo);
  }
  return rutasDeVehiculos;
}

//1: K ← ∅
//2: k ← 1
//3: Mientras C ≠ ∅ hacer
//4:    R_k ← {depósito}
//5:    q_k ← Q                  // Capacidad residual del vehículo
//6:    T_k ← L                  // Tiempo residual de la ruta
//7:    Repetir
//8:        c' ← Zona más cercana a la última posición en R_k (dentro de C)
//9:        t' ← Tiempo para visitar c', una SWTS y volver al depósito
//10:       Si d_c' ≤ q_k y t' ≤ T_k entonces
//11:           R_k ← R_k ∪ {c'}
//12:           q_k ← q_k - d_c'
//13:           Actualizar T_k
//14:           C ← C \ {c'}
//15:       Sino
//16:           Si t' ≤ T_k entonces
//17:               s' ← SWTS más cercana a c'
//18:               R_k ← R_k ∪ {s'}
//19:               q_k ← Q      // Vaciar el vehículo
//20:               Actualizar T_k
//21:           Sino
//22:               Romper bucle
//23:           Fin Si
//24:       Fin Si
//25:   Hasta que no se puedan añadir más zonas
//26:   Si la última parada de R_k no es una SWTS entonces
//27:       s' ← SWTS más cercana a la última posición en R_k
//28:       R_k ← R_k ∪ {s'} ∪ {depósito}
//29:   Sino
//30:       R_k ← R_k ∪ {depósito}
//31:   K ← K ∪ {R_k}
//32:   k ← k + 1
//33: Fin Mientras
//34: Retornar K