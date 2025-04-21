/**
 * @class Clase que representa un algoritmo GRASP en el sistema
 */
#ifndef C_Grasp_H
#define C_Grasp_H

#include "../algoritmo.h"
#include "../../vehiculo/recoleccion/recoleccion.h"
#include "../voraz/voraz.h"
#include "busqueda_local/busqueda_local.h"
#include <random>

class Grasp : public Algoritmo {
  public:
    // Constructores de la clase
    Grasp(const int& mejoresZonas, const int& numeroEjecuciones) : Algoritmo(), numeroMejoresZonasCercanas_(mejoresZonas), numeroEjecuciones_(numeroEjecuciones) {}
    Grasp(Tools& dato, const int& mejoresZonas, const int& numeroEjecuciones) : Algoritmo(dato), numeroMejoresZonasCercanas_(mejoresZonas), numeroEjecuciones_(numeroEjecuciones) {}

    // Métodos de la clase
    void ejecutar() override;
    void mostrarResultados() override;
    void calcularRutasRecoleccion(const int mejorezZonas, const int ejecucion); // Método para calcular las rutas de recolección
    void mostrarDistancias();

    // Getters
    vector<Tools*> getDato(const Tools& dato);

  private:
    // Métodos para calcular el GRASP de las rutas de recolección
    pair<Zona&, double> zonaMasCercana(const Recoleccion& vehiculo, const int numeroMejoresZonas); // Zona más cercana al vehículo
    pair<Zona&, double> swtsMasCercana(const Recoleccion& vehiculo); // SWTS más cercana a la zona
    int TiempoVolverDeposito(Recoleccion vehiculo, const int numeroMejoresZonas); // Tiempo que tarda en volver al depósito pasando por las zonas y la swts más cercanas
    // Atributos para cuántas zonas cercanas se van a considerar y el número de ejecuciones
    int numeroMejoresZonasCercanas_;
    int numeroEjecuciones_;
    vector<double> distanciaSinMejoras;
    vector<double> distanciasConMejoras_;
    vector<pair<int, int>> mejoresZonasYEjecuciones_;
};

#endif