#ifndef USUARIOCONDICION_H
#define USUARIOCONDICION_H
#include "../../models/Usuario/Usuario.h"
#include "Icondicion.h"

//orden ascendente 
class ordenarUsuarioPorIdAscedente: public Icondicion<Usuario*> {
public:
        
    bool ordenar ( Usuario* a,  Usuario* b) override {
        return a->getId () < b->getId ();
    }
};


// orden descendente
class ordenarUsuarioPorIdDescendente : public Icondicion<Usuario*> {
public:
    bool ordenar ( Usuario* a,  Usuario* b) override {
        return a-> getId () > b->getId ();
    }
};
#endif