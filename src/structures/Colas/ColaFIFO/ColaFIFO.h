#ifndef COLAFIFO_H
#define COLAFIFO_H
#include "../Cola.h"

class ColaFIFO: public Cola {
private:

public:
    ColaFIFO(): Cola () {}
    ~ColaFIFO() override {}
    void encolar(Tarea* tarea);
    Tarea* desencolar();
    //Tarea* extraerTarea(int idTarea);
};

#endif