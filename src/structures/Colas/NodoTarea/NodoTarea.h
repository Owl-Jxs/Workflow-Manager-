#ifndef NODOTAREA_H
#define NODOTAREA_H
#include "../../../models/Tarea/Tarea.h"  // Incluye la definición de la clase Tarea

struct NodoTarea {
    Tarea* datos;
    NodoTarea* siguiente; 
    NodoTarea(Tarea* tarea);
    ~NodoTarea();
};

#endif