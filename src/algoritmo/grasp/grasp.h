/**
 * @class Clase para representar un algoritmo GRASP en el sistema
 */

#pragma once
#include "../algoritmo/algoritmo.h"
#include "../vehiculo/vehiculo.h"
#include "../zona/zona.h"
#include "../tools/tools.h"

class Grasp : public Algoritmo {
  public:
  // Constructor de la clase
  Grasp(Tools& datos) : Algoritmo(datos) {}

  // Destructor de la clase
  ~Grasp() {}

  // Método para ejecutar el algoritmo GRASP
  vector<Vehiculo> ejecutar() override;
};