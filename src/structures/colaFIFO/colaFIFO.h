#ifndef COLAFIFO_H
#define COLAFIFO_H

class Tarea;
class nodoTarea;

class ColaFIFO {
private:
    nodoTarea* frente;
    nodoTarea* final;
    int cantidad;

public:
    ColaFIFO();
    ~ColaFIFO();
    void encolar(Tarea* tarea);
    Tarea* desencolar();
    bool estaVacia() const;
};

#endif