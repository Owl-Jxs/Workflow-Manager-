#ifndef PILAICOMANDO_H
#define PILAICOMANDO_H
#include "../../models/Comandos/IComando.h"

class NodoComando {
private:
    IComando* datos;
    NodoComando* siguiente;
public:
    NodoComando(IComando* comando);
    ~NodoComando();
    IComando* getDatos() const;
    NodoComando* getSiguiente() const;
    void setSiguiente(NodoComando* siguiente);
};


class PilaIComando {
private:
    NodoComando* tope;
    int cantidad;
public:
    PilaIComando();
    ~PilaIComando();
    void push(IComando* comando);
    IComando* pop();
    bool estaVacia() const;
    int getCantidad() const;
};

#endif