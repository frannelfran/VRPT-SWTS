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
Vehiculo::Vehiculo(const int& capacidad, const int& velocidad, const pair<int, int>& posicion, const int& duracion) {
  capacidad_ = capacidad;
  velocidad_ = velocidad;
  posicion_ = posicion;
  duracion_ = duracion;
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
 * @param distancia Distancia que hay entre la posición actual del vehículo y la zona
 * @return void
 */
void Vehiculo::moverVehiculo(const Zona& zona, const double distancia) {
  // En base a la posicion actual del vehículo, la velocidad y la distancia a la que se encuentra la zona 
  // se calcula el tiempo que tarda en llegar a la zona
  int tiempo = distancia / velocidad_;
  duracion_ -= tiempo;
  posicion_ = zona.getPosicion();
  zonasVisitadas_.push_back(zona);
}

/**
 * @brief Método para calcular el tiempo que tarda en llegar a una zona
 * @param zona Zona a la que se va a mover el vehículo
 * @return Tiempo que tarda el vehículo en llegar a la zona
 */
int Vehiculo::calcularTiempo(const Zona& zona) {
  return sqrt(pow(posicion_.first - zona.getPosicion().first, 2) + pow(posicion_.second - zona.getPosicion().second, 2)) / velocidad_;
}