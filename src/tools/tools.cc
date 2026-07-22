#include "tools.h"
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::vector;
using std::pair;
using std::istringstream;
using std::ifstream;
using std::ofstream;
using std::invalid_argument;

/**
 * @brief Función para procesar una línea en tokens
 * @param linea Línea a procesar
 * @param datos Estructura donde se van almacenando los datos leídos
 * @return void
 */
void procesarLinea(istringstream& linea, Tools& datos) {
  string token;
  linea >> token;
  if (token == "L1") linea >> datos.duracionRecoleccion;
  else if (token == "L2") linea >> datos.duracionTransporte;
  else if (token == "num_vehicles") linea >> datos.numVehiculos;
  else if (token == "num_zones") linea >> datos.numZonas;
  else if (token == "Lx") linea >> datos.maxX;
  else if (token == "Ly") linea >> datos.maxY;
  else if (token == "Q1") linea >> datos.capacidadRecoleccion;
  else if (token == "Q2") linea >> datos.capacidadTransporte;
  else if (token == "V") linea >> datos.velocidad;
  else crearZona(token, linea, datos);
}

/**
 * @brief Función para crear una zona
 * @param id Identificador de la zona
 * @param linea Línea con los datos de la zona
 * @param datos Estructura donde se va a almacenar la zona creada
 * @return void
 */
void crearZona(const string& id, istringstream& linea, Tools& datos) {
  if (id == "epsilon" || id == "offset" || id == "k") return;
  pair<double, double> posicion;
  double contenido, tiempoProcesado;
  linea >> posicion.first >> posicion.second;
  if (id == "Dumpsite") {
    contenido = 0;
    datos.zonas.push_back(Zona(id, posicion, contenido));
    return;
  }
  else if ((posicion.first < 0 || posicion.second < 0)) {
    throw invalid_argument("Error: Las coordenadas de la zona " + id + " no pueden ser negativas");
  }
  else if (posicion.first > datos.maxX || posicion.second > datos.maxY) {
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
  datos.zonas.push_back(Zona(id, posicion, tiempoProcesado, contenido));
  // Si la zona es de recolección, la añado al vector de zonas de recolección
  if (id != "IF" && id != "IF1" && id != "Depot" && id != "Dumpsite") {
    datos.zonasRecoleccion.push_back(Zona(id, posicion, tiempoProcesado, contenido));
  }
}

/**
 * @brief Función para calcular las distancias entre las zonas
 * @param datos Estructura con las zonas ya cargadas
 * @return void
 */
vector<vector<double>> calcularDistancias(const Tools& datos) {
  vector<vector<double>> distancias(datos.zonas.size(), vector<double>(datos.zonas.size(), INFINITY));
  for (size_t i = 0; i < datos.zonas.size(); i++) {
    for (size_t j = 0; j < datos.zonas.size(); j++) {
      if (i == j) continue;
      else {
        distancias[i][j] = datos.zonas[i].getDistancia(datos.zonas[j]);
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
 * @param datos Estructura con las zonas y la matriz de distancias ya calculadas
 * @return void
 */
void mostrarDistancias(const Tools& datos) {
  for (size_t i = 0; i < datos.distancias.size(); i++) {
    cout << "Distancias de la zona " << datos.zonas[i].getId() << endl;
    for (size_t j = 0; j < datos.distancias[i].size(); j++) {
      cout << datos.zonas[i].getId() << " --> " << datos.zonas[j].getId() << ": " << datos.distancias[i][j] << endl;
    }
    cout << endl;
  }
}

/**
 * @brief Función para leer los datos de un único fichero de instancia
 * @param rutaFichero Ruta al fichero de instancia (p.ej. "data/instance5.txt")
 * @return Tools Datos de la instancia leída, con la matriz de distancias ya calculada
 * @throws std::invalid_argument si el fichero no se puede abrir o no contiene zonas
 */
Tools leerInstancia(const string& rutaFichero) {
  ifstream file(rutaFichero);
  if (!file.is_open()) {
    throw invalid_argument("Error: No se pudo abrir el fichero de instancia " + rutaFichero);
  }
  Tools dato;
  dato.nombreInstancia = fs::path(rutaFichero).stem().string();
  string line;
  while (getline(file, line)) {
    istringstream lineaStream(line);
    procesarLinea(lineaStream, dato);
  }
  if (dato.zonas.empty()) {
    throw invalid_argument("Error: El fichero de instancia " + rutaFichero + " no contiene ninguna zona");
  }
  dato.distancias = calcularDistancias(dato); // Calculo las distancias entre las zonas
  return dato;
}

namespace {

// Extrae el número de "instanceN.txt" (p.ej. 12 de "instance12"); -1 si el nombre no sigue ese patrón.
int numeroDeInstancia(const fs::path& ruta) {
  const string nombre = ruta.stem().string();
  const string prefijo = "instance";
  if (nombre.size() <= prefijo.size() || nombre.compare(0, prefijo.size(), prefijo) != 0) return -1;
  const string resto = nombre.substr(prefijo.size());
  if (resto.empty() || !std::all_of(resto.begin(), resto.end(), [](unsigned char c) { return std::isdigit(c); })) return -1;
  return std::stoi(resto);
}

} // namespace

/**
 * @brief Función para leer todos los ficheros de instancia ("instanceN.txt") de un directorio
 * @param dirName Ruta al directorio que contiene los ficheros de instancia
 * @return vector<Tools> Datos de cada instancia encontrada, ordenados por número de instancia
 * @throws std::invalid_argument si el directorio no existe o no contiene ningún fichero de instancia
 */
vector<Tools> readData(const string& dirName) {
  if (!fs::exists(dirName) || !fs::is_directory(dirName)) {
    throw invalid_argument("Error: El directorio de instancias " + dirName + " no existe");
  }
  vector<fs::path> ficheros;
  for (const auto& entrada : fs::directory_iterator(dirName)) {
    if (entrada.is_regular_file() && numeroDeInstancia(entrada.path()) >= 0) {
      ficheros.push_back(entrada.path());
    }
  }
  if (ficheros.empty()) {
    throw invalid_argument("Error: El directorio " + dirName + " no contiene ningún fichero \"instanceN.txt\"");
  }
  std::sort(ficheros.begin(), ficheros.end(), [](const fs::path& a, const fs::path& b) {
    return numeroDeInstancia(a) < numeroDeInstancia(b);
  });

  vector<Tools> datos;
  datos.reserve(ficheros.size());
  for (const auto& fichero : ficheros) {
    datos.push_back(leerInstancia(fichero.string()));
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
double Tools::calcularDistanciaRecoleccion() const {
  double distancia = 0.0;
  for (const auto& vehiculo : rutasRecoleccion) {
    const vector<Zona>& zonasVisitadas = vehiculo.getZonasVisitadas();
    for (auto it = zonasVisitadas.begin(); it != zonasVisitadas.end(); ++it) {
      if (it + 1 != zonasVisitadas.end()) {
        distancia += it->getDistancia(*next(it));
      }
    }
  }
  return distancia;
}

namespace {

// Escapa comillas y barras invertidas para incrustar cadenas en JSON
string escaparJSON(const string& texto) {
  string resultado;
  resultado.reserve(texto.size());
  for (char c : texto) {
    if (c == '"' || c == '\\') resultado += '\\';
    resultado += c;
  }
  return resultado;
}

void escribirZona(ofstream& salida, const Zona& zona) {
  string tipo = "recoleccion";
  if (zona.esDeposito()) tipo = "deposito";
  else if (zona.esSWTS()) tipo = "swts";
  else if (zona.esDumpsite()) tipo = "vertedero";
  salida << "{\"id\":\"" << escaparJSON(zona.getId()) << "\","
         << "\"x\":" << zona.getPosicion().first << ","
         << "\"y\":" << zona.getPosicion().second << ","
         << "\"contenido\":" << zona.getContenido() << ","
         << "\"tipo\":\"" << tipo << "\"}";
}

template <typename VehiculoT>
void escribirRuta(ofstream& salida, const VehiculoT& vehiculo) {
  salida << "{\"capacidad\":" << vehiculo.getCapacidad() << ","
         << "\"duracion\":" << vehiculo.getDuracion() << ","
         << "\"tiempoTranscurrido\":" << vehiculo.getTiempoTranscurrido() << ","
         << "\"zonas\":[";
  const auto& zonas = vehiculo.getZonasVisitadas();
  for (size_t i = 0; i < zonas.size(); i++) {
    if (i > 0) salida << ",";
    escribirZona(salida, zonas[i]);
  }
  salida << "]}";
}

} // namespace

/**
 * @brief Función para exportar las zonas y las rutas calculadas a un fichero JSON,
 *        consumido por la visualización web (visualizacion/index.html).
 * @param datos Vector con los datos (una entrada por instancia) que se quieren exportar
 * @param rutaSalida Ruta del fichero JSON de salida
 * @return void
 */
void exportarResultadosJSON(const vector<std::shared_ptr<Tools>>& datos, const string& rutaSalida) {
  ofstream salida(rutaSalida);
  if (!salida.is_open()) {
    throw std::runtime_error("Error: No se pudo crear el fichero de salida " + rutaSalida);
  }
  salida << "{\"instancias\":[";
  for (size_t d = 0; d < datos.size(); d++) {
    if (d > 0) salida << ",";
    const Tools& dato = *datos[d];
    salida << "{\"nombre\":\"" << escaparJSON(dato.nombreInstancia) << "\","
           << "\"maxX\":" << dato.maxX << ",\"maxY\":" << dato.maxY << ","
           << "\"tiempoCPU\":" << dato.tiempoCPU << ","
           << "\"zonas\":[";
    for (size_t i = 0; i < dato.zonas.size(); i++) {
      if (i > 0) salida << ",";
      escribirZona(salida, dato.zonas[i]);
    }
    salida << "],\"rutasRecoleccion\":[";
    for (size_t i = 0; i < dato.rutasRecoleccion.size(); i++) {
      if (i > 0) salida << ",";
      escribirRuta(salida, dato.rutasRecoleccion[i]);
    }
    salida << "],\"rutasTransporte\":[";
    for (size_t i = 0; i < dato.rutasTransporte.size(); i++) {
      if (i > 0) salida << ",";
      escribirRuta(salida, dato.rutasTransporte[i]);
    }
    salida << "]}";
  }
  salida << "]}";
}
