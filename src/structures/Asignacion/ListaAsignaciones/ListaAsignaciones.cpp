#include "ListaAsignaciones.h"

ListaAsignaciones::ListaAsignaciones() {
    cabeza = nullptr;
    cola = nullptr;
}

ListaAsignaciones::~ListaAsignaciones() {
    NodoAsignacion* actual = cabeza;
    while (actual != nullptr) {
        NodoAsignacion* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

bool ListaAsignaciones::estaVacia() {
    return cabeza == nullptr;
}

void ListaAsignaciones::agregarAsignacion (int idTarea, int idUsuario) {
    if (idTarea < 0 || idUsuario < 0)    throw std::invalid_argument ("Error: Los IDs no pueden ser negativos.");
    if (buscarAsignacion (idTarea, idUsuario))    throw std::invalid_argument ("Error: La asignacion ya existe en la lista.");

    NodoAsignacion* nuevoNodo = new NodoAsignacion(idTarea, idUsuario);
    if (estaVacia()) {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    } else {
        cola->siguiente = nuevoNodo;
        nuevoNodo->anterior = cola;
        cola = nuevoNodo;
    }
}

void ListaAsignaciones::eliminarAsignacion(int idTarea, int idUsuario) {
    if (estaVacia())   throw std::runtime_error("Error: La lista está vacía. No se puede eliminar la asignación.");
    if (idTarea < 0 || idUsuario < 0)    throw std::invalid_argument("Error: Los IDs no pueden ser negativos.");

    NodoAsignacion* actual = cabeza;
    while (actual != nullptr) {
        if (actual->idTarea == idTarea && actual->idUsuario == idUsuario) {
            if (actual->anterior != nullptr) {
                actual->anterior->siguiente = actual->siguiente;
            } else {
                cabeza = actual->siguiente; // Si es el primer nodo
            }

            if (actual->siguiente != nullptr) {
                actual->siguiente->anterior = actual->anterior;
            } else {
                cola = actual->anterior; // Si es el último nodo
            }

            delete actual;
            return;
        }
        actual = actual->siguiente;
    }

    throw std::runtime_error("Error: No se encontró la asignación especificada para eliminar.");
}

bool ListaAsignaciones::buscarAsignacion(int idTarea, int idUsuario) {
    if (idTarea < 0 || idUsuario < 0)    throw std::invalid_argument("Error: Los IDs no pueden ser negativos.");
    NodoAsignacion* actual = cabeza;
    while (actual != nullptr) {
        if (actual->idTarea == idTarea && actual->idUsuario == idUsuario) {
            return true; // Asignación encontrada
        }
        actual = actual->siguiente;
    }
    return false; // Asignación no encontrada
}
//getters
NodoAsignacion* ListaAsignaciones::getCabeza() {
    return cabeza;
}

NodoAsignacion* ListaAsignaciones::getCola() {
    return cola;
}