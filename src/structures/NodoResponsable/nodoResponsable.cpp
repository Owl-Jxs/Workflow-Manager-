#include "nodoResponsable.h"

nodoResponsable::nodoResponsable(int idUsuario) {
    this->idUsuario = idUsuario;
    this->siguiente = nullptr;
}

nodoResponsable::~nodoResponsable() {
    //la lista de responsables es administrada por Tarea
    this->siguiente = nullptr;
}