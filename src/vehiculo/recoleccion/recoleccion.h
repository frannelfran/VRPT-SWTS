/**
 * @class Clase para representar un vehículo de recolección en el sistema
 */

#pragma once
#include "../vehiculo.h"

class Recoleccion : public Vehiculo {
  public:
  // Constructor de la clase
  Recoleccion(const int& capacidad, const int& velocidad, const pair<int, int>& posicion, const int& duracion) : Vehiculo(capacidad, velocidad, posicion, duracion) {}
}; 