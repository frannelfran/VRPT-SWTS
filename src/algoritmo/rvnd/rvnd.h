/**
 * @class Clase para representar el algoritmo RVND
 */

#ifndef C_RVND_H
#define C_RVND_H

#include "../algoritmo.h"
#include "../grasp/grasp.h"

class RVND : public Algoritmo {
  public:
    // Constructor
    RVND() : Algoritmo() {}
    RVND(const int& mejoresZonas, const int& numeroEjecuciones) : mejoresZonasCercanas_(mejoresZonas), numeroEjecuciones_(numeroEjecuciones) {}

    
    // Métodos de la clase
    void ejecutar() override;
    void mostrarResultados() override;
    Tools* mejorRuta();

  private:
    int mejoresZonasCercanas_;
    int numeroEjecuciones_;
    vector<Tools*> busquedasLocales_;
    vector<double> distancias_;
};

#endif