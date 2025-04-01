/**
 * @struct Estructura para almacenar los datos de un archivo de texto
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <sstream> // Para procesar las líneas
#include <cctype> // Para convertir a minúsculas
#include <stdexcept> // Para lanzar excepciones
#include <filesystem> // Para recorrer los ficheros del directorio
#include "../zona/zona.h"

namespace fs = filesystem;

using namespace std;

struct Tools {
  vector<Zona> zonas; // Vector con todas las zonas
  vector<Zona> zonasRecoleccion; // Vector con las zonas de recolección
  vector<vector<double>> distancias;
  int numZonas;
  int numVehiculos;
  // Información de los vehículos de recolección
  int capacidadRecoleccion;
  int duracionRecoleccion;
  // Información de los vehículos de transporte
  int capacidadTransporte;
  int duracionTransporte;
  // Información general
  int velocidad;
  int maxX;
  int maxY;
};

extern Tools tools;

vector<Tools> readData(const string& dirName); // Función para almacenar los datos de los ficheros
void procesarLinea(istringstream& linea); // Función para procesar una línea de texto
void crearZona(string id, istringstream& linea); // Función para crear una zona
vector<vector<double>> calcularDistancias(); // Función para calcular las distancias entre las zonas
void mostrarZonas(const vector<Zona>& zonas); // Función para mostrar las zonas
void mostrarDistancias(); // Función para mostrar las distancias
void mostrarMenu(); // Función para mostrar el menú de opciones