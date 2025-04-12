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
  static mt19937 gen(random_device{}());
  uniform_int_distribution<size_t> dis(0, mejoresZonas.size() - 1);
  const size_t indiceAleatorio = dis(gen);

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
 * @brief Método para obtener el tiempo en que tarda el vehículo en volver al depósito pasando por las zonas y la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @param numeroMejoresZonas Número de mejores zonas a considerar
 * @return Tiempo que tarda en volver al depósito
 */
int Grasp::TiempoVolverDeposito(Recoleccion vehiculo, const int numeroMejoresZonas) {
  int tiempo = 0;
  pair<Zona&, double> zonaTransferenciaCercana = swtsMasCercana(vehiculo);
  pair<Zona&, double> zonaCercana = zonaMasCercana(vehiculo, numeroMejoresZonas);

  Recoleccion vehiculoAux = vehiculo;
  vehiculoAux.setPosicion(zonaTransferenciaCercana.first);
  // Tiempo que tarda en llegar a la zona más cercana
  tiempo += vehiculo.calcularTiempo(vehiculo.getPosicion().getDistancia(zonaCercana.first));
  // Tiempo que tarda en procesar la zona más cercana
  tiempo += zonaCercana.first.getTiempoDeProcesado();
  // Tiempo que tarda en llegar a la swts más cercana
  tiempo += vehiculo.calcularTiempo(zonaCercana.first.getDistancia(zonaTransferenciaCercana.first));
  // Tiempo que tarda en volver al depósito desde la swts más cercana
  tiempo += vehiculoAux.calcularTiempo(dato_->zonas[0].getDistancia(vehiculoAux.getPosicion()));

  return tiempo;
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
      Tools copiaDato = datoOriginal; // Copiamos el dato original
      dato_ = new Tools(copiaDato); // Creamos una nueva instancia de Tools
      vector<Recoleccion> rutasDeVehiculos;
      vector<Zona>& zonasPendientes = dato_->zonasRecoleccion;
      while (!zonasPendientes.empty()) {
        // Creamos el vehículo
        Recoleccion vehiculo(dato_->capacidadRecoleccion, dato_->velocidad, dato_->zonas[0], dato_->duracionRecoleccion);
        do {
          if (zonasPendientes.empty()) break;
          pair<Zona&, double> zonaCercana = zonaMasCercana(vehiculo, i);
          int tiempoEnVolverAlDeposito = TiempoVolverDeposito(vehiculo, i);
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
      mejoresZonasYEjecuciones_.push_back(make_pair(i, j));
    }
  }
}

/**
 * @brief Método para ejecutar el algoritmo GRASP
 * @return void
 */
void Grasp::ejecutar() {
  calcularRutasRecoleccion(); // Calculamos las rutas de recolección
}

/**
 * @brief Método para mostrar los resultados del algoritmo GRASP
 * @return void
 */
void Grasp::mostrarResultados() {
  // Cabecera
  cout << "----------------------------------------------------------------" << endl;
  cout << left 
  << setw(15) << "Instancia" 
  << setw(10) << "#Zonas"
  << setw(6) << "|LRC|"
  << setw(10) << "Ejecucion"
  << setw(6) << "#CV"
  << setw(6) << "#TV"
  << setw(12) << "Tiempo CPU" 
  << endl;
  cout << "----------------------------------------------------------------" << endl;

  // Itero sobre los datos
  int numeroMejoresZonas = 2;
  int numeroEjecuciones = 1;

  // Recorro los datos junto con los mejoresZonasYEjecuciones
  for (size_t i = 0; i < datos_.size(); i++) {
    const auto& dato = datos_[i];
    const int mejoresZonas = mejoresZonasYEjecuciones_[i].first;
    const int ejecucion = mejoresZonasYEjecuciones_[i].second;
    cout << left
    << setw(15) << dato->nombreInstancia
    << setw(10) << dato->numZonas
    << setw(6) << mejoresZonas
    << setw(10) << ejecucion
    << setw(6) << dato->rutasRecoleccion.size()
    << setw(6) << dato->rutasTransporte.size()
    << setw(12) << dato->tiempoCPU
    << endl;
  }

  cout << "----------------------------------------------------------------" << endl;
  // Calculo la media de todas las instancias
  double mediaZonas = 0.0, mediaMejoresZonas = 0.0, mediaEjecucion = 0.0, mediaCV = 0.0, mediaTV = 0.0, mediaCPU = 0.0;
  
  for (size_t i = 0; i < datos_.size(); i++) {
    const auto& dato = datos_[i];
    const int mejoresZonas = mejoresZonasYEjecuciones_[i].first;
    const int ejecucion = mejoresZonasYEjecuciones_[i].second;
    mediaZonas += dato->numZonas;
    mediaMejoresZonas += mejoresZonas;
    mediaEjecucion += ejecucion;
    mediaCV += dato->rutasRecoleccion.size();
    mediaTV += dato->rutasTransporte.size();
    mediaCPU += dato->tiempoCPU;
  }
  mediaZonas /= datos_.size();
  mediaMejoresZonas /= datos_.size();
  mediaEjecucion /= datos_.size();
  mediaCV /= datos_.size();
  mediaTV /= datos_.size();
  mediaCPU /= datos_.size();

  cout << left 
  << setw(15) << "Averages" 
  << setw(10) << mediaZonas
  << setw(6) << mediaMejoresZonas
  << setw(10) << mediaEjecucion
  << setw(6) << fixed << setprecision(2) << mediaCV
  << setw(6) << mediaTV
  << setw(12) << mediaCPU
  << endl;
  cout << "----------------------------------------------------------------" << endl;
}