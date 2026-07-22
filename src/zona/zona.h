/**
 * @class Clase para representar una zona para recoger la basura
 */
#ifndef C_Zona_H
#define C_Zona_H

#include <iostream>
#include <cmath>
#include <string>
#include <utility>

class Zona {
  public:
    // Constructor de la clase
    Zona() {}
    Zona(const std::string& id, const std::pair<int, int>& posicion, const double& contenido) : id_(id), posicion_(posicion), contenido_(contenido) {};
    Zona(const std::string& id, const std::pair<int, int>& posicion, const double& tiempoRecoleccion, const double& contenido) : id_(id), posicion_(posicion), tiempoDeProcesado_(tiempoRecoleccion), contenido_(contenido) {};

    // Getters
    inline const std::string& getId() const { return id_; }
    inline const std::pair<int, int>& getPosicion() const { return posicion_; }
    inline double getContenido() const { return contenido_; }
    inline double getDistancia(const Zona& zona) const {
      return std::sqrt(std::pow(posicion_.first - zona.getPosicion().first, 2) + std::pow(posicion_.second - zona.getPosicion().second, 2));
    }
    inline double getTiempoDeProcesado() const { return tiempoDeProcesado_; }

    // Setters
    inline void setContenido(double contenido) { contenido_ = contenido; }

    // Métodos de la clase
    inline bool esSWTS() const { return id_ == "IF" || id_ == "IF1"; }
    inline bool esDeposito() const { return id_ == "Depot"; }
    inline bool esDumpsite() const { return id_ == "Dumpsite"; }

    // Sobrecarga de operadores
    friend inline bool operator==(const Zona& zona1, const Zona& zona2) {
      return zona1.id_ == zona2.id_;
    }

  private:
    std::string id_;
    std::pair<int, int> posicion_;
    double contenido_;
    double tiempoDeProcesado_ = 0.0; // Tiempo que tarda en procesar la zona
};

#endif