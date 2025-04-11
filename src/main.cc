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
    mostrarMenu();
    int opcion;
    cin >> opcion;
    if (opcion < 1 || opcion > 3) {
      throw invalid_argument("Opción no válida");
    }
    // Ejecutar el algoritmo Voraz
    switch (opcion) {
      case 1: {
        Algoritmo* voraz = new Voraz();
        for (auto& dato : datos) {
          voraz->setDato(dato);
          auto start = chrono::high_resolution_clock::now();
          voraz->ejecutar();
          auto end = chrono::high_resolution_clock::now();
          dato.tiempoCPU = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        }
        voraz->mostrarResultados();
      }
      break;
    }
    
  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}