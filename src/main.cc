/**
 * @file main.cc
 * @brief Función principal del programa
 */
#include "tools/tools.h"
#include "algoritmo/vorazRecoleccion/vorazRecoleccion.h"
#include "algoritmo/vorazTransporte/vorazTransporte.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Error: Número de argumentos incorrecto" << endl;
    cerr << "Uso: " << argv[0] << " <ubicación_directorio>" << endl;
    exit(1);
  }
  string dirName = argv[1];
  try {
    vector<Tools> datos = readData(dirName);
    // Menú de opciones
    for (auto& dato : datos) {
      // Calculamos las rutas de los vehículos de recolección
      Algoritmo* vorazRecoleccion = new VorazRecoleccion(dato);
      Algoritmo* vorazTransporte = new VorazTransporte(dato);
      vector<Vehiculo> rutasRecoleccion = vorazRecoleccion->ejecutar();
      dato.rutasRecoleccion = rutasRecoleccion;
      vector<Vehiculo> rutasTransporte = vorazTransporte->ejecutar();
      dato.rutasTransporte = rutasTransporte;
    }
    mostrarResultados(datos);
  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}