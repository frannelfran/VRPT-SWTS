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
    std::shared_ptr<Tools> mejorRuta();

  private:
    int mejoresZonasCercanas_;
    int numeroEjecuciones_;
    std::vector<std::shared_ptr<Tools>> busquedasLocales_;
    std::vector<double> distancias_;
};

#endif