#include "grasp.h"
#include "../../tools/tools.h"

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @param vehiculo Vehículo que se va a mover
 * @param numeroMejoresZonas Número de mejores zonas a considerar
 * @return Zona más cercana al vehículo con su distancia
 */
pair<Zona&, double> Grasp::zonaMasCercana(const Recoleccion& vehiculo, const int numeroMejoresZonas) {
  vector<vector<double>> distancias = dato_->distancias;
  Zona zonaActual = vehiculo.getPosicion();
  vector<pair<Zona*, double>> zonaCercana; // Vector con las mejores zonas cercanas
  auto it = find(dato_->zonas.begin(), dato_->zonas.end(), zonaActual); // Obtengo la posicion de la zona
  
  int index = distance(dato_->zonas.begin(), it);
  
  vector<pair<Zona*, double>> mejoresZonas; // Vector para almacenar las mejores zonas y sus distancias

  for (int i = 1; i <= numeroMejoresZonas; i++) {
    double minDistanciaZona = INFINITY;
    Zona* mejorZona = nullptr;

    for (size_t j = 0; j < distancias[index].size(); j++) {
      if (distancias[index][j] < minDistanciaZona &&
          find(vehiculo.getZonasVisitadas().begin(), vehiculo.getZonasVisitadas().end(), dato_->zonas[j]) == vehiculo.getZonasVisitadas().end() &&
          find(dato_->zonasRecoleccion.begin(), dato_->zonasRecoleccion.end(), dato_->zonas[j]) != dato_->zonasRecoleccion.end() &&
          find(mejoresZonas.begin(), mejoresZonas.end(), make_pair(&dato_->zonas[j], distancias[index][j])) == mejoresZonas.end()) {
        minDistanciaZona = distancias[index][j];
        mejorZona = &dato_->zonas[j];
      }
    }
    if (mejorZona != nullptr) {
      mejoresZonas.push_back(make_pair(mejorZona, minDistanciaZona));
    }
  }
  // Escogemos una zona al azar entre las mejores zonas
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, mejoresZonas.size() - 1);
  int indiceAleatorio = dis(gen);
  return pair<Zona&, double>(*mejoresZonas[indiceAleatorio].first, mejoresZonas[indiceAleatorio].second);
}

/**
 * @brief Método para obtener la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @return SWTS más cercana al vehículo con su distancia
 */
pair<Zona&, double> Grasp::swtsMasCercana(const Recoleccion& vehiculo) {
  vector<vector<double>> distancias = dato_->distancias;
  Zona zonaActual = vehiculo.getPosicion();
  Zona* swtsCercana = nullptr;
  double minDistancia = INFINITY;

  auto it = find(dato_->zonas.begin(), dato_->zonas.end(), zonaActual);

  int posicion = distance(dato_->zonas.begin(), it);

  for (size_t i = 0; i < distancias[posicion].size(); i++) {
    Zona& zona = dato_->zonas[i];
    if (!zona.esSWTS()) continue;
    else {
      if (distancias[posicion][i] < minDistancia) {
        minDistancia = distancias[posicion][i];
        swtsCercana = &dato_->zonas[i];
      }
    }
  }
  return pair<Zona&, double>(*swtsCercana, minDistancia);
}

/**
 * @brief Método para calcular las rutas de los vehículos de recolección
 * @return void
 */
void Grasp::calcularRutasRecoleccion() {
  Tools datoOriginal = *dato_; // Guardamos el dato original
  // Para todas las ejecuciones
  for (int i = 2; i <= numeroMejoresZonasCercanas_; i++) {
    for (int j = 1; j <= numeroEjecuciones_; j++) {
      dato_ = &datoOriginal;
      vector<Recoleccion> rutasDeVehiculos;
      vector<Zona>& zonasPendientes = dato_->zonasRecoleccion;
      while (!zonasPendientes.empty()) {
        // Creamos el vehículo
        Recoleccion vehiculo(dato_->capacidadRecoleccion, dato_->velocidad, dato_->zonas[0], dato_->duracionRecoleccion);
        do {
          if (zonasPendientes.empty()) break;
          pair<Zona&, double> zonaCercana = zonaMasCercana(vehiculo, i);
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
      dato_->rutasRecoleccion = rutasDeVehiculos;
      datos_.push_back(dato_); // Guardamos los datos de la instancia
    }
  }
}

/**
 * @brief Método para ejecutar el algoritmo GRASP
 * @return void
 */
void Grasp::ejecutar() {
  calcularRutasRecoleccion(); // Calculamos las rutas de recolección
  mostrarResultados(); // Mostramos los resultados
}