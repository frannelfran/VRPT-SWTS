/**
 * @class Clase para repressentar un vehículo en el sistema
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../zona/zona.h"

using namespace std;

class Vehiculo {
  // Constructor de la clase
  public:
  Vehiculo(const int& capacidad, const int& velocidad, const pair<int, int>& posicion, const int& duracion, const vector<Zona>& zonas);

  // Métodos de la clase
  bool llenarVehiculo(double cantidad);
  void vaciarVehiculo(Zona& zona);
  void moverVehiculo(const Zona& zona);
  
  private:
  // Valores que dependen para que el vehículo siga funcionando
  int capacidad_;
  int duracion_;
  // Datos importantes para el vehículo
  double cantidad_ = 0.0;
  int velocidad_;
  pair<int, int> posicion_; // Posición actual del vehículo
  vector<Zona> zonasVisitadas_; // Zonas que ha visitado el vehículo
  vector<Zona> zonas_; // Conjunto de zonas que tiene que visitar el vehículo
};