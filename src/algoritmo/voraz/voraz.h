/**
 * @class Algortimo Voraz para determinar la ruta de los vehículos de recolección
 */

#pragma once
#include "../algoritmo.h"
#include "../../vehiculo/recoleccion/recoleccion.h"

class Voraz : public Algoritmo {
  public:
    // Constructor de la clase
    Voraz(Tools& datos) : Algoritmo(datos) {};
    
    // Métodos de la clase
    vector<Vehiculo> ejecutar() override;
    pair<Zona&, double> zonaMasCercana(const Vehiculo& vehiculo); // Zona más cercana al vehículo
    pair<Zona&, double> swtsMasCercana(const Vehiculo& vehiculo); // SWTS más cercana a la zona
    int TiempoVolverDeposito(Vehiculo vehiculo); // Tiempo que tarda en volver al depósito pasando por las zonas y la swts más cercanas
};