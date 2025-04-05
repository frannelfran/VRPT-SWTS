/**
 * @class Clase para representar a los vehículos de recolección
 */

#pragma once
#include "../vehiculo.h"

class Recoleccion : public Vehiculo {
  public:
    // Constructor de la clase
    Recoleccion(const int& capacidad, const int& velocidad, const Zona& posicion, const int& duracion) : Vehiculo(capacidad, velocidad, posicion, duracion) {}
    
    // Destructor de la clase
    ~Recoleccion() {}
    // Métodos de la clase
    void vaciarZona(Zona& zona);
};