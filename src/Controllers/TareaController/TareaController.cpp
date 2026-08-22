#include "TareaController.h"

//                              === === === CONSTRUCTOR Y DESCRUCTOR === === ===
TareaController::TareaController () {
    this->listaTareasRegulares = new ColaFIFO ();
    this->listaTareasUrgentes = new ColaFIFO ();
    this->archivosTareas = new TareaDataBase ();
}

TareaController::~TareaController () {
    delete listaTareasRegulares;
    delete listaTareasUrgentes;
    delete archivosTareas;
}

//                                  === === ===     METODOS PUBLICOS    === === ===

void TareaController::guardarArchivos () { //guarda todos los archivos de las dos listas en la DB
    archivosTareas->guardarListaEnArchivo (listaTareasRegulares, false);
    archivosTareas->guardarListaEnArchivo (listaTareasUrgentes, true);
}

void TareaController::cargarArchivos () { //carga las dos listas de la DB
    ColaFIFO* nuevaListaRegular = nullptr;
    ColaFIFO* nuevaListaUrgente = nullptr;
//intentamos cargar los archivos 
    try {
        nuevaListaRegular = archivosTareas->cargarListaDelArchivo (false);
        nuevaListaUrgente = archivosTareas->cargarListaDelArchivo (true);
    } catch (std::exception&) {
        delete  nuevaListaRegular; delete nuevaListaUrgente; 
        throw; //lo lanzamos de nuevo sin cortar su tipo
    }
//si habian datos anteriores los borramos
    if (listaTareasRegulares != nullptr) delete listaTareasRegulares;
    if (listaTareasUrgentes != nullptr) delete listaTareasUrgentes;
//cargamos las nuevas listas
    listaTareasRegulares = nuevaListaRegular;
    listaTareasUrgentes = nuevaListaUrgente;
}

 ColaFIFO* TareaController::getListaTareasRegulares () {
    return this->listaTareasRegulares;
}

ColaFIFO* TareaController::getListaTareasUrgentes () {
    return this->listaTareasUrgentes;
}

void TareaController::agregarTarea (Tarea* tarea, bool perteneceListaUrgente) {
    if (perteneceListaUrgente) {
        listaTareasUrgentes->encolar (tarea);
    } else {
        listaTareasRegulares->encolar (tarea);
    } 

    archivosTareas->guardarNuevaTareaEnArchivo (tarea, perteneceListaUrgente);
 }

void TareaController::agregarSubTarea (Tarea* nuevaSubTarea, int idTareaPadre, bool perteneceListaUrgente) {
    (void) perteneceListaUrgente; // la lista destino se determina donde se encuentre el padre

    ColaFIFO* listas[2] = { listaTareasRegulares, listaTareasUrgentes };
    ColaFIFO* listaPadre = nullptr;
    Tarea* padre = nullptr;

    for (ColaFIFO* lista : listas) { //buscamos la tarea padre en ambas listas
        NodoTarea* nodoActual = lista->getFrente ();
        while (nodoActual != nullptr) {
            Tarea* encontrada = nodoActual->datos->buscarSubTarea (idTareaPadre);
            if (encontrada != nullptr) {
                padre = encontrada;
                listaPadre = lista;
                break;
            }
            nodoActual = nodoActual->siguiente;
        }
        if (padre != nullptr) break;
    }

//si no se encuentra la tarea padre
    if (padre == nullptr) throw std::invalid_argument ("No existe ninguna tarea con el id proporcionado");

    padre->agregarSubTarea (nuevaSubTarea);
    bool padreEsUrgente = (listaPadre == listaTareasUrgentes);
    archivosTareas->guardarNuevaSubTareaEnArchivo (nuevaSubTarea, padreEsUrgente);
}

Tarea* TareaController::buscarTarea (int idTarea) {
    ColaFIFO* listas [2] = { listaTareasRegulares, listaTareasUrgentes };
    for (ColaFIFO* lista : listas) {
        NodoTarea* nodoActual = lista->getFrente ();
        while (nodoActual != nullptr) {
            Tarea* encontrada = nodoActual->datos->buscarSubTarea (idTarea);
            if (encontrada != nullptr) return encontrada;
            nodoActual = nodoActual->siguiente;
        }
    }
    return nullptr;
}

Tarea* TareaController::eliminarTarea (int idTarea, bool perteneceListaUrgente) {
    if (idTarea < 0) throw std::invalid_argument ("El id de la tarea no puede ser negativo");
    Tarea* eliminada = perteneceListaUrgente
        ? listaTareasUrgentes->extraerTarea (idTarea)
        : listaTareasRegulares->extraerTarea (idTarea);
    if (eliminada == nullptr) {
        ColaFIFO* otra = perteneceListaUrgente ? listaTareasRegulares : listaTareasUrgentes;
        eliminada = otra->extraerTarea(idTarea);
        if (eliminada == nullptr) throw std::invalid_argument ("No existe una tarea con el id proporcionado");
    }
    guardarArchivos();
    return eliminada;
}

Tarea* TareaController::escalarTarea (int idTarea) {
    Tarea* tarea = listaTareasRegulares->extraerTarea (idTarea);
    if (tarea == nullptr) {
        throw std::invalid_argument ("No existe una tarea regular con el id proporcionado");
    }
    tarea->setPrioridad (true);
    tarea->reiniciarCiclosEspera ();
    listaTareasUrgentes->encolar (tarea);
    guardarArchivos ();
    return tarea;
}

namespace {
    void aplanarArbol(Tarea* tarea, std::vector<Tarea*>& resultado) {
        if (tarea == nullptr) return;
        resultado.push_back(tarea);
        aplanarArbol(tarea->getPrimerSubTarea(), resultado);
        aplanarArbol(tarea->getSiguienteSubTarea(), resultado);
    }

    void aplanarCola(ColaFIFO* cola, std::vector<Tarea*>& resultado) {
        NodoTarea* actual = cola->getFrente();
        while (actual != nullptr) {
            aplanarArbol(actual->datos, resultado);
            actual = actual->siguiente;
        }
    }
}

std::vector<Tarea*> TareaController::listarTodasLasTareas() {
    std::vector<Tarea*> resultado;
    aplanarCola(listaTareasRegulares, resultado);
    aplanarCola(listaTareasUrgentes, resultado);
    return resultado;
}