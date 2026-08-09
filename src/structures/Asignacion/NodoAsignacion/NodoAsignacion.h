#ifndef NODOASIGNACION_H
#define NODOASIGNACION_H
#include <iostream>
class  NodoAsignacion {
public:
    int idTarea;
    int idUsuario;
    NodoAsignacion* siguiente;
    NodoAsignacion* anterior;
    NodoAsignacion(int idTarea, int idUsuario);
    ~NodoAsignacion();
};

#endif