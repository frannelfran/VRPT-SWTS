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
  inline double getDistancia(const Zona& zona) const {
    return sqrt(pow(posicion_.first - zona.getPosicion().first, 2) + pow(posicion_.second - zona.getPosicion().second, 2));
  }

  // Setters
  inline void setContenido(double contenido) { contenido_ = contenido; }

  // Sobrecarga de operadores
  friend bool operator==(const Zona& zona1, const Zona& zona2) {
    return zona1.id_ == zona2.id_;
  }

  private:
  string id_;
  pair<int, int> posicion_;
  double contenido_;
  bool visitada_ = false; // Indica si la zona ha sido visitada
};