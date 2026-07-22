/**
 * @file main.cc
 * @brief Función principal del programa
 */
#include "tools/tools.h"
#include "algoritmo/voraz/voraz.h"
#include "algoritmo/grasp/grasp.h"
#include "algoritmo/rvnd/rvnd.h"
#include <chrono>
#include <memory>

using namespace std;

namespace {
constexpr int kOpcionSalir = 4;
const char* kRutaExportacionJSON = "resultados.json";
} // namespace

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Error: Número de argumentos incorrecto" << endl;
    cerr << "Uso: " << argv[0] << " <directorio_de_instancias>" << endl;
    exit(1);
  }
  string dirName = argv[1];
  try {
    vector<Tools> datos = readData(dirName);
    // Menú de opciones
    mostrarMenu();
    int opcion;
    cin >> opcion;
    if (opcion == kOpcionSalir) {
      cout << "Fin del programa" << endl;
      return 0;
    }
    unique_ptr<Algoritmo> algoritmo(crearAlgoritmo(opcion));
    // Ejecutar el algoritmo para cada instancia del directorio
    for (auto& dato : datos) {
      algoritmo->setDato(dato);
      algoritmo->ejecutar();
    }
    // Muestro los resultados
    algoritmo->mostrarResultados();
    // Exporto los resultados a JSON para la visualización web (ver visualizacion/index.html)
    exportarResultadosJSON(algoritmo->getDatos(), kRutaExportacionJSON);
    cout << "Resultados exportados a " << kRutaExportacionJSON << endl;
    cout << "Fin del programa" << endl;
  } catch (const exception& e) {
    cerr << e.what() << endl;
    exit(1);
  }

  return 0;
}