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
    //mostrarZonas(tools.zonas);
    //mostrarDistancias();

    double totalContenido = 0.0;
    for (const auto& zona : tools.zonasRecoleccion) {
      totalContenido += zona.getContenido();
    }

    Algoritmo* algoritmo = new Voraz(tools);
    vector<Vehiculo> vehiculos = algoritmo->ejecutar();
    // Muestro los vehículos
    cout << "Vehículos:" << endl;
    for (size_t i = 0; i < vehiculos.size(); i++) {
      cout << "Vehículo " << i + 1 << ": " << endl;
      cout << "Zonas visitadas: ";
      for (const auto& zona : vehiculos[i].getZonasVisitadas()) {
        cout << zona.getId() << " ";
      }
      cout << endl;
    }

    // Muestro el contenido de las zonas
    //mostrarZonas(tools.zonas);

    // Sumo el contenido de las zonas
  //double contenidoTotal = 0.0;
  //for (const auto& zona : tools.zonas) {
  //  if (!zona.esSWTS()) {
  //    contenidoTotal += zona.getContenido();
  //  }
  //}
  //cout << "Contenido total de las zonas: " << contenidoTotal << endl;

  //for (const auto& zona : tools.zonas) {
  //  if (zona.esSWTS()) {
  //    cout << "Zona " << zona.getId() << ": " << zona.getContenido() << endl;
  //  }
  //}

  } catch (const invalid_argument& e) {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}