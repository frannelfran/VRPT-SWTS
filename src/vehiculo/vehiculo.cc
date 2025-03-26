#include "vehiculo.h"

/**
 * @brief Constructor de la clase
 * @param capacidad Capacidad del vehículo
 * @param velocidad Velocidad del vehículo
 * @param posicion Posición inicial del vehículo
 * @param duracion Duración máxima que puede estar el vehiculo en funcionamiento
 * @param zonas Zonas que tiene que visitar el vehículo
 * @return Vehiculo
 */
Vehiculo::Vehiculo(const int& capacidad, const int& velocidad, const pair<int, int>& posicion, const int& duracion, const vector<Zona>& zonas) {
  capacidad_ = capacidad;
  velocidad_ = velocidad;
  posicion_ = posicion;
  duracion_ = duracion;
  zonas_ = zonas;
}

/**
 * @brief Método para llenar el vehículo
 * @param cantidad Cantidad de residuos que se van a añadir al vehículo
 * @return bool
 */
bool Vehiculo::llenarVehiculo(double cantidad) {
  if (cantidad_ + cantidad > capacidad_) return false;
  else {
    cantidad_ += cantidad;
  }
  return true;
}

/**
 * @brief Método para vaciar el vehículo
 * @param zona Zona en la que se va a vaciar el vehículo
 * @return void
 */
void Vehiculo::vaciarVehiculo(Zona& zona) {
  zona.setContenido(this->cantidad_);
  this->cantidad_ = 0.0;
}

/**
 * @brief Método para mover el vehículo a una zona
 * @param zona Zona a la que se va a mover el vehículo
 * @return void
 */
void Vehiculo::moverVehiculo(const Zona& zona) {
  posicion_ = zona.getPosicion();
  zonasVisitadas_.push_back(zona);
}

/**
 * @brief Método para obtener la zona más cercana al vehículo que no haya sido visitada aún.
 *        Para ello se calculara la distancia euclídea entre el vehículo y las zonas que no han sido visitadas.
 * @return Zona más cercana al vehículo
 */
Zona Vehiculo::zonaMasCercana() {
  double distancia = 0.0;
  double minDistancia = sqrt(pow(zonas_[0].getPosicion().first - posicion_.first, 2) + pow(zonas_[0].getPosicion().second - posicion_.second, 2));
  Zona zonaMasCercana = zonas_[0];
  for (size_t i = 1; i < zonas_.size(); i++) {
    distancia = sqrt(pow(zonas_[i].getPosicion().first - posicion_.first, 2) + pow(zonas_[i].getPosicion().second - posicion_.second, 2));
    if (distancia < minDistancia && find(zonasVisitadas_.begin(), zonasVisitadas_.end(), zonas_[i]) == zonasVisitadas_.end()) {
      minDistancia = distancia;
      zonaMasCercana = zonas_[i];
    }
  }
  return zonaMasCercana;
}