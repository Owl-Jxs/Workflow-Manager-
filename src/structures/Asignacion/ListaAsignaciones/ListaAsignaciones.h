#ifndef LISTA_ASIGNACIONES_H
#define LISTA_ASIGNACIONES_H

#include "../NodoAsignacion/NodoAsignacion.h"

class ListaAsignaciones {
private:    
    NodoAsignacion* cabeza;
    NodoAsignacion* cola;
public:
    ListaAsignaciones();
    ~ListaAsignaciones();
    bool estaVacia();  // Verifica si la lista está vacía
    void agregarAsignacion(int idTarea, int idUsuario);
    void eliminarAsignacion(int idTarea, int idUsuario);
    bool buscarAsignacion(int idTarea, int idUsuario); 
//getters
    NodoAsignacion* getCabeza();
    NodoAsignacion* getCola();    
};

#endif