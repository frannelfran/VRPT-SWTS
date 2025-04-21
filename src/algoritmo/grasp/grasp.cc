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
 * @param numeroMejoresZonas Número de mejores zonas a considerar
 * @param ejecucion Número de ejecución
 * @return void
 */
void Grasp::calcularRutasRecoleccion(const int numeroMejoresZonas, const int ejecucion) {
  vector<Recoleccion> rutasDeVehiculos;
  vector<Zona>& zonasPendientes = dato_->zonasRecoleccion;
  while (!zonasPendientes.empty()) {
    // Creamos el vehículo
    Recoleccion vehiculo(dato_->capacidadRecoleccion, dato_->velocidad, dato_->zonas[0], dato_->duracionRecoleccion);
    do {
      if (zonasPendientes.empty()) break;
      pair<Zona&, double> zonaCercana = zonaMasCercana(vehiculo, numeroMejoresZonas);
      int tiempoEnVolverAlDeposito = TiempoVolverDeposito(vehiculo, numeroMejoresZonas);
      // Si el contendio de la zona es menor a la capacidad del vehículo y le da tiempo a volver al deposito
      if (vehiculo.llenarVehiculo(zonaCercana.first.getContenido()) && tiempoEnVolverAlDeposito <= (vehiculo.getDuracion() - vehiculo.getTiempoTranscurrido())) {
        vehiculo.moverVehiculo(zonaCercana.first, zonaCercana.second);
        vehiculo.vaciarZona(zonaCercana.first);
        zonasPendientes.erase(remove(zonasPendientes.begin(), zonasPendientes.end(), zonaCercana.first), zonasPendientes.end());
      } else if (tiempoEnVolverAlDeposito <= (vehiculo.getDuracion() - vehiculo.getTiempoTranscurrido())) {
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
  mejoresZonasYEjecuciones_.push_back(make_pair(numeroMejoresZonas, ejecucion)); // Guardamos el número de mejores zonas y la ejecución
}

/**
 * @brief Método para calcular la distancia de recolección
 * @return Distancia de recolección
 */
double Grasp::CalcularDistanciaRecoleccion() {
  double distancia = 0.0;
  for (const auto& vehiculo : dato_->rutasRecoleccion) {
    for (auto it = vehiculo.getZonasVisitadas().begin(); it != vehiculo.getZonasVisitadas().end(); ++it) {
      Zona zona = *it;
      if (it + 1 != vehiculo.getZonasVisitadas().end()) {
        distancia += zona.getDistancia(*next(it));
      }
    }
  }
  return distancia;
}

/**
 * @brief Método para ejecutar el algoritmo GRASP
 * @return void
 */
void Grasp::ejecutar() {
  Tools datoOriginal = *dato_;
  auto voraz = make_unique<Voraz>();
  BusquedaLocal local;
  for (int i = 2; i <= numeroMejoresZonasCercanas_; i++) {
    for (int j = 1; j <= numeroEjecuciones_; j++) {
      Tools copiaDato = datoOriginal; // Copiamos el dato original
      dato_ = new Tools(copiaDato); // Creamos una nueva instancia de Tools
      auto start = chrono::high_resolution_clock::now();
      calcularRutasRecoleccion(i, j); // Calculamos las rutas de recolección
      distanciaSinMejoras.push_back(CalcularDistanciaRecoleccion());
      // Mejoro las rutas
      local.setVehiculos(dato_->rutasRecoleccion);
      local.mejorarRutas();
      distanciasConMejoras_.push_back(CalcularDistanciaRecoleccion());
      // Calculo las rutas de transporte
      voraz->setDato(*dato_);
      voraz->calcularRutasTransporte();

      auto end = chrono::high_resolution_clock::now();
      dato_->tiempoCPU = std::chrono::duration<double>(end - start).count();
    }
  }
}

/**
 * @brief Método para obtener las diferentes ejecuciones para un misma instancia
 * @param dato Dato a obtener
 * @return Dato
 */
vector<Tools*> Grasp::getDato(const Tools& dato) {
  vector<Tools*> datos;
  for (const auto& d : datos_) {
    if (d->nombreInstancia == dato.nombreInstancia) {
      datos.push_back(d);
    }
  }
  return datos;
}

/**
 * @brief Método para mostrar los resultados del algoritmo GRASP
 * @return void
 */
void Grasp::mostrarResultados() {
  // Cabecera
  cout << "--------------------------------------------------------------------------------------------" << endl;
  cout << left 
  << setw(20) << "Instancia" 
  << setw(15) << "#Zonas"
  << setw(10) << "|LRC|"
  << setw(15) << "Ejecucion"
  << setw(10) << "#CV"
  << setw(10) << "#TV"
  << setw(15) << "Tiempo CPU" 
  << endl;
  cout << "--------------------------------------------------------------------------------------------" << endl;

  // Recorro los datos junto con los mejoresZonasYEjecuciones
  for (size_t i = 0; i < datos_.size(); i++) {
    const auto& dato = datos_[i];
    const int mejoresZonas = mejoresZonasYEjecuciones_[i].first;
    const int ejecucion = mejoresZonasYEjecuciones_[i].second;
    cout << left
    << setw(20) << dato->nombreInstancia
    << setw(15) << dato->numZonas
    << setw(10) << mejoresZonas
    << setw(15) << ejecucion
    << setw(10) << dato->rutasRecoleccion.size()
    << setw(10) << dato->rutasTransporte.size()
    << setw(15) << dato->tiempoCPU
    << endl;
  }

  cout << "--------------------------------------------------------------------------------------------" << endl;
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
  << setw(20) << "Averages" 
  << setw(15) << mediaZonas
  << setw(10) << mediaMejoresZonas
  << setw(15) << mediaEjecucion
  << setw(10) << mediaCV
  << setw(10) << mediaTV
  << setw(15) << mediaCPU
  << endl;
  cout << "--------------------------------------------------------------------------------------------" << endl;

  mostrarDistancias();
}

/**
 * @brief Método para mostrar los tiempos con y sin mejoras
 * @return void
 */
void Grasp::mostrarDistancias() {
  cout << "--------------------------------------------------------------------------------------------" << endl;
  cout << left 
  << setw(20) << "Instancia" 
  << setw(25) << "Distancia sin mejoras"
  << setw(25) << "Distancia con mejoras"
  << endl;
  cout << "--------------------------------------------------------------------------------------------" << endl;

  for (size_t i = 0; i < datos_.size(); i++) {
    const auto& dato = datos_[i];
    cout << left
    
    << setw(20) << dato->nombreInstancia
    << setw(25) << distanciaSinMejoras[i]
    << setw(25) << distanciasConMejoras_[i]
    << endl;
  }
  cout << "--------------------------------------------------------------------------------------------" << endl;
  // Calculo la media
  double mediaSinMejoras = 0, mediaConMejoras = 0;
  for (size_t i = 0; i < datos_.size(); i++) {
    mediaSinMejoras += distanciaSinMejoras[i];
    mediaConMejoras +=distanciasConMejoras_[i];
  }
  mediaSinMejoras /= datos_.size();
  mediaConMejoras /= datos_.size();
  cout << left
  << setw(20) << "Averages"
  << setw(25) << mediaSinMejoras
  << setw(25) << mediaConMejoras
  << endl;
  cout << "--------------------------------------------------------------------------------------------" << endl;
}