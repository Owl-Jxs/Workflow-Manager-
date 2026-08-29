#ifndef TAREACONDICION_H
#define TAREACONDICION_H
#include "../../models/Usuario/Usuario.h"
#include "Icondicion.h"

//orden ascendente 
class ordenarTareaPorIdAscedente: public Icondicion<Tarea*> {
public:
    bool ordenar ( Tarea* a, Tarea* b) override {
        return a->getIdTarea () <= b->getIdTarea ();
    }
};


// orden descendente
class ordenarTareaPorIdDescendente : public Icondicion<Tarea*> {
public:
    bool ordenar ( Tarea* a,  Tarea* b) override {
        return a->getIdTarea () >= b->getIdTarea ();
    }
};


#endif