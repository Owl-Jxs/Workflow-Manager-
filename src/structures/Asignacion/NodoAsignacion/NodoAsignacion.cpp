#include "NodoAsignacion.h"

NodoAsignacion::NodoAsignacion(int idTarea, int idUsuario) {
    this->idTarea = idTarea;
    this->idUsuario = idUsuario;
    this->siguiente = nullptr;
    this->anterior = nullptr;
}

NodoAsignacion::~NodoAsignacion() { }