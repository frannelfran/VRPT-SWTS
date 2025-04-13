#include "recoleccion.h"

/**
 * @brief Método para vaciar la zona
 * @param zona Zona que se va a vaciar
 * @return void
 */
void Recoleccion::vaciarZona(Zona& zona) {
  agregarContenido(zona.getContenido()); // Agregamos el contenido de la zona al vehículo
  zona.setContenido(0.0);
  this->tiempoTotal_ += zona.getTiempoDeProcesado();
}