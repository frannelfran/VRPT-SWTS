/**
 * @class Clase para repressentar un vehículo en el sistema
 */

#pragma once
#include <iostream>
#include <string>

using namespace std;

class Vehiculo {
  // Constructor de la clase
  public:
  Vehiculo(int capacidad, int velocidad, pair<int, int> posicion, int duracion);

  protected:
  int capacidad_;
  int velocidad_;
  pair<int, int> posicion_;
  int duracion_;
};