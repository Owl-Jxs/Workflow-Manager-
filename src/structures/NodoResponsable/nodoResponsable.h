#ifndef NODORESPONSABLE_H
#define NODORESPONSABLE_H

class nodoResponsable {
public:
    int idUsuario;
    nodoResponsable* siguiente;

    nodoResponsable(int idUsuario);
    ~nodoResponsable();
};

#endif