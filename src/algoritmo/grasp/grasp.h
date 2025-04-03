/**
 * @class Clase para representar un algoritmo GRASP en el sistema
 */

#pragma once
#include "../algoritmo.h"
#include "../../vehiculo/vehiculo.h"
#include "../../zona/zona.h"
#include "../../tools/tools.h"

struct Tarea {
  int Dh; // Cantidad de residuos tranposrtados por el vehículo derecolección
  string Sh; // SWTS visitada por el vehículo de recolección
  int Th; // Tiempo en el que el vehículo de recolección llega a la SWTS
};

class Grasp : public Algoritmo {
  public:
  // Constructor de la clase
  Grasp(Tools& datos) : Algoritmo(datos) {}

  // Destructor de la clase
  ~Grasp() {}

  // Método para ejecutar el algoritmo GRASP
  vector<Vehiculo> ejecutar() override;

  // Método para crear el conjunto de tareas
  vector<Tarea> crearConjuntoTareas(const vector<Vehiculo>& vehiculos);
};