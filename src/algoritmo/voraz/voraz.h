/**
 * @class Clase para representar un algoritmo Voraz en el sistema
 */

#ifndef C_Voraz_H
#define C_Voraz_H

#include "../algoritmo.h"
#include "../../vehiculo/recoleccion/recoleccion.h"
#include "../../vehiculo/transporte/transporte.h"

class Voraz : public Algoritmo {
  public:
    Voraz() : Algoritmo() {} // Constructor por defecto
    Voraz(Tools& datos) : Algoritmo(datos) {} // Constructor de la clase

    // Métodos de la clase
    void ejecutar() override; // Método para ejecutar el algoritmo Voraz
    void mostrarResultados() override; // Método para mostrar los resultados del algoritmo Voraz
    void calcularRutasRecoleccion(); // Método para calcular las rutas de recolección
    void calcularRutasTransporte(); // Método para calcular las rutas de transporte

  private:
    // Métodos para calcular el voraz de las rutas de recolección
    pair<Zona&, double> zonaMasCercana(const Recoleccion& vehiculo); // Zona más cercana al vehículo
    pair<Zona&, double> swtsMasCercana(const Recoleccion& vehiculo); // SWTS más cercana a la zona
    int TiempoVolverDeposito(Recoleccion vehiculo); // Tiempo que tarda en volver al depósito pasando por las zonas y la swts más cercanas

    // Métodos para calcular el voraz de las rutas de transporte
    vector<Tarea> crearConjuntoTareas(const vector<Recoleccion>& vehiculos);
    vector<Tarea> ordenarTareas(const vector<Tarea>& tareas);
    double buscarCantidadMinima(const vector<Tarea>& tareas); // Método para obtener la cantidad mínima de residuos entre todas las tareas
    double calcularCostoInsercion(const Tarea& tarea, Transporte& vehiculo); // Método para calcular el costo de inserción de una tarea en un vehículo
    int tiempoVolverAlVertedero(const Transporte& vehiculo);
    Transporte* escogerVehiculo(vector<Transporte>& vehiculos, const Tarea& tarea); // Escoger el vehículo que mínimice el costo de inserción
};

#endif