#include "GestorHistorial.h"
#include <stdexcept>
using namespace std;

GestorHistorial::GestorHistorial(AuditoriaDataBase* auditoriaParam, int idUsuarioSesionParam)
    : auditoria(auditoriaParam), idUsuarioSesion(idUsuarioSesionParam) { }

GestorHistorial::~GestorHistorial() { }

void GestorHistorial::setAuditoria(AuditoriaDataBase* auditoriaParam) {
    this->auditoria = auditoriaParam;
}

void GestorHistorial::setIdUsuarioSesion(int idUsuarioSesionParam) {
    this->idUsuarioSesion = idUsuarioSesionParam;
}

void GestorHistorial::ejecutarComando(IComando* comando) {
    if (comando == nullptr) throw invalid_argument("El comando no puede ser nulo.");

    try { //intentamos ejecutarlo
        comando->ejecutar();

    } catch (std::exception &e) {
        delete comando; throw;
    }

    //guardarlo para poder deshacerlo
    pilaDeshacer.push(comando);

    //registrar la accion en la bitacora de auditoria (Aclaratoria 3, parte 2)
    if (auditoria != nullptr) {
        auditoria->registrar(idUsuarioSesion, comando->getAccionAuditoria(), comando->getIdTareaAuditoria());
    }
   
    //se limpia la pila de rehacer.
    while (!pilaRehacer.estaVacia()) {
        IComando* comandoRehacer = pilaRehacer.pop();
        delete comandoRehacer;
    }
}

void GestorHistorial::deshacer() {
    if (pilaDeshacer.estaVacia())  throw std::invalid_argument ("La pila de deshacer se encuentra vacia");

    //sacamos el último comando ejecutado
    IComando* comando = pilaDeshacer.pop();

    try { //intentamos ejecutarlo
        comando->deshacer();

    } catch (std::exception &e) {
        delete comando; throw;
    }

    //registrar la accion en la bitacora de auditoria
    if (auditoria != nullptr) {
        auditoria->registrar(idUsuarioSesion, "DESHACER: " + comando->getAccionAuditoria(), comando->getIdTareaAuditoria());
    }

    //lo pasamos a la pila de rehacer
    pilaRehacer.push(comando);
}

void GestorHistorial::rehacer() {
    if (pilaRehacer.estaVacia()) throw std::invalid_argument ("La pila de rehacer se encuentra vacia");

    //sacamos el último comando deshecho
    IComando* comando = pilaRehacer.pop();

    try { //intentamos ejecutarlo
        comando->ejecutar ();

    } catch (std::exception &e) {
        delete comando; throw;
    }


   
    //registrar la accion en la bitacora de auditoria
    if (auditoria != nullptr) {
        auditoria->registrar(idUsuarioSesion, "REHACER: " + comando->getAccionAuditoria(), comando->getIdTareaAuditoria());
    }

    //se devuelve a la pila de deshacer
    pilaDeshacer.push(comando);
}