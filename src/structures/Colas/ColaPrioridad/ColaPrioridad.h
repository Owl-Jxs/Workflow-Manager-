#ifndef COLAPRIORIDAD_H
#define COLAPRIORIDAD_H
#include "../Cola.h"
#include "../NodoTarea/NodoTarea.h"
#include <stdexcept>

class ColaPrioridad: public Cola {
public:
    ColaPrioridad() :Cola (){}
    ~ColaPrioridad() override {}

    void encolar(Tarea* tarea) override;
    Tarea* desencolar() override;
};


#endif