#include "PilaIComando.h"
#include <stdexcept>
using namespace std;

NodoComando::NodoComando(IComando* comando) {
    this->datos = comando;
    this->siguiente = nullptr;
}

NodoComando::~NodoComando() {
}

IComando* NodoComando::getDatos() const {
    return this->datos;
}

NodoComando* NodoComando::getSiguiente() const {
    return this->siguiente;
}

void NodoComando::setSiguiente(NodoComando* siguiente) {
    this->siguiente = siguiente;
}

PilaIComando::PilaIComando() {
    this->tope = nullptr;
    this->cantidad = 0;
}

PilaIComando::~PilaIComando() {
    while (!estaVacia()) {
        IComando* comando = pop();
        delete comando;
    }
}

void PilaIComando::push(IComando* comando) {
    if (comando == nullptr) {
        throw invalid_argument("No se puede insertar un comando nulo.");
    }
    NodoComando* nuevo = new NodoComando(comando);
    nuevo->setSiguiente(this->tope);
    this->tope = nuevo;
    this->cantidad++;
}

IComando* PilaIComando::pop() {
    if (estaVacia()) {
        return nullptr;
    }
    NodoComando* nodo = this->tope;
    IComando* comando = nodo->getDatos();
    this->tope = nodo->getSiguiente();
    delete nodo;
    this->cantidad--;
    return comando;
}

bool PilaIComando::estaVacia() const {
    return this->tope == nullptr;
}

int PilaIComando::getCantidad() const {
    return this->cantidad;
}