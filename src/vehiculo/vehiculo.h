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
  Vehiculo(const int& capacidad, const int& velocidad, const pair<int, int>& posicion, const int& duracion);

  protected:
  int capacidad_;
  int velocidad_;
  pair<int, int> posicion_;
  int duracion_;
};