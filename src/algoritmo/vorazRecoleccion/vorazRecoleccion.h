/**
 * @class Algortimo Voraz para determinar la ruta de los vehículos de recolección
 */

#pragma once
#include "../algoritmo.h"
#include "../../vehiculo/recoleccion/recoleccion.h"

class VorazRecoleccion : public Algoritmo {
  public:
    // Constructor de la clase
    VorazRecoleccion(Tools& datos) : Algoritmo(datos) {};
    
    // Métodos de la clase
    vector<Vehiculo> ejecutar() override;
    pair<Zona&, double> zonaMasCercana(const Recoleccion& vehiculo); // Zona más cercana al vehículo
    pair<Zona&, double> swtsMasCercana(const Recoleccion& vehiculo); // SWTS más cercana a la zona
    int TiempoVolverDeposito(Recoleccion vehiculo); // Tiempo que tarda en volver al depósito pasando por las zonas y la swts más cercanas
};