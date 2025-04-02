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
Vehiculo::Vehiculo(const int& capacidad, const int& velocidad, const Zona& posicion, const int& duracion) {
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
  return (contenido_ + cantidad <= capacidad_);
}

/**
 * @brief Método para vaciar la zona
 * @param zona Zona que se va a vaciar
 * @return void
 */
void Vehiculo::vaciarZona(Zona& zona) {
  this->contenido_ += zona.getContenido();
  zona.setContenido(0.0);
  this->duracion_ -= zona.getTiempoDeProcesado();
}

/**
 * @brief Método para vaciar el vehículo
 * @param zona Zona en la que se va a vaciar el vehículo
 * @return void
 */
void Vehiculo::vaciarVehiculo(Zona& zona) {
  double contenidoTotal = zona.getContenido() + contenido_;
  zona.setContenido(contenidoTotal);
  this->contenido_ = 0.0;
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
  int tiempo = calcularTiempo(distancia);
  duracion_ -= tiempo;
  zonasVisitadas_.push_back(posicion_);
  posicion_ = zona;
}

/**
 * @brief Método para calcular el tiempo que tarda en recorrer una distancia
 * @param distancia Distancia que hay entre la posición actual del vehículo y la zona
 * @return Tiempo que tarda el vehículo en llegar a la zona
 */
int Vehiculo::calcularTiempo(const double distancia) const {
  double tiempo = distancia / velocidad_;
  // Paso el tiempo a minutos
  return int(tiempo * 60);
}

/**
 * @brief Método para volver al depósito
 * @return void
 */
void Vehiculo::volverAlDeposito() {
  Zona deposito = zonasVisitadas_[0];
  double distancia = posicion_.getDistancia(deposito);
  int tiempo = calcularTiempo(distancia);
  zonasVisitadas_.push_back(posicion_);
  zonasVisitadas_.push_back(deposito);
  posicion_ = deposito;
  contenido_ = 0.0;
}