#include "voraz.h"

/**
 * @brief Método para construir las rutas de los vehículos de recolección
 * @return vector<Vehiculo> Rutas de los vehículos de recolección
 */
vector<Vehiculo> Voraz::ejecutar() {
  vector<Vehiculo> rutasDeVehiculos;
  vector<Zona> zonas = datos_.zonas;
  while (zonas.size() != 0) {
    // Creamos el vehículo
    Vehiculo vehiculo(datos_.capacidadRecoleccion, datos_.velocidad, zonas[0], datos_.duracionRecoleccion);
    do {
      pair<Zona, double> zonaCercana = zonaMasCercana(vehiculo);
      break;
    } while (zonas.size() != 0);
    int tiempo = TiempoVolverDeposito(vehiculo);
    cout << "Tiempo: " << tiempo << endl;
    return rutasDeVehiculos;
//
//  break
//
    //} while (zonas.size() != 0);
    break;

  }
  return rutasDeVehiculos;


}

//1: K ← ∅
//2: k ← 1
//3: Mientras C ≠ ∅ hacer
//4:    R_k ← {depósito}
//5:    q_k ← Q                  // Capacidad residual del vehículo
//6:    T_k ← L                  // Tiempo residual de la ruta
//7:    Repetir
//8:        c' ← Zona más cercana a la última posición en R_k (dentro de C)
//9:        t' ← Tiempo para visitar c', una SWTS y volver al depósito
//10:       Si d_c' ≤ q_k y t' ≤ T_k entonces
//11:           R_k ← R_k ∪ {c'}
//12:           q_k ← q_k - d_c'
//13:           Actualizar T_k
//14:           C ← C \ {c'}
//15:       Sino
//16:           Si t' ≤ T_k entonces
//17:               s' ← SWTS más cercana a c'
//18:               R_k ← R_k ∪ {s'}
//19:               q_k ← Q      // Vaciar el vehículo
//20:               Actualizar T_k
//21:           Sino
//22:               Romper bucle
//23:           Fin Si
//24:       Fin Si
//25:   Hasta que no se puedan añadir más zonas
//26:   Si la última parada de R_k no es una SWTS entonces
//27:       s' ← SWTS más cercana a la última posición en R_k
//28:       R_k ← R_k ∪ {s'} ∪ {depósito}
//29:   Sino
//30:       R_k ← R_k ∪ {depósito}
//31:   K ← K ∪ {R_k}
//32:   k ← k + 1
//33: Fin Mientras
//34: Retornar K