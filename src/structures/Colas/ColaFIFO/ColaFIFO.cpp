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

/*
Tarea* ColaFIFO::extraerTarea(int idTarea) {
    if (estaVacia()) return nullptr;

    NodoTarea* anterior = nullptr;
    NodoTarea* actual = frente;

    while (actual != nullptr) {
        if (actual->datos != nullptr) {
            if (actual->datos->getIdTarea() == idTarea) {
                Tarea* tarea = actual->datos;
                if (anterior == nullptr) {
                    frente = actual->siguiente;
                    if (frente == nullptr) final = nullptr;
                } else {
                    anterior->siguiente = actual->siguiente;
                    if (actual == final) final = anterior;
                }
                actual->datos = nullptr;
                delete actual;
                cantidad--;
                return tarea;
            }
            Tarea* subExtraida = actual->datos->eliminarSubTarea(idTarea);
            if (subExtraida != nullptr) return subExtraida;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return nullptr;
}
*/