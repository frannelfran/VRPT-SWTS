/**
 * @class Clase para representar un vehículo de transporte en el sistema
 */

#pragma once
#include "../vehiculo.h"
#include "../../algoritmo/vorazTransporte/vorazTransporte.h"

class Tarea;

class Transporte : public Vehiculo {
  public:
    // Constructor de la clase
    Transporte(const int& capacidad, const int& velocidad, const Zona& posicion, const int& duracion) : Vehiculo(capacidad, velocidad, posicion, duracion) {}

    // Métodos de la clase
    inline void agregarTarea(const Tarea& tarea) { tareasAsignadas_.push_back(tarea); } // Agregar una tarea al vehículo

    // Getters
    inline const vector<Tarea>& getTareasAsignadas() const { return tareasAsignadas_; } // Obtener las tareas asignadas al vehículo

  private:
    vector<Tarea> tareasAsignadas_; // Tareas asignadas al vehículo
};