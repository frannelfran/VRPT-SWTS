/**
 * @class Clase para repressentar un vehículo en el sistema
 */
#ifndef C_Vehiculo_H
#define C_Vehiculo_H

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
    void vaciarVehiculo(Zona& zona);
    void moverVehiculo(const Zona& zona, const double distancia);
    int calcularTiempo(const double distancia) const;
    void volverAlInicio();
    inline void agregarContenido(double contenido) { contenido_ += contenido; }

    // Getters
    inline Zona getPosicion() const { return posicion_; }
    inline vector<Zona>& getZonasVisitadas() { return zonasVisitadas_; }
    inline const vector<Zona>& getZonasVisitadas() const { return zonasVisitadas_; }
    inline int getDuracion() const { return duracion_; }
    inline int getContenido() const { return contenido_; }
    inline int getCapacidad() const { return capacidad_; }
    inline int getTiempoTranscurrido() const { return tiempoTotal_; }

    // Setters
    inline void setPosicion(const Zona& zona) { posicion_ = zona; }
    
  protected:
    // Valores que dependen para que el vehículo siga funcionando
    int capacidad_;
    int duracion_;
    int tiempoTotal_ = 0;
    // Datos importantes para el vehículo
    double contenido_ = 0.0;
    int velocidad_;
    Zona posicion_; // Zona actual del vehículo
    vector<Zona> zonasVisitadas_; // Zonas que ha visitado el vehículo
};

#endif