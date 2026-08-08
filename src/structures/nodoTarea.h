#ifndef NODOTAREA_H
#define NODOTAREA_H

class Tarea;   // Declaración adelantada para evitar dependencias circulares

class nodoTarea {
public:
    Tarea* datos;
    nodoTarea* siguiente;

    nodoTarea(Tarea* tarea = nullptr);

    ~nodoTarea();
};

#endif
