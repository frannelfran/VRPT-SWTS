#include "grasp.h"

/**
 * @brief Método para ordenar el conjunto de tareas en función del tiempo
 * @param tareas Vector con las tareas a ordenar
 * @return vector<Tarea> Vector con las tareas ordenadas
 */
vector<Tarea> Grasp::ordenarTareas(const vector<Tarea>& tareas) {
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
vector<Tarea> Grasp::crearConjuntoTareas(const vector<Vehiculo>& vehiculos) {
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
double Grasp::buscarCantidadMinima(const vector<Tarea>& tareas) {
  double cantidadMinima = INFINITY;
  for (const auto& tarea : tareas) {
    if (tarea.Dh < cantidadMinima) {
      cantidadMinima = tarea.Dh;
    }
  }
  return cantidadMinima;
}

/**
 * @brief Método para ejecutar el algoritmo GRASP
 * @return vector<Vehiculo> Rutas de los vehiculos de transporte
 */
//vector<Vehiculo> Grasp::ejecutar() {
//  vector<Vehiculo> rutasDeVehiculos;
//  vector<Tarea> tareas = crearConjuntoTareas(datos_.rutasRecoleccion);
//  // Muestro las tareas
//  cout << "Tareas:" << endl;
//  for (const auto& tarea : tareas) {
//    cout << "Dh: " << tarea.Dh << ", Sh: " << tarea.Sh.getId() << ", Th: " << tarea.Th << endl;
//  }
//  double cantidadMínima = buscarCantidadMinima(tareas);
//  // Mientras queden tareas por hacer
//  while (!tareas.empty()) {
//    Tarea tareaMinima = tareas.front(); // Tomamos la primera tarea
//    tareas.erase(tareas.begin()); // La eliminamos de la lista
//    Vehiculo* vehiculo = nullptr;
//    // Buscamos el vehículo que mínimice el costo de inserción
//    vehiculo = escogerVehiculo(rutasDeVehiculos, tareaMinima);
//
//    if (vehiculo == nullptr) {
//      Vehiculo vehiculoNuevo(datos_.capacidadTransporte, datos_.velocidad, datos_.zonas[3], datos_.duracionTransporte);
//      vehiculoNuevo.moverVehiculo(tareaMinima.Sh, tareaMinima.Sh.getDistancia(vehiculoNuevo.getPosicion()));
//      vehiculoNuevo.vaciarZona(tareaMinima.Sh, tareaMinima.Dh);
//      rutasDeVehiculos.push_back(vehiculoNuevo);
//    }
//    else {
//      // Movemos el vehículo a la zona de transferencia
//      vehiculo->moverVehiculo(tareaMinima.Sh, tareaMinima.Sh.getDistancia(vehiculo->getPosicion()));
//      vehiculo->vaciarZona(tareaMinima.Sh, tareaMinima.Dh);
//      // Si la capacidad remanente es menor es insuficiente para atender la tarea mínima
//      if  (vehiculo->getContenido() < cantidadMínima) {
//        // Volvemos al vertedero
//        vehiculo->moverVehiculo(datos_.zonas[3], vehiculo->getPosicion().getDistancia(datos_.zonas[3]));
//        vehiculo->vaciarVehiculo(datos_.zonas[3]);
//      }
//    }
//  }
//  for (auto& vehiculo : rutasDeVehiculos) {
//    // Si la última parada no es el vertedero
//    if (vehiculo.getPosicion().getId() != "Dumpsite") {
//      vehiculo.volverAlInicio(); // Volvemos al vertedero
//    }
//  }
//  return rutasDeVehiculos;
//}

/**
 * @brief Método para escoger el vehículo que mínimice el costo de inserción
 * @param vehiculos Vector con los vehículos de transporte
 * @param tarea Tarea a realizar
 * @return Vehiculo& Vehículo que mínimice el costo de inserción
 */
Vehiculo* Grasp::escogerVehiculo(vector<Vehiculo>& vehiculos, const Tarea& tarea) {
  Vehiculo* vehiculoMinimo = nullptr;
  if (vehiculos.empty()) {
    return vehiculoMinimo;
  }
  else {
    // Devuelvo el primer vehiculo
    vehiculoMinimo = &vehiculos[0];
    return vehiculoMinimo;
  }
}

/**
 * @brief Método para calcular el costo de inserción de una tarea en un vehículo
 * @param vehiculo Vehículo a calcular el costo
 * @param tarea Tarea a realizar
 * @return int Costo de inserción
 */
int Grasp::calcularCostoInsercion(const Tarea& tarea, const Vehiculo& vehiculo) {
  int costo = 0;

  







  

  return costo;
}