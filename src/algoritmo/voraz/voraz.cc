#include "voraz.h"
#include "../../tools/tools.h"

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @param vehiculo Vehículo que se va a mover
 * @return Zona más cercana al vehículo con su distancia
 */
pair<Zona&, double> Voraz::zonaMasCercana(const Recoleccion& vehiculo) {
  vector<vector<double>> distancias = dato_->distancias;
  Zona zonaActual = vehiculo.getPosicion();
  Zona* zonaCercana = nullptr; // Inicializo la zona más cercana
  double minDistancia = INFINITY;
  auto it = find(dato_->zonas.begin(), dato_->zonas.end(), zonaActual); // Obtengo la posicion de la zona

  int index = distance(dato_->zonas.begin(), it);
  for (size_t i = 0; i < distancias[index].size(); i++) {
    if (distancias[index][i] < minDistancia &&
        find(vehiculo.getZonasVisitadas().begin(), vehiculo.getZonasVisitadas().end(), dato_->zonas[i]) == vehiculo.getZonasVisitadas().end() &&
        find(dato_->zonasRecoleccion.begin(), dato_->zonasRecoleccion.end(), dato_->zonas[i]) != dato_->zonasRecoleccion.end()) {
      minDistancia = distancias[index][i];
      zonaCercana = &dato_->zonas[i];
    }
  }
  return pair<Zona&, double>(*zonaCercana, minDistancia);
}

/**
 * @brief Método para obtener la swts más cercana
 * @param vehiculo Vehículo que se va a mover
 * @return SWTS más cercana al vehículo con su distancia
 */
pair<Zona&, double> Voraz::swtsMasCercana(const Recoleccion& vehiculo) {
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
 * @return Tiempo que tarda en volver al depósito
 */
int Voraz::TiempoVolverDeposito(Recoleccion vehiculo) {
  int tiempo = 0;
  pair<Zona&, double> zonaTransferenciaCercana = swtsMasCercana(vehiculo);
  pair<Zona&, double> zonaCercana = zonaMasCercana(vehiculo);

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
 * @brief Método para construir las rutas de los vehículos de recolección
 * @return void
 */
void Voraz::ejecutar() {
  auto start = chrono::high_resolution_clock::now();
  calcularRutasRecoleccion(); // Calculamos las rutas de recolección
  calcularRutasTransporte(); // Calculamos las rutas de transporte
  auto end = chrono::high_resolution_clock::now();
  dato_->tiempoCPU = round(chrono::duration_cast<chrono::duration<double>>(end - start).count() * 10000) / 10000.0;
  datos_.push_back(dato_); // Guardamos los datos de la instancia
};

/**
 * @brief Método para mostrar los resultados del algoritmo Voraz
 * @return void
 */
void Voraz::mostrarResultados() {
  // Cabecera
  cout << "----------------------------------------" << endl;
  cout << left 
  << setw(15) << "Instancia" 
  << setw(10) << "#Zonas" 
  << setw(6) << "#CV" 
  << setw(6) << "#TV" 
  << setw(12) << "Tiempo CPU" 
  << endl;
  cout << "----------------------------------------" << endl;

  // Itero sobre los datos
  for (const auto& dato : datos_) {
    cout << left 
    << setw(15) << dato->nombreInstancia 
    << setw(10) << dato->numZonas
    << setw(6) << dato->rutasRecoleccion.size()
    << setw(6) << dato->rutasTransporte.size()
    << setw(12) << dato->tiempoCPU
    << endl;
  }
  cout << "----------------------------------------" << endl;
  // Calculo la media de todas las instancias
  double mediaZonas = 0.0, mediaCV = 0.0, mediaTV = 0.0, mediaCPU = 0.0;
  for (const auto& dato : datos_) {
    mediaZonas += dato->numZonas;
    mediaCV += dato->rutasRecoleccion.size();
    mediaTV += dato->rutasTransporte.size();
    mediaCPU += dato->tiempoCPU;
  }
  mediaZonas /= datos_.size();
  mediaCV /= datos_.size();
  mediaTV /= datos_.size();
  cout << left 
  << setw(15) << "Averages" 
  << setw(10) << mediaZonas
  << setw(6) << mediaCV
  << setw(6) << mediaTV
  << setw(12) << mediaCPU
  << endl;
  cout << "----------------------------------------" << endl;
}

/**
 * @brief Método para calcular las rutas de los vehículos de recolección
 * @return void
 */
void Voraz::calcularRutasRecoleccion() {
  vector<Recoleccion> rutasDeVehiculos;
  vector<Zona>& zonasPendientes = dato_->zonasRecoleccion;

  while (!zonasPendientes.empty()) {
    // Creamos el vehículo
    Recoleccion vehiculo(dato_->capacidadRecoleccion, dato_->velocidad, dato_->zonas[0], dato_->duracionRecoleccion);
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
  dato_->rutasRecoleccion = rutasDeVehiculos; // Guardamos las rutas de los vehículos de recolección
}

/**
 * @brief Método para ordenar el conjunto de tareas en función del tiempo
 * @param tareas Vector con las tareas a ordenar
 * @return vector<Tarea> Vector con las tareas ordenadas
 */
vector<Tarea> Voraz::ordenarTareas(const vector<Tarea>& tareas) {
  vector<Tarea> tareasOrdenadas = tareas;
  sort(tareasOrdenadas.begin(), tareasOrdenadas.end(), [](const Tarea& a, const Tarea& b) {
    return a.Th < b.Th;
  });
  return tareasOrdenadas;
}

/**
 * @brief Método para crear el conjunto de tareas
 * @param vehículos Vector con los vehículos de recolección
 * @return vector<Tarea> Conjunto de tareas
 */
vector<Tarea> Voraz::crearConjuntoTareas(const vector<Recoleccion>& vehiculos) {
  vector<Tarea> tareas;
  for (auto& vehiculo : vehiculos) {
    int totalBasura = 0, totalTiempo = 0;
    Tarea tarea;
    // Recorremos las zonas visitadas por el vehiculo
    for (auto it = vehiculo.getZonasVisitadas().begin(); it != vehiculo.getZonasVisitadas().end(); ++it) {
      Zona zona = *it;
      if (zona.esSWTS()) {
        tarea.Dh = totalBasura;
        tarea.Sh = zona;
        tarea.Th = totalTiempo;
        tareas.push_back(tarea);
        totalBasura = 0;
      }
      else {
        totalBasura += zona.getContenido();
        totalTiempo += vehiculo.calcularTiempo(zona.getDistancia(*next(it))) + zona.getTiempoDeProcesado();
      }
    }
  }
  return ordenarTareas(tareas);
}

/**
 * @brief Método para buscar la cantidad mínima de residuos entre todas las tareas
 * @param tareas Vector con las tareas
 * @return double Cantidad mínima de residuos
 */
double Voraz::buscarCantidadMinima(const vector<Tarea>& tareas) {
  double cantidadMinima = INFINITY;
  for (const auto& tarea : tareas) {
    if (tarea.Dh < cantidadMinima) {
      cantidadMinima = tarea.Dh;
    }
  }
  return cantidadMinima;
}

/**
 * @brief Método para ejecutar el algoritmo Voraz
 * @return void
 */
void Voraz::calcularRutasTransporte() {
  vector<Transporte> rutasDeVehiculos;
  vector<Tarea> tareas = crearConjuntoTareas(dato_->rutasRecoleccion);
  // Muestro las tareas
  cout << "Tareas:" << endl;
  for (const auto& tarea : tareas) {
    cout << "Dh: " << tarea.Dh << ", Sh: " << tarea.Sh.getId() << ", Th: " << tarea.Th << endl;
  }
  double cantidadMínima = buscarCantidadMinima(tareas);
  cout << "Cantidad mínima de residuos: " << cantidadMínima << endl;
  // Mientras queden tareas por hacer
  while (!tareas.empty()) {
    Tarea tareaMinima = tareas.front(); // Tomamos la primera tarea
    tareas.erase(tareas.begin()); // La eliminamos de la lista
    Transporte* vehiculo = nullptr;
    // Buscamos el vehículo que mínimice el costo de inserción
    vehiculo = escogerVehiculo(rutasDeVehiculos, tareaMinima);

    if (vehiculo == nullptr) {
      // Si no hay vehículos disponibles, creamos uno nuevo
      vehiculo = new Transporte(dato_->capacidadTransporte, dato_->velocidad, dato_->zonas[3], dato_->duracionTransporte);
      vehiculo->moverVehiculo(tareaMinima.Sh, vehiculo->getPosicion().getDistancia(tareaMinima.Sh));
      // Llenamos el vehículo con la cantidad de la tarea
      vehiculo->agregarContenido(tareaMinima.Dh);
      vehiculo->agregarTarea(tareaMinima);
      // Agregamos el vehículo a la lista de rutas
      rutasDeVehiculos.push_back(*vehiculo);
    } else {
      // Agregamos la SWTS de la tarea a la ruta del vehículo
      vehiculo->agregarTarea(tareaMinima);
      vehiculo->moverVehiculo(tareaMinima.Sh, tareaMinima.Sh.getDistancia(vehiculo->getPosicion()));
      vehiculo->agregarContenido(tareaMinima.Dh);
      // Si la capacidad remanente es insuficiente para atender la tarea mínima
      if (vehiculo->getContenido() < cantidadMínima) {
        // Nos desplazamos al vertedero para vaciar el vehículo
        vehiculo->moverVehiculo(dato_->zonas[3], vehiculo->getPosicion().getDistancia(dato_->zonas[3]));
        vehiculo->vaciarVehiculo(dato_->zonas[3]); // Vaciar en el vertedero
      }
    }
  }
  // Para cada vehículo de trasnporte revisar si la ultima parada fue el vertedero
  for (auto& vehiculo : rutasDeVehiculos) {
    if (!vehiculo.getPosicion().esDumpsite()) {
      // Nos aseguramos que la ruta finalice en el vertedero
      vehiculo.volverAlInicio();
      vehiculo.vaciarVehiculo(dato_->zonas[3]); // Vaciar en el vertedero
    }
  }
  dato_->rutasTransporte = rutasDeVehiculos; // Guardamos las rutas de los vehículos de transporte
}

/**
 * @brief Método para escoger el vehículo que mínimice el costo de inserción
 * @param vehiculos Vector con los vehículos de transporte
 * @param tarea Tarea a realizar
 * @return Vehiculo& Vehículo que mínimice el costo de inserción
 */
Transporte* Voraz::escogerVehiculo(vector<Transporte>& vehiculos, const Tarea& tarea) {
  double costoMinimo = INFINITY;
  Transporte* vehiculoMinimo = nullptr;

  for (auto& vehiculo : vehiculos) {
    double costo = calcularCostoInsercion(tarea, vehiculo);
    if (costo < costoMinimo) {
      costoMinimo = costo;
      vehiculoMinimo = &vehiculo;
    }
  }
  return vehiculoMinimo;
}

/**
 * @brief Método para calcular el costo de inserción de una tarea en un vehículo
 * @param vehiculo Vehículo a calcular el costo
 * @param tarea Tarea a realizar
 * @return int Costo de inserción
 */
double Voraz::calcularCostoInsercion(const Tarea& tarea, Transporte& vehiculo) {
  int costo;

  // La cantidad de residuos del vehículo debe ser menor a la cantidad de residuos de la tarea
  if (!vehiculo.llenarVehiculo(tarea.Dh)) {
    return INFINITY;
  }
  // El tiempo de la tarea que se quiere asignar menos el tiempo de la tarea anterior debe ser menor o igual que el timepo que tardas en ir
  // a la zona de la tarea que se quiere asignar
  int tiempoTarea = tarea.Th;
  int tiempoAnterior = vehiculo.getTareasAsignadas().back().Th;
  int tiempoViaje = vehiculo.calcularTiempo(vehiculo.getTareasAsignadas().back().Sh.getDistancia(tarea.Sh));

  if (tiempoViaje > (tiempoTarea - tiempoAnterior)) {
    return INFINITY;
  }
  // La duracion total de la ruta considerando el regreso al vertedero no debe exceder el tiempo del vehiculo
  else if (tiempoVolverAlVertedero(vehiculo) > vehiculo.getDuracion()) {
    return INFINITY;
  }
  costo = vehiculo.calcularTiempo(tarea.Sh.getDistancia(vehiculo.getPosicion()));
  return costo;
}

/**
 * @brief Método para calcular el tiempo que tarda el vehículo en volver al vertedero
 * @param vehiculo Vehículo a calcular el tiempo
 * @return int Tiempo que tarda el vehículo en volver al vertedero
 */
int Voraz::tiempoVolverAlVertedero(const Transporte& vehiculo) {
  int tiempo = 0;
  Transporte vehiculoAux = vehiculo;
  vehiculoAux.setPosicion(dato_->zonas[3]); // Colocamos el vehículo en el vertedero
  // Recorremos las tareas asignadas al vehículo
  for (auto& tarea : vehiculo.getTareasAsignadas()) {
    // Agregamos el tiempo que tarda en ir a la zona de la tarea
    tiempo += vehiculoAux.calcularTiempo(vehiculoAux.getPosicion().getDistancia(tarea.Sh));
    // Actualizamos la posición del vehículo
    vehiculoAux.setPosicion(tarea.Sh);
  }
  // Agrego el tiempo que tarda en volver al vertedero
  tiempo += vehiculo.calcularTiempo(vehiculo.getPosicion().getDistancia(dato_->zonas[3]));
  return tiempo;
}