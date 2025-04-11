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
    Algoritmo() : dato_(nullptr) {}
    Algoritmo(Tools& dato) : dato_(&dato) {}

    // Destructor de la clase
    ~Algoritmo() {}

    // Métodos de la clase
    virtual void ejecutar() = 0;
    virtual void mostrarResultados() = 0;

    // Setters
    inline void setDato(Tools& dato) { dato_ = &dato; }

  protected:
    // Datos importantes para el algoritmo
    Tools* dato_;
    vector<Tools*> datos_;
};