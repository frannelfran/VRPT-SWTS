/**
 * @class Clase para repressentar un algoritmo en el sistema
 */

#pragma once
#include "vehiculo/vehiculo.h"
#include "zona/zona.h"
#include "tools/tools.h"

using namespace std;

class Algoritmo {
  public:
  // Constructor de la clase
  Algoritmo(const Tools& datos) : datos_(datos) {};

  // Métodos de la clase
  virtual vector<Vehiculo> ejecutar();

  protected:
  // Datos importantes para el algoritmo
  Tools datos_;
};