#ifndef COLAFIFO_H
#define COLAFIFO_H
#include "NodoTarea/NodoTarea.h"

class ColaFIFO {
private:
    NodoTarea* frente;
    NodoTarea* final;
    int cantidad;

public:
    ColaFIFO();
    ~ColaFIFO();
    void encolar(Tarea* tarea);
    Tarea* desencolar();
    NodoTarea* getFrente ();
    NodoTarea* getCola ();
    bool estaVacia() const;
    bool tareaRepetida (Tarea* tarea) const;
};

#endif