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
  Zona(const string& id, const pair<int, int>& posicion, const int capacidad) : id_(id), posicion_(posicion), capacidad_(capacidad) {};

  private:
  string id_;
  pair<int, int> posicion_;
  int capacidad_;
};