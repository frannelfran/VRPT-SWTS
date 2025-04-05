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
vector<Tarea> VorazTransporte::crearConjuntoTareas(const vector<Vehiculo>& vehiculos) {
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
 * @return vector<Vehiculo> Rutas de los vehiculos de transporte
 */
vector<Vehiculo> VorazTransporte::ejecutar() {
  vector<Transporte> rutasDeVehiculos;
  vector<Tarea> tareas = crearConjuntoTareas(datos_.rutasRecoleccion);
  // Muestro las tareas
  cout << "Tareas:" << endl;
  for (const auto& tarea : tareas) {
    cout << "Dh: " << tarea.Dh << ", Sh: " << tarea.Sh.getId() << ", Th: " << tarea.Th << endl;
  }
  double cantidadMínima = buscarCantidadMinima(tareas);
  // Mientras queden tareas por hacer
  while (!tareas.empty()) {
    Tarea tareaMinima = tareas.front(); // Tomamos la primera tarea
    tareas.erase(tareas.begin()); // La eliminamos de la lista
    Transporte* vehiculo = nullptr;
    // Buscamos el vehículo que mínimice el costo de inserción
    
    

  }
}

/**
 * @brief Método para escoger el vehículo que mínimice el costo de inserción
 * @param vehiculos Vector con los vehículos de transporte
 * @param tarea Tarea a realizar
 * @return Vehiculo& Vehículo que mínimice el costo de inserción
 */
Transporte& VorazTransporte::escogerVehiculo(vector<Transporte>& vehiculos, const Tarea& tarea) {
  double costoMinimo = INFINITY;
  Transporte* vehiculoMinimo = nullptr;

  for (auto& vehiculo : vehiculos) {
    int costo = calcularCostoInsercion(tarea, vehiculo);
    if (costo < costoMinimo) {
      costoMinimo = costo;
      vehiculoMinimo = &vehiculo;
    }
  }
  return *vehiculoMinimo;
}

/**
 * @brief Método para calcular el costo de inserción de una tarea en un vehículo
 * @param vehiculo Vehículo a calcular el costo
 * @param tarea Tarea a realizar
 * @return int Costo de inserción
 */
int VorazTransporte::calcularCostoInsercion(const Tarea& tarea, Transporte& vehiculo) {
  int costo;

  // La cantidad de residuos del vehículo debe ser menor a la cantidad de residuos de la tarea
  if (!vehiculo.llenarVehiculo(tarea.Dh)) {
    return INFINITY;
  }
  // El tiempo de la tarea que se quiere asignar menos el tiempo de la tarea anterior debe ser menor o igual que el timepo que tardas en ir
  // a la zona de la tarea que se quiere asignar
  int tiempoTarea = tarea.Th;
  int tiempoAnterior = vehiculo.getTareasAsignadas().back().Th;
  int tiempoViaje = vehiculo.calcularTiempo(tarea.Sh.getDistancia(vehiculo.getPosicion()));





  







  

  return costo;
}