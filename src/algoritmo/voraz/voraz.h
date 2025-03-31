/**
 * @class Algortimo Voraz para determinar la ruta de los vehículos de recolección
 */

#pragma once
#include "../algoritmo.h"

class Voraz : public Algoritmo {
  public:
  // Constructor de la clase
  Voraz(const Tools& datos) : Algoritmo(datos) {};

  // Métodos de la clase
  vector<Vehiculo> ejecutar() override;
  vector<Zona> zonasDeRecoleccion() const;
};