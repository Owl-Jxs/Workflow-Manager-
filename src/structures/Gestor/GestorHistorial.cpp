#include "GestorHistorial.h"
#include <stdexcept>
using namespace std;

GestorHistorial::GestorHistorial() {
}

GestorHistorial::~GestorHistorial() {
}

void GestorHistorial::ejecutarComando(IComando* comando) {
    if (comando == nullptr) {
        throw invalid_argument("El comando no puede ser nulo.");
    }
    comando->ejecutar();

    //guardarlo para poder deshacerlo
    pilaDeshacer.push(comando);

    //se limpia la pila de rehacer.
    while (!pilaRehacer.estaVacia()) {
        IComando* comandoRehacer = pilaRehacer.pop();
        delete comandoRehacer;
    }
}

void GestorHistorial::deshacer() {
    if (pilaDeshacer.estaVacia()) {
        return;
    }

    //sacamos el último comando ejecutado
    IComando* comando = pilaDeshacer.pop();

    //deshacer el comando
    comando->deshacer();

    //lo pasamos a la pila de rehacer
    pilaRehacer.push(comando);
}

void GestorHistorial::rehacer() {
    if (pilaRehacer.estaVacia()) {
        return;
    }

    //sacamos el último comando deshecho
    IComando* comando = pilaRehacer.pop();

    //ejecutamos el comando
    comando->ejecutar();

    //se devuelve a la pila de deshacer
    pilaDeshacer.push(comando);
}