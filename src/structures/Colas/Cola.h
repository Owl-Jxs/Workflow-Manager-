#ifndef COLA_H
#define COLA_H
#include "NodoTarea/NodoTarea.h"
#include <stdexcept>

class Cola {
protected:
    NodoTarea* frente;
    NodoTarea* final;
    int cantidad;

public:
    Cola () {
        frente = nullptr;
        final = nullptr;
        cantidad = 0;
    }

    virtual ~Cola() {
        NodoTarea* aux = frente;
        while (aux != nullptr) {
            NodoTarea* temp = aux;
            aux = aux->siguiente;
            delete temp;
        }
    }
// Métodos comunes
    NodoTarea* getFrente() { return frente;}
    NodoTarea* getCola() { return final; }
    bool estaVacia() const { return frente == nullptr;}
    int getCantidad() const {return cantidad;}
    
    bool tareaRepetida(Tarea* tarea) const {
        NodoTarea* actual = frente;
        while (actual != nullptr) {
            if (actual->datos != nullptr && actual->datos->buscarSubTarea(tarea->getIdTarea()) != nullptr) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    Tarea* extraerTarea(int idTarea) {
        if (estaVacia()) return nullptr;
        NodoTarea* anterior = nullptr;  NodoTarea* actual = frente;

        while (actual != nullptr) {
            if (actual->datos != nullptr) {
                if (actual->datos->getIdTarea() == idTarea) {
                    Tarea* tarea = actual->datos;
                    if (anterior == nullptr) {
                        frente = actual->siguiente;
                        if (frente == nullptr) final = nullptr;
                    } else {
                        anterior->siguiente = actual->siguiente;
                        if (actual == final) final = anterior;
                    }
                    actual->datos = nullptr;
                    delete actual;
                    cantidad--;
                    return tarea;
                }
                Tarea* subExtraida = actual->datos->eliminarSubTarea(idTarea);
                if (subExtraida != nullptr) return subExtraida;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

// Métodos virtuales puros
    virtual void encolar(Tarea* tarea) = 0;
    virtual Tarea* desencolar() = 0; 
};

#endif
