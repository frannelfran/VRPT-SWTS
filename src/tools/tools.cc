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
  else if (token == "L2") linea >> tools.duracionTransporte;
  else if (token == "num_vehicles") linea >> tools.numVehiculos;
  else if (token == "num_zones") linea >> tools.numZonas;
  else if (token == "Lx") linea >> tools.maxX;
  else if (token == "Ly") linea >> tools.maxY;
  else if (token == "Q1") linea >> tools.capacidadRecoleccion;
  else if (token == "Q2") linea >> tools.capacidadTransporte;
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
  if (id == "epsilon" || id == "offset" || id == "k") return;
  pair<double, double> posicion;
  double contenido, tiempoProcesado;
  linea >> posicion.first >> posicion.second;
  if (id == "Dumpsite") {
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
  for (int i = 1; i <= 20; i++) {
    string fileName = dirName + "instance" + to_string(i) + ".txt";
    if (fileName != dirName + "instance1.txt" && fileName != dirName + "instance2.txt") {
      continue; // Solo leo el primer fichero
    }
    ifstream file(fileName);
    tools.nombreInstancia = "instance" + to_string(i);
    string line;
    while (getline(file, line)) {
      istringstream lineaStream(line);
      procesarLinea(lineaStream);
    }
    tools.distancias = calcularDistancias(); // Calculo las distancias entre las zonas
    datos.push_back(tools);
    tools.zonas.clear();
  }
  return datos;
}

/**
 * @brief Función que muestra el menú del programa
 * @return void
 */
void mostrarMenu() {
  cout << "Seleccione una opción:" << endl;
  cout << "1. Algoritmo Voraz" << endl;
  cout << "2. Algoritmo GRASP" << endl;
  cout << "3. Algoritmo RVND" << endl;
  cout << "4. Salir" << endl;
  cout << "Introduce una opción: ";
}

/**
 * @brief Función para crear el algoritmo según la opción elegida
 * @param opcion Opción elegida
 * @return Algoritmo* Puntero al algoritmo creado
 */
Algoritmo* crearAlgoritmo(int opcion) {
  Algoritmo* algoritmo = nullptr;
  switch (opcion) {
    case 1:
      algoritmo = new Voraz();
      break;
    case 2:
      algoritmo = new Grasp(3, 3);
      break;
    case 3:
      algoritmo = new RVND(3, 3);
      break;
    default:
      throw invalid_argument("Opción no válida");
  }
  return algoritmo;
}

/**
 * @brief Función para calcular la distancia de recolección
 * @return double Distancia de recolección
 */
double Tools::calcularDistanciaRecoleccion() {
  double distancia = 0.0;
  for (const auto& vehiculo : rutasRecoleccion) {
    for (auto it = vehiculo.getZonasVisitadas().begin(); it != vehiculo.getZonasVisitadas().end(); ++it) {
      Zona zona = *it;
      if (it + 1 != vehiculo.getZonasVisitadas().end()) {
        distancia += zona.getDistancia(*next(it));
      }
    }
  }
  return distancia;
}