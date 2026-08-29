#include "ColaPrioridad.h"

using namespace std;

void ColaPrioridad::encolar(Tarea* tarea) {

    if (tarea == nullptr) throw invalid_argument("No se puede encolar una tarea nula.");
    if (tareaRepetida(tarea)) throw invalid_argument("No se puede agregar una tarea repetida.");
    
    NodoTarea* nuevo = new NodoTarea(tarea);
    if (estaVacia()) { // La cola está vacía
        frente = nuevo;
        final = nuevo;
        cantidad++;
        return;
    }

    if (tarea->getPrioridad() > frente->datos->getPrioridad()) {// Si la nueva tarea tiene mayor prioridad que la primera
        nuevo->siguiente = frente;
        frente = nuevo;

    } else {
        NodoTarea* actual = frente;
    //buscamos el ultimo nodo con responsabilidad mayor
        while (actual->siguiente != nullptr && actual->siguiente->datos->getPrioridad() >= tarea->getPrioridad()) {  
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;

        if (nuevo->siguiente == nullptr) {
            final = nuevo;
        }
    }
    cantidad++;
}


Tarea* ColaPrioridad::desencolar() {
    if (estaVacia()) throw out_of_range("No se puede desencolar: la cola esta vacia.");
    
    NodoTarea* aux = frente;
    Tarea* tarea = aux->datos;
    frente = frente->siguiente;

    if (frente == nullptr) {
        final = nullptr;
    }
    cantidad--;
    aux->datos = nullptr;
    delete aux;
    return tarea;
}