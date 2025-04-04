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
        tarea.Sh = zona.getId();
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
vector<Vehiculo> Grasp::ejecutar() {
  vector<Vehiculo> rutasDeVehiculos;
  vector<Tarea> tareas = crearConjuntoTareas(datos_.rutasRecoleccion);
  // Muestro las tareas
  cout << "Tareas:" << endl;
  for (const auto& tarea : tareas) {
    cout << "Dh: " << tarea.Dh << ", Sh: " << tarea.Sh << ", Th: " << tarea.Th << endl;
  }
  double cantidadMínima = buscarCantidadMinima(tareas);
  // Mientras queden tareas por hacer
  while (!tareas.empty()) {
    Tarea tarea = tareas.front(); // Tomamos la primera tarea
    tareas.erase(tareas.begin()); // La eliminamos de la lista
  }
}