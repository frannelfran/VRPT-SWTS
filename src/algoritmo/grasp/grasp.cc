#include "grasp.h"

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
      if (zona.getId() == "IF" || zona.getId() == "IF1") {
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
  return tareas;
}

/**
 * @brief Método para ejecutar el algoritmo GRASP
 * @return vector<Vehiculo> Rutas de los vehiculos de transporte
 */
vector<Vehiculo> Grasp::ejecutar() {
  vector<Tarea> tareas = crearConjuntoTareas(datos_.rutasRecoleccion);
  // Muestro el conjunto de tareas
  cout << "Conjunto de tareas:" << endl;
  for (auto& tarea : tareas) {
    cout << "Dh: " << tarea.Dh << ", Sh: " << tarea.Sh << ", Th: " << tarea.Th << endl;
  }
  exit(0);
  vector<Vehiculo> rutas;
  return rutas;
  
}