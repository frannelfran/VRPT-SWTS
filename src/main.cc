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
    cout << "Seleccione una opción:" << endl;
    cout << "1. Algortimo Voraz" << endl;
    cout << "Introduce una opción: ";
    

    

  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}