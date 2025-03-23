/**
 * @class Clase para representar un vehículo de transporte en el sistema
 */


#pragma once
#include "../vehiculo.h"

class Transporte : public Vehiculo {
  public:
  // Constructor de la clase
  Transporte(int capacidad, int velocidad, pair<int, int> posicion, int duracion) : Vehiculo(capacidad, velocidad, posicion, duracion) {}
};