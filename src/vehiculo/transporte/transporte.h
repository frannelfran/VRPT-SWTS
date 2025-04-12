/**
 * @class Clase para representar un vehículo de transporte en el sistema
 */

#ifndef C_Transporte_H
#define C_Transporte_H

#include "../vehiculo.h"

struct Tarea {
  double Dh; // Cantidad de residuos
  Zona Sh; // Zona a la que se va a mover el vehículo
  int Th; // Tiempo que tarda en llegar a la zona
};

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

#endif