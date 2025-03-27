/**
 * @class Clase para repressentar un algoritmo en el sistema
 */

#pragma once
#include "../vehiculo/vehiculo.h"
#include "../zona/zona.h"
#include "../tools/tools.h"
#include <vector>

using namespace std;

class Algoritmo {
  public:
  // Constructor de la clase
  Algoritmo(const Tools& datos) : datos_(datos) {};

  // Destructor de la clase
  ~Algoritmo() {}

  // Métodos de la clase
  virtual vector<Vehiculo> ejecutar() = 0;

  pair<Zona, double> zonaMasCercana(const vector<Zona>& zonas, const Vehiculo& vehiculo); // Zona más cercana al vehículo

  protected:
  // Datos importantes para el algoritmo
  Tools datos_;
};