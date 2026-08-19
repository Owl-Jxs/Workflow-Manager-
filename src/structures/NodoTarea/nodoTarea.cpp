#include "nodoTarea.h"
#include "../../models/Tarea/Tarea.h"

nodoTarea::nodoTarea(Tarea* tarea) {
    datos = tarea;
    siguiente = nullptr;
}

nodoTarea::~nodoTarea() {
    
    datos = nullptr;
    siguiente = nullptr;
}