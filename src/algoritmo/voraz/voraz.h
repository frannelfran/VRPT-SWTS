/**
 * @class Clase para representar un algoritmo Voraz en el sistema
 */

#pragma once
#include "../algoritmo.h"

class Voraz : public Algoritmo {
  public:
    Voraz(Tools& datos) : Algoritmo(datos) {} // Constructor de la clase

    // Métodos de la clase
    void ejecutar() override; // Método para ejecutar el algoritmo Voraz
    void calcularRutasRecoleccion(); // Método para calcular las rutas de recolección
    void calcularRutasTransporte(); // Método para calcular las rutas de transporte

  private:
    // Métodos para calcular el voraz de las rutas de recolección
    pair<Zona&, double> zonaMasCercana(const Recoleccion& vehiculo); // Zona más cercana al vehículo
    pair<Zona&, double> swtsMasCercana(const Recoleccion& vehiculo); // SWTS más cercana a la zona
    int TiempoVolverDeposito(Recoleccion vehiculo); // Tiempo que tarda en volver al depósito pasando por las zonas y la swts más cercanas

};
