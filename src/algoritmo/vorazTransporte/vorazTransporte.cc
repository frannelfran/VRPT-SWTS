#include "vorazTransporte.h"

/**
 * @brief Método para ordenar el conjunto de tareas en función del tiempo
 * @param tareas Vector con las tareas a ordenar
 * @return vector<Tarea> Vector con las tareas ordenadas
 */
vector<Tarea> VorazTransporte::ordenarTareas(const vector<Tarea>& tareas) {
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
vector<Tarea> VorazTransporte::crearConjuntoTareas(const vector<Recoleccion>& vehiculos) {
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
double VorazTransporte::buscarCantidadMinima(const vector<Tarea>& tareas) {
  double cantidadMinima = INFINITY;
  for (const auto& tarea : tareas) {
    if (tarea.Dh < cantidadMinima) {
      cantidadMinima = tarea.Dh;
    }
  }
  return cantidadMinima;
}

/**
 * @brief Método para ejecutar el algoritmo VorazTransporte
 * @return void
 */
void VorazTransporte::ejecutar() {
  vector<Transporte> rutasDeVehiculos;
  vector<Tarea> tareas = crearConjuntoTareas(datos_.rutasRecoleccion);
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
      vehiculo = new Transporte(datos_.capacidadTransporte, datos_.velocidad, datos_.zonas[3], datos_.duracionTransporte);
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
        vehiculo->moverVehiculo(datos_.zonas[3], vehiculo->getPosicion().getDistancia(datos_.zonas[3]));
        vehiculo->vaciarVehiculo(datos_.zonas[3]); // Vaciar en el vertedero
      }
    }
  }
  // Para cada vehículo de trasnporte revisar si la ultima parada fue el vertedero
  for (auto& vehiculo : rutasDeVehiculos) {
    if (!vehiculo.getPosicion().esDumpsite()) {
      // Nos aseguramos que la ruta finalice en el vertedero
      vehiculo.vaciarVehiculo(datos_.zonas[3]); // Vaciar en el vertedero
      vehiculo.volverAlInicio();
    }
  }
  datos_.rutasTransporte = rutasDeVehiculos; // Guardamos las rutas de los vehículos de transporte
}

/**
 * @brief Método para escoger el vehículo que mínimice el costo de inserción
 * @param vehiculos Vector con los vehículos de transporte
 * @param tarea Tarea a realizar
 * @return Vehiculo& Vehículo que mínimice el costo de inserción
 */
Transporte* VorazTransporte::escogerVehiculo(vector<Transporte>& vehiculos, const Tarea& tarea) {
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
double VorazTransporte::calcularCostoInsercion(const Tarea& tarea, Transporte& vehiculo) {
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
int VorazTransporte::tiempoVolverAlVertedero(const Transporte& vehiculo) {
  int tiempo = 0;
  Transporte vehiculoAux = vehiculo;
  vehiculoAux.setPosicion(datos_.zonas[3]); // Colocamos el vehículo en el vertedero
  // Recorremos las tareas asignadas al vehículo
  for (auto& tarea : vehiculo.getTareasAsignadas()) {
    // Agregamos el tiempo que tarda en ir a la zona de la tarea
    tiempo += vehiculoAux.calcularTiempo(vehiculoAux.getPosicion().getDistancia(tarea.Sh));
    // Actualizamos la posición del vehículo
    vehiculoAux.setPosicion(tarea.Sh);
  }
  // Agrego el tiempo que tarda en volver al vertedero
  tiempo += vehiculo.calcularTiempo(vehiculo.getPosicion().getDistancia(datos_.zonas[3]));
  return tiempo;
}