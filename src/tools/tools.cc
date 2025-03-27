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
  if (id == "epsilon" || id == "offset" || id == "k") return;
  else if (id == "Dumpsite") {
    contenido = 0;
    tools.zonas.push_back(Zona(id, posicion, contenido));
    return;
  }
  else if ((posicion.first < 0 || posicion.second < 0)) {
    throw invalid_argument("Error: Las coordenadas de la zona " + id + " no pueden ser negativas");
  } 
  else if (posicion.first > tools.maxX || posicion.second > tools.maxY) {
    throw invalid_argument("Error: Las coordenadas de la zona " + id + " están fuera del rango");
  }
  else if (id == "IF" || id == "IF1" || id == "Depot") {
    contenido = 0;
  } else {
    double D1, D2;
    linea >> D1 >> D2;
    contenido = D2 - D1;
  }
  tools.zonas.push_back(Zona(id, posicion, contenido));
}

/**
 * @brief Función para calcular las distancias entre las zonas
 * @return void
 */
vector<vector<double>> calcularDistancias() {
  vector<vector<double>> distancias(tools.zonas.size(), vector<double>(tools.zonas.size(), 0.0));
  for (size_t i = 0; i < tools.zonas.size(); i++) {
    for (size_t j = 0; j < tools.zonas.size(); j++) {
      distancias[i][j] = sqrt(pow(tools.zonas[i].getPosicion().first - tools.zonas[j].getPosicion().first, 2) + pow(tools.zonas[i].getPosicion().second - tools.zonas[j].getPosicion().second, 2));
    }
  }
  return distancias;
}

/**
 * @brief Función para mostrar las zonas
 * @return void
 */
void mostrarZonas() {
  for (size_t i = 0; i < tools.zonas.size(); i++) {
    cout << "Zona " << tools.zonas[i].getId() << ": " << tools.zonas[i].getPosicion().first << " " << tools.zonas[i].getPosicion().second << " " << tools.zonas[i].getContenido() << endl;
  }
}

/**
 * @brief Función para mostrar las distancias
 * @return void
 */
void mostrarDistancias() {
  for (size_t i = 0; i < tools.distancias.size(); i++) {
    cout << "Distancias de la zona " << tools.zonas[i].getId() << endl;
    for (size_t j = 0; j < tools.distancias[i].size(); j++) {
      cout << tools.zonas[i].getId() << " --> " << tools.zonas[j].getId() << ": " << tools.distancias[i][j] << endl;
    }
    cout << endl;
  }
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
  tools.distancias = calcularDistancias(); // Calculo las distancias entre las zonas
  return tools;
}