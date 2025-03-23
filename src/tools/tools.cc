#include "tools.h"

Tools tools;

/**
 * @brief Función para procesar una línea en tokens
 * @param linea Línea a procesar
 * @return void
 */
void procesarLinea(istringstream& linea) {
  string token;
  linea >> token;
  if (token == "L1") linea >> tools.duracionRecoleccion;
  else if (token == "L2") linea >> tools.capacidadTransporte;
  else if (token == "num_vehicles") linea >> tools.numVehiculos;
  else if (token == "num_zones") linea >> tools.numZonas;
  else if (token == "Lx") linea >> tools.maxX;
  else if (token == "Ly") linea >> tools.maxY;
  else if (token == "Q1") linea >> tools.capacidadRecoleccion;
  else if (token == "Q2") linea >> tools.duracionTransporte;
  else if (token == "V") linea >> tools.velocidad;
  else crearZona(token, linea);
}

/**
 * @brief Función para crear una zona
 * @param id Identificador de la zona
 * @param linea Línea con los datos de la zona
 * @return void
 */
void crearZona(string id, istringstream& linea) {
  pair<double, double> posicion;
  double contenido;
  linea >> posicion.first >> posicion.second;
//if (posicion.first > tools.maxX || posicion.second > tools.maxY) {
//  throw invalid_argument("Error: Las coordenadas de la zona " + id + " están fuera del rango");
//}
  if (id == "epsilon" || id == "offset" || id == "k") return;
  else if (id == "Depot" || id == "IF" || id == "IF1" || id == "Dumpsite") {
    contenido = 0;
  } else {
    double D1, D2;
    linea >> D1 >> D2;
    contenido = D2 - D1;
  }
  tools.zonas.push_back(Zona(id, posicion, contenido));
}

/**
 * @brief Función para leer los datos del fichero
 * @param tools Estructura con los datos del fichero
 * @return Tools
 */
Tools readData(const string& fileName) {
  ifstream file(fileName);
  string line;
  while (getline(file, line)) {
    istringstream lineaStream(line);
    procesarLinea(lineaStream);
  }
  return tools;
}