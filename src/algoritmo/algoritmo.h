/**
 * @class Clase para repressentar un algoritmo en el sistema
 */

#pragma once
#include "../vehiculo/vehiculo.h"
#include "../zona/zona.h"
#include "../tools/tools.h"
#include <vector>

class Tools; // Declaración anticipada de la clase Tools

using namespace std;

class Algoritmo {
  public:
    // Constructor de la clase
    Algoritmo(Tools& datos) : datos_(datos) {};

    // Destructor de la clase
    ~Algoritmo() {}

    // Métodos de la clase
    virtual vector<Vehiculo> ejecutar() = 0;

  protected:
    // Datos importantes para el algoritmo
    Tools& datos_;
};