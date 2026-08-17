#include "ColaFIFO.h"
#include "nodoTarea.h"
#include "Tarea.h"
#include <stdexcept>

using namespace std;

ColaFIFO::ColaFIFO() {
    this->frente = nullptr;
    this->final = nullptr;
    this->cantidad = 0;
}

ColaFIFO::~ColaFIFO() {
    while (!estaVacia()) {
        desencolar();
    }
}

void ColaFIFO::encolar(Tarea* tarea) {
    if (tarea == nullptr) {
        throw invalid_argument("No se puede encolar una tarea nula.");
    }

    nodoTarea* nuevo = new nodoTarea(tarea);

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

    nodoTarea* aux = frente;

    Tarea* tarea = aux->datos;

    frente = frente->siguiente;

    if (frente == nullptr) {
        final = nullptr;
    }

    cantidad--;
    delete aux;
    return tarea;
}

bool ColaFIFO::estaVacia() const {
    return frente == nullptr;
}