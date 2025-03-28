/**
 * @file main.cc
 * @brief Función principal del programa
 */
#include "tools/tools.h"
#include "algoritmo/voraz/voraz.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Error: Número de argumentos incorrecto" << endl;
    cerr << "Uso: " << argv[0] << " <nombre_fichero>" << endl;
    exit(1);
  }
  string fileName = argv[1];
  try {
    Tools tools = readData(fileName);
    // Muestro las zonas
    cout << "Zonas:" << endl;
    //mostrarZonas();
    mostrarDistancias();

    Algoritmo* algoritmo = new Voraz(tools);
    vector<Vehiculo> vehiculos = algoritmo->ejecutar();

  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}