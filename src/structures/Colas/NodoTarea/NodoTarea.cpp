#include "NodoTarea.h"


NodoTarea::NodoTarea(Tarea* tarea) {
    if (tarea == nullptr) {
        throw std::invalid_argument("La tarea no puede ser nula");
    }
    datos = tarea;
    siguiente = nullptr;
}

NodoTarea::~NodoTarea() {
    delete datos;  // Liberar la memoria de la tarea asociada
    datos = nullptr;
}