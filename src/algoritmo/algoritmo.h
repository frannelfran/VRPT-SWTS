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
  pair<Zona, double> swtsMasCercana(const vector<Zona>& zonas, const Vehiculo& vehiculo); // SWTS más cercana a la zona
  int TiempoVolverDeposito(Vehiculo vehiculo, Zona zonaCercana); // Tiempo que tarda en volver al depósito pasando por las zonas y la swts más cercana

  protected:
  // Datos importantes para el algoritmo
  Tools datos_;
};