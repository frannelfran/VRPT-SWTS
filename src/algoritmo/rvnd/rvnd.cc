#include "rvnd.h"

/**
 * @brief Método para buscar la mejor ruta en base al número de vehículos que tenga
 * @return Tools* Mejor ruta
 */

Tools* RVND::mejorRuta() {
  Tools* mejorRuta = nullptr;
  double numVehiculos = INFINITY;
  // Buscamos la mejor ruta
  for (auto& busquedaLocal : busquedasLocales_) {
    if ((busquedaLocal->rutasRecoleccion.size() + busquedaLocal->rutasTransporte.size()) < numVehiculos) {
      numVehiculos = busquedaLocal->rutasRecoleccion.size() + busquedaLocal->rutasTransporte.size();
      mejorRuta = busquedaLocal;
    }
  }
  return mejorRuta;
}

/**
 * @brief Método para ejecutar el algoritmo RVND
 * @return void
 */

void RVND::ejecutar() {
  auto grasp = make_unique<Grasp>(mejoresZonasCercanas_, numeroEjecuciones_);
  grasp->setDato(*dato_);

  auto start = chrono::high_resolution_clock::now();
  grasp->ejecutar();
  // Guardamos los datos de la instancia
  busquedasLocales_ = grasp->getDato(*dato_);

  // Guardamos las distancias
  
  // Buscamos la mejor ruta
  datos_.push_back(mejorRuta());
  distancias_.push_back(datos_.back()->calcularDistanciaRecoleccion());
  auto end = chrono::high_resolution_clock::now();
  dato_->tiempoCPU = std::chrono::duration<double>(end - start).count();
}

/**
 * @brief Método para mostrar los resultados del algoritmo RVND
 * @return void
 */
void RVND::mostrarResultados() {
  // Cabecera
  cout << "------------------------------------------------------------" << endl;
  cout << left 
  << setw(15) << "Instancia" 
  << setw(10) << "#Zonas" 
  << setw(6) << "#CV" 
  << setw(6) << "#TV" 
  << setw(12) << "Distancia"
  << setw(12) << "Tiempo CPU" 
  << endl;
  cout << "------------------------------------------------------------" << endl;

  // Itero sobre los datos
  for (size_t i = 0; i < datos_.size(); i++) {
    const auto& dato = datos_[i];
    cout << left 
    << setw(15) << dato->nombreInstancia 
    << setw(10) << dato->numZonas
    << setw(6) << dato->rutasRecoleccion.size()
    << setw(6) << dato->rutasTransporte.size()
    << setw(12) << distancias_[i]
    << setw(12) << dato->tiempoCPU
    << endl;
  }
  cout << "------------------------------------------------------------" << endl;
  // Calculo la media de todas las instancias
  double mediaZonas = 0.0, mediaCV = 0.0, mediaTV = 0.0, mediaCPU = 0.0, mediaDistancia = 0.0;
  for (size_t i = 0; i < datos_.size(); i++) {
    const auto& dato = datos_[i];
    mediaZonas += dato->numZonas;
    mediaCV += dato->rutasRecoleccion.size();
    mediaTV += dato->rutasTransporte.size();
    mediaDistancia += distancias_[i];
    mediaCPU += dato->tiempoCPU;
  }
  mediaZonas /= datos_.size();
  mediaCV /= datos_.size();
  mediaTV /= datos_.size();
  mediaDistancia /= datos_.size();
  mediaCPU /= datos_.size();

  cout << left 
  << setw(15) << "Averages" 
  << setw(10) << mediaZonas
  << setw(6) << mediaCV
  << setw(6) << mediaTV
  << setw(12) << mediaDistancia
  << setw(12) << mediaCPU
  << endl;
  cout << "------------------------------------------------------------" << endl;
}