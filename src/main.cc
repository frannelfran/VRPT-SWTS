/**
 * @file main.cc
 * @brief Función principal del programa
 */
#include "tools/tools.h"
#include "algoritmo/voraz/voraz.h"

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
      Algoritmo* voraz = new Voraz(dato);
      vector<Vehiculo> rutasRecoleccion = voraz->ejecutar();
      dato.rutasRecoleccion = rutasRecoleccion;
    }
    mostrarResultados(datos);
  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}