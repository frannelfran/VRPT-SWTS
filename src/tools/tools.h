/**
 * @struct Estructura para almacenar los datos de un archivo de texto
 */

#ifndef C_Tools_H
#define C_Tools_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <sstream> // Para procesar las líneas
#include <cctype> // Para convertir a minúsculas
#include <stdexcept> // Para lanzar excepciones
#include <filesystem> // Para recorrer los ficheros del directorio
#include <memory>
#include "../zona/zona.h"
#include "../vehiculo/recoleccion/recoleccion.h"
#include "../vehiculo/transporte/transporte.h"
#include "../algoritmo/voraz/voraz.h"
#include "../algoritmo/grasp/grasp.h"
#include "../algoritmo/rvnd/rvnd.h"

class Algoritmo;

namespace fs = std::filesystem;

struct Tools {
  std::vector<Zona> zonas; // Vector con todas las zonas
  std::vector<Zona> zonasRecoleccion; // Vector con las zonas de recolección
  std::vector<Recoleccion> rutasRecoleccion; // Vector con las rutas de los vehículos de recolección
  std::vector<Transporte> rutasTransporte; // Vector con las rutas de los vehículos de transporte
  std::vector<std::vector<double>> distancias;
  int numZonas = 0;
  int numVehiculos = 0;
  // Información de los vehículos de recolección
  int capacidadRecoleccion = 0;
  int duracionRecoleccion = 0;
  // Información de los vehículos de transporte
  int capacidadTransporte = 0;
  int duracionTransporte = 0;
  // Información general
  int velocidad = 0;
  int maxX = 0;
  int maxY = 0;
  std::string nombreInstancia;
  double tiempoCPU = 0.0; // Tiempo que tarda en resolver el problema

  double calcularDistanciaRecoleccion() const;
};

std::vector<Tools> readData(const std::string& dirName); // Lee todos los ficheros "instanceN.txt" de un directorio
Tools leerInstancia(const std::string& rutaFichero); // Lee los datos de un único fichero de instancia
void procesarLinea(std::istringstream& linea, Tools& datos); // Función para procesar una línea de texto
void crearZona(const std::string& id, std::istringstream& linea, Tools& datos); // Función para crear una zona
std::vector<std::vector<double>> calcularDistancias(const Tools& datos); // Función para calcular las distancias entre las zonas
void mostrarZonas(const std::vector<Zona>& zonas); // Función para mostrar las zonas
void mostrarDistancias(const Tools& datos); // Función para mostrar las distancias
void mostrarMenu(); // Función para mostrar el menú de opciones
Algoritmo* crearAlgoritmo(int opcion); // Función para crear el algoritmo según la opción elegida
void exportarResultadosJSON(const std::vector<std::shared_ptr<Tools>>& datos, const std::string& rutaSalida); // Exporta zonas y rutas a JSON para la visualización web

#endif
