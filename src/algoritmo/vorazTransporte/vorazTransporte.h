/**
 * @class Clase para representar un algoritmo VorazTransporte en el sistema
 */

#pragma once
#include "../../tools/tools.h"

class Transporte;

struct Tarea {
  double Dh; // Cantidad de residuos tranposrtados por el vehículo derecolección
  Zona Sh; // SWTS visitada por el vehículo de recolección
  int Th; // Tiempo en el que el vehículo de recolección llega a la SWTS
};

class VorazTransporte : public Algoritmo {
  public:
    // Constructor de la clase
    VorazTransporte(Tools& datos) : Algoritmo(datos) {}

    // Destructor de la clase
    ~VorazTransporte() {}

    // Método para ejecutar el algoritmo VorazTransporte
    void ejecutar() override;

    // Métodos de la clase
  private:
    vector<Tarea> crearConjuntoTareas(const vector<Recoleccion>& vehiculos);
    vector<Tarea> ordenarTareas(const vector<Tarea>& tareas);
    double buscarCantidadMinima(const vector<Tarea>& tareas); // Método para obtener la cantidad mínima de residuos entre todas las tareas
    double calcularCostoInsercion(const Tarea& tarea, Transporte& vehiculo); // Método para calcular el costo de inserción de una tarea en un vehículo
    int tiempoVolverAlVertedero(const Transporte& vehiculo);
    Transporte* escogerVehiculo(vector<Transporte>& vehiculos, const Tarea& tarea); // Escoger el vehículo que mínimice el costo de inserción
};