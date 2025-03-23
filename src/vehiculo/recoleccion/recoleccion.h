/**
 * @class Clase para representar un vehículo de recolección en el sistema
 */

#pragma once
#include "../vehiculo.h"

class Recoleccion : public Vehiculo {
  public:
  // Constructor de la clase
  Recoleccion(int capacidad, int velocidad, pair<int, int> posicion, int duracion) : Vehiculo(capacidad, velocidad, posicion, duracion) {}
}; 