/**
 * @class Clase para repressentar un algoritmo en el sistema
 */

#ifndef C_Algoritmo_H
#define C_Algoritmo_H

#include <iostream>
#include <vector>
#include <memory>

class Tools; // Declaración anticipada de la clase Tools

class Algoritmo {
  public:
    // Constructor de la clase
    Algoritmo() : dato_(nullptr) {}
    Algoritmo(Tools& dato) : dato_(&dato) {}

    // Destructor de la clase (virtual: se destruye a través de un puntero a la clase base)
    virtual ~Algoritmo() {}

    // Métodos de la clase
    virtual void ejecutar() = 0;
    virtual void mostrarResultados() = 0;

    // Setters
    inline void setDato(Tools& dato) { dato_ = &dato; }

    // Getters
    inline const std::vector<std::shared_ptr<Tools>>& getDatos() const { return datos_; } // Instancias resueltas, para exportar/visualizar resultados

  protected:
    // Datos importantes para el algoritmo
    Tools* dato_;
    std::vector<std::shared_ptr<Tools>> datos_; // Instancias generadas por el algoritmo (con dueño compartido para evitar fugas y punteros colgantes)
};

#endif