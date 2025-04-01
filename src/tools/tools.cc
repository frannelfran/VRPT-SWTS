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
  double contenido, tiempoProcesado;
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
    contenido = 0.0;
    tiempoProcesado = 0.0;
  } else {
    linea >> tiempoProcesado >> contenido;
    if (contenido < 0) {
      throw invalid_argument("Error: El contenido de la zona " + id + " no puede ser negativo");
    }
    if (tiempoProcesado < 0) {
      throw invalid_argument("Error: El tiempo de procesado de la zona " + id + " no puede ser negativo");
    }
  }
  tools.zonas.push_back(Zona(id, posicion, tiempoProcesado, contenido));
  // Si la zona es de recolección, la añado al vector de zonas de recolección
  if (id != "IF" && id != "IF1" && id != "Depot" && id != "Dumpsite") {
    tools.zonasRecoleccion.push_back(Zona(id, posicion, tiempoProcesado, contenido));
  }
}

/**
 * @brief Función para calcular las distancias entre las zonas
 * @return void
 */
vector<vector<double>> calcularDistancias() {
  vector<vector<double>> distancias(tools.zonas.size(), vector<double>(tools.zonas.size(), INFINITY));
  for (size_t i = 0; i < tools.zonas.size(); i++) {
    for (size_t j = 0; j < tools.zonas.size(); j++) {
      if (i == j) continue;
      else {
        distancias[i][j] = tools.zonas[i].getDistancia(tools.zonas[j]);
      }
    }
  }
  return distancias;
}

/**
 * @brief Función para mostrar las zonas
 * @param zonas Vector con las zonas a mostrar
 * @return void
 */
void mostrarZonas(const vector<Zona>& zonas) {
  for (const auto& zona : zonas) {
    cout << "Zona: " << zona.getId() << endl;
    cout << "Posición: (" << zona.getPosicion().first << ", " << zona.getPosicion().second << ")" << endl;
    cout << "Contenido: " << zona.getContenido() << endl;
    cout << "Tiempo de procesado: " << zona.getTiempoDeProcesado() << endl;
    cout << "----------------------------------------" << endl;
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
 * @return vector<Tools> Vector con los datos de los ficheros
 */
vector<Tools> readData(const string& dirName) {
  vector<Tools> datos;
  // Recorro los ficheros del directorio
  for (const auto& nombre : fs::directory_iterator(dirName)) {
    if (nombre.path().extension() == ".txt") {
      string fileName = nombre.path().string();
      ifstream file(fileName);
      string line;
      while (getline(file, line)) {
        istringstream lineaStream(line);
        procesarLinea(lineaStream);
      }
      file.close();
      tools.distancias = calcularDistancias(); // Calculo las distancias entre las zonas
      datos.push_back(tools);
      file.close();

    }
    else {
      throw invalid_argument("Error: El fichero " + nombre.path().string() + " no es un fichero de texto");
    }
  }
  return datos;
}


//ifstream file(fileName);
//  string line;
//  while (getline(file, line)) {
//    istringstream lineaStream(line);
//    procesarLinea(lineaStream);
//  }
//  file.close();
//  tools.distancias = calcularDistancias(); // Calculo las distancias entre las zonas
//  return tools;