/**
 * @class Clase para representar un vehículo de transporte en el sistema
 */


#pragma once
#include "../vehiculo.h"

class Transporte : public Vehiculo {
  public:
  // Constructor de la clase
  Transporte(const int& capacidad, const int& velocidad, const pair<int, int>& posicion, const int& duracion) : Vehiculo(capacidad, velocidad, posicion, duracion) {}
};