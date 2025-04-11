/**
 * @file main.cc
 * @brief Función principal del programa
 */
#include "tools/tools.h"
#include "algoritmo/voraz/voraz.h"
#include <chrono>

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
      auto inicio = chrono::high_resolution_clock::now();

      // Creamos los algoritmos una vez
      unique_ptr<Algoritmo> voraz = make_unique<Voraz>(dato);
      // Pasamos los datos y ejecutamos los algoritmos
      voraz->ejecutar();

      auto fin = chrono::high_resolution_clock::now();
      // Guardamos el tiempo de ejecución
      dato.tiempoCPU = round(chrono::duration_cast<chrono::duration<double>>(fin - inicio).count() * 10000) / 10000.0;
    }
    mostrarResultados(datos);
  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}