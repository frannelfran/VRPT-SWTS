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
  Zona(const string& id, const pair<double, double>& posicion, const double& contenido) : id_(id), posicion_(posicion), contenido_(contenido) {};

  // Getters
  inline string getId() const { return id_; }
  inline pair<double, double> getPosicion() const { return posicion_; }
  inline double getContenido() const { return contenido_; }

  private:
  string id_;
  pair<double, double> posicion_;
  double contenido_;
};