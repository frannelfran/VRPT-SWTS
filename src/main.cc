/**
 * @file main.cc
 * @brief Función principal del programa
 */
#include "tools/tools.h"
#include "algoritmo/voraz/voraz.h"
#include "algoritmo/grasp/grasp.h"
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
    Algoritmo* algoritmo = crearAlgoritmo(opcion);
    // Ejecutar el algoritmo para cada instancia
    for (auto& dato : datos) {
      algoritmo->setDato(dato);
      algoritmo->ejecutar();
    }
    // Muestro los resultados
    algoritmo->mostrarResultados();
    // Liberar memoria
    delete algoritmo;
    cout << "Fin del programa" << endl;
  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}