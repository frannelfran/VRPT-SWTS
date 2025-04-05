/**
 * @class Clase para repressentar un algoritmo en el sistema
 */

#pragma once
#include "../tools/tools.h"

class Tools; // Declaración anticipada de la clase Tools

using namespace std;

class Algoritmo {
  public:
    // Constructor de la clase
    Algoritmo(Tools& datos) : datos_(datos) {};

    // Destructor de la clase
    ~Algoritmo() {}

    // Métodos de la clase
    virtual void ejecutar() = 0;

  protected:
    // Datos importantes para el algoritmo
    Tools& datos_;
};