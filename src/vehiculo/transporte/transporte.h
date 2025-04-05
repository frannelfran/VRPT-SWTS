/**
 * @class Clase para representar un vehículo de transporte en el sistema
 */

#pragma once
#include "../vehiculo.h"
#include "../../algoritmo/grasp/grasp.h"

class Tarea;

class Transporte : public Vehiculo {
  public:
    // Constructor de la clase
    Transporte(const int& capacidad, const int& velocidad, const Zona& posicion, const int& duracion) : 
      Vehiculo(capacidad, velocidad, posicion, duracion) {}

  private:
    vector<Tarea> tareasAsignadas_; // Tareas asignadas al vehículo
};