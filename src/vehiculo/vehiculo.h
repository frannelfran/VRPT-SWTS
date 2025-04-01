/**
 * @class Clase para repressentar un vehículo en el sistema
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../zona/zona.h"

using namespace std;

class Vehiculo {
  // Constructor de la clase
  public:
  Vehiculo(const int& capacidad, const int& velocidad, const Zona& posicion, const int& duracion);

  // Métodos de la clase
  bool llenarVehiculo(double cantidad);
  void vaciarZona(Zona& zona);
  void vaciarVehiculo(Zona& zona);
  void moverVehiculo(const Zona& zona, const double distancia);
  int calcularTiempo(const double distancia) const;
  void volverAlDeposito();

  // Getters
  inline Zona getPosicion() const { return posicion_; }
  const inline vector<Zona>& getZonasVisitadas() const { return zonasVisitadas_; }

  // Setters
  inline Vehiculo setPosicion(const Zona& zona) { posicion_ = zona; return *this; }
  inline int getDuracion() const { return duracion_; }
  inline int getContenido() const { return contenido_; }
  
  private:
  // Valores que dependen para que el vehículo siga funcionando
  int capacidad_;
  int duracion_;
  // Datos importantes para el vehículo
  double contenido_ = 0.0;
  int velocidad_;
  Zona posicion_; // Zona actual del vehículo
  vector<Zona> zonasVisitadas_; // Zonas que ha visitado el vehículo
};