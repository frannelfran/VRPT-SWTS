/**
 * @class Clase para representar una zona para recoger la basura
 */

#pragma once
#include <iostream>
#include <string>

using namespace std;

class Zona {
  public:
  // Constructor de la clase
  Zona() {}
  Zona(const string& id, const pair<int, int>& posicion, const double& contenido) : id_(id), posicion_(posicion), contenido_(contenido) {};

  // Getters
  inline string getId() const { return id_; }
  inline pair<int, int> getPosicion() const { return posicion_; }
  inline double getContenido() const { return contenido_; }

  private:
  string id_;
  pair<int, int> posicion_;
  double contenido_;
};