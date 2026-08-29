#include "ColaFIFO.h"
#include <stdexcept>
using namespace std;

void ColaFIFO::encolar(Tarea* tarea) {
    if (tarea == nullptr) {
        throw invalid_argument("No se puede encolar una tarea nula.");
    }
    if (tareaRepetida (tarea))  throw std::invalid_argument ("No se puede agregar una tarea repetida");
    NodoTarea* nuevo = new NodoTarea(tarea);

    if (estaVacia()) {
        frente = nuevo;
        final = nuevo;
    }
    else {
        final->siguiente = nuevo;
        final = nuevo;
    }

    cantidad++;
}

Tarea* ColaFIFO::desencolar() {
    if (estaVacia()) {
        throw out_of_range("No se puede desencolar: la cola esta vacia.");
    }
// Guardar el nodo frente y obtener la tarea
    NodoTarea* aux = frente;
    Tarea* tarea = aux->datos;
    frente = frente->siguiente;
//

    if (frente == nullptr) {
        final = nullptr;
    }

    cantidad--;
    aux->datos = nullptr;
    delete aux;
    return tarea;
}

