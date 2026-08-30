#include "TareaController.h"

//                              === === === METODOS PRIVATE  === === ===
void TareaController:: aplanarArbol(Tarea* tarea, std::vector<Tarea*>& resultado) const {
    if (tarea == nullptr) return;
    resultado.push_back(tarea);
    aplanarArbol(tarea->getPrimerSubTarea(), resultado);    
    aplanarArbol(tarea->getSiguienteSubTarea(), resultado);
}

void TareaController::aplanarIdsArbol (Tarea* tarea, std::vector<int>& resultado){
    if (tarea == nullptr) return;
    resultado.push_back(tarea->getIdTarea ());
    aplanarIdsArbol(tarea->getPrimerSubTarea(), resultado);
    aplanarIdsArbol(tarea->getSiguienteSubTarea(), resultado);
}

void TareaController::cambiarEstadoArbol (Tarea* tarea, std::string nuevoEstado) {
    if (tarea == nullptr) return;
    tarea->setEstado (nuevoEstado);
    cambiarEstadoArbol (tarea->getPrimerSubTarea (), nuevoEstado);
    cambiarEstadoArbol (tarea->getSiguienteSubTarea (), nuevoEstado);
}

std::vector <Tarea*> TareaController::listarTareas (Cola* cola) const {
    std::vector <Tarea*> lista;
    NodoTarea* nodoActual = cola->getFrente ();
    while (nodoActual != nullptr) {
        lista.push_back (nodoActual->datos);
        nodoActual = nodoActual->siguiente;
    }   
    return lista;
}

std::vector <Tarea*> TareaController::listarSubTareas (Tarea* tarea) const {
    std::vector <Tarea*> lista;
    Tarea* subTareaActual = tarea->getPrimerSubTarea ();
    aplanarArbol (subTareaActual, lista); 
    return lista;
}

bool TareaController::subTareasCompletas (Tarea* tarea) {
    Tarea* subTareaActual = tarea->getPrimerSubTarea ();
    while (subTareaActual != nullptr) {
        if (subTareaActual->getEstado() != Tarea::ESTADO[3]) return false; 
        if (!subTareasCompletas(subTareaActual)) return false;
        subTareaActual = subTareaActual->getSiguienteSubTarea();
    }
    return true;
}

std::vector <int> TareaController::listarIdsArbol (Tarea* tarea) {
    std::vector <int> ids;  aplanarIdsArbol (tarea, ids);
    return ids;   
}

Tarea* TareaController::buscarTareaEnLista (int idTarea, Cola* lista) {
    NodoTarea* nodoActual = lista->getFrente ();
    while (nodoActual != nullptr) {
        Tarea* tareaActual = nodoActual->datos;
        if (tareaActual->getIdTarea () == idTarea) return tareaActual;
        nodoActual = nodoActual->siguiente;
    }
    return nullptr;
}

//                              === === === CONSTRUCTOR Y DESCRUCTOR === === ===
TareaController::TareaController () :cantidadCiclosParaEscalar (3) {
    this->listaTareasRegulares = new ColaFIFO ();
    this->listaTareasUrgentes = new ColaFIFO ();
    this->listaTareasEnRevision = new ColaPrioridad ();
    this->listaTareasCompletadas = new ColaFIFO ();
    this->archivosTareas = new TareaDataBase ();
    this->ultimoId = 0;
}

TareaController::~TareaController () {
    for (Tarea* t : listaTareasEnProceso) {
        delete t;
    }
    delete listaTareasRegulares;
    delete listaTareasUrgentes;
    delete listaTareasEnRevision;
    delete listaTareasCompletadas;
    delete archivosTareas;
}
//                                  === === ===     METODOS PUBLICOS    === === ===

void TareaController::guardarArchivos () { //guarda todos los archivos de las dos listas en la DB
    archivosTareas->guardarTareasActivas (listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision);
    archivosTareas->guardarTareasCompletadas (listaTareasCompletadas);
}

void TareaController::cargarArchivos () { //carga las dos listas de la DB
    try{
        archivosTareas->cargarTareasActivas (listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision, ultimoId);
    } catch (std::exception &e) {
        delete listaTareasRegulares;
        delete listaTareasUrgentes;
        delete listaTareasEnRevision;
        this-> listaTareasRegulares = new ColaFIFO ();
        this-> listaTareasUrgentes = new ColaFIFO ();
        this-> listaTareasEnRevision = new ColaPrioridad ();
        ultimoId = 0;     
    }
    try {
        archivosTareas->cargarTareasCompletadas (listaTareasCompletadas, ultimoId);
    } catch (...) {}
}

void TareaController::agregarTarea (Tarea* tarea) {
    if (tarea == nullptr) throw std::invalid_argument ("Tarea no puede ser nula");
    int idNuevo = ultimoId + 1;     tarea->setIdTarea (idNuevo);
    ultimoId++;

    if (tarea->getIdPadre () != Tarea::sinPadre) { //si la tarea es una subTarea buscamos su padre dentro de las listas
        Tarea* padre = buscarTareaPorHacer (tarea->getIdPadre ());
        if (padre == nullptr) padre = buscarTareaEnProceso (tarea->getIdPadre ());
        if (padre == nullptr) throw std::invalid_argument ("No existe ninguna tarea con el id proporcionado o bien esta en una tarea de revision que no puede agregar mas subtareas");
        padre->agregarSubTarea (tarea);

    } else { // si es una tarea Raiz
        ColaFIFO* lista = ((tarea->getPrioridad ()) ? listaTareasUrgentes : listaTareasRegulares);      
        lista->encolar (tarea);
    }

    try{
        archivosTareas->guardarNuevaTareaEnArchivo (tarea);
    } catch (std::exception &e) {
        Tarea * tareaAeliminar = buscarTareaPorHacer (tarea->getIdTarea ());
        delete tareaAeliminar; throw;
    }
 }

void TareaController::completarSubTarea (int idTarea) { 
    Tarea* t = buscarTareaEnProceso (idTarea);
    if (t != nullptr) {
        t->setEstado (Tarea::ESTADO[3]);
        archivosTareas->actualizarEstadoTarea (t);
    }
}

Tarea* TareaController::buscarTareaPorHacer (int idTarea){
    Tarea* tareaBuscada = buscarTareaEnLista (idTarea, listaTareasUrgentes);
    if (tareaBuscada == nullptr) { tareaBuscada = buscarTareaEnLista (idTarea, listaTareasRegulares); }
    return tareaBuscada;
}

Tarea*  TareaController::buscarTareaEnProceso (int idTarea) {
    for (Tarea* t : listaTareasEnProceso) {
        if (t->getIdTarea () == idTarea) return t;
    }
    return nullptr;
}

Tarea* TareaController::buscarTareaEnRevision (int idTarea) {
    return buscarTareaEnLista (idTarea, listaTareasEnRevision);
}

Tarea* TareaController::buscarTareaCompletada (int idTarea) {
    return buscarTareaEnLista (idTarea, listaTareasCompletadas);
}


Tarea* TareaController::eliminarTarea (int idTarea) {
    if (idTarea < 0) throw std::invalid_argument ("El id de la tarea no puede ser negativo");

    Tarea* eliminada = listaTareasUrgentes->extraerTarea (idTarea);
    if (eliminada == nullptr) eliminada = listaTareasRegulares->extraerTarea (idTarea); //si no se encontro
    
    if (eliminada == nullptr){
        for (Tarea* t : listaTareasEnProceso) {
            if (t->getIdTarea () == idTarea) {
                eliminada = t; break;
            }
        }
    }
    
    if (eliminada == nullptr) eliminada = listaTareasEnRevision->extraerTarea (idTarea);

    if (eliminada != nullptr) {
        archivosTareas->eliminarTareaActiva (listarIdsArbol (eliminada));
    }
    return eliminada;
}

Tarea* TareaController::escalarTarea () {
    NodoTarea* nodo = listaTareasRegulares->getFrente (); 
    Tarea* tarea = nullptr;
    if (nodo != nullptr) tarea = nodo->datos;
    if (tarea == nullptr) return nullptr;
    
    if (tarea-> getCiclosEspera () >= cantidadCiclosParaEscalar) {
        tarea->setPrioridad (true);
        tarea->reiniciarCiclosEspera ();
        listaTareasUrgentes->encolar (listaTareasRegulares->desencolar ());
        archivosTareas->actualizarEstadoTarea (tarea);
        return tarea;
    }
    return nullptr;
}

std::vector <int> TareaController::delegarTarea () {
    Tarea* tareaDelegada = nullptr;

    if (listaTareasUrgentes->getFrente () != nullptr) {
        tareaDelegada = listaTareasUrgentes->desencolar ();

    } else if (listaTareasRegulares->getFrente () != nullptr) {
        tareaDelegada = listaTareasRegulares->desencolar ();

    } else { throw std::invalid_argument (" NO hay tareas por delegar"); }
    
    listaTareasEnProceso.push_back (tareaDelegada);
    cambiarEstadoArbol (tareaDelegada,Tarea::ESTADO[1]);
    archivosTareas->actualizarEstadoTarea (tareaDelegada);

    std::vector<int> ids;
    aplanarIdsArbol (tareaDelegada, ids);
    return ids;
}

void TareaController::deshacerDelegacion (int idTarea) {
    if (idTarea < 0) throw std::invalid_argument ("Id invalido");

    Tarea* tareaDerrogada = nullptr;
    int indice = 0;
    for (Tarea* t : listaTareasEnProceso){
        if (t->getIdTarea () == idTarea) {
            tareaDerrogada = t; listaTareasEnProceso.erase (listaTareasEnProceso.begin () + indice); break;
        }
        indice++;
    }
    if (tareaDerrogada != nullptr){
        cambiarEstadoArbol ( tareaDerrogada, Tarea::ESTADO[0]);
    Cola* lista = ((tareaDerrogada->getPrioridad () ) ? listaTareasUrgentes : listaTareasRegulares);
    lista->encolar (tareaDerrogada);
    archivosTareas->actualizarEstadoTarea (tareaDerrogada);
    }
}

int TareaController::mandar_A_Revision (int idTarea) {
    if (idTarea < 0) throw std::invalid_argument ("Id invalido");

    Tarea* tarea = nullptr;
    int indice = 0;

    for (Tarea* t : listaTareasEnProceso) { //buscamos la tarea correspondiente
        if (t->getIdTarea () == idTarea) {
            tarea = t; //la marcamos como la correct
        //nos fijamos que tenga sus subTareas completas
            if (!subTareasCompletas (tarea)) throw std::invalid_argument ("Tarea con subTareas pendientes, completelas antes de mandarla a revision");
            listaTareasEnProceso.erase (listaTareasEnProceso.begin () + indice);
            break;
        }
        indice++;
    }
    if (tarea == nullptr) throw std::invalid_argument ("Tarea no encontrada en proceso");
    tarea->setEstado (Tarea::ESTADO[2]);
    listaTareasEnRevision->encolar (tarea);
    archivosTareas->actualizarEstadoTarea (tarea);
    return tarea->getIdTarea ();
}

void TareaController::revisionExitosa () {
    Tarea* tareaCompletada = listaTareasEnRevision->desencolar ();
    listaTareasCompletadas->encolar (tareaCompletada);
    tareaCompletada->setEstado (Tarea::ESTADO[3]);
    archivosTareas->registrarTareaCompletada (tareaCompletada);

}

void TareaController::anularRevisionExitosa (int idTarea) {
    Tarea* tareaAnulada = listaTareasCompletadas->extraerTarea (idTarea);
    if (tareaAnulada == nullptr) throw std::invalid_argument ("Tarea completa no existente");
    tareaAnulada->setEstado (Tarea::ESTADO[1]);
    listaTareasEnRevision->encolar (tareaAnulada);
    archivosTareas->actualizarEstadoTarea (tareaAnulada);
}

void TareaController::rechazarRevision (){
    Tarea* tareaRechazada = listaTareasEnRevision->desencolar ();
    listaTareasEnProceso.push_back (tareaRechazada);
    tareaRechazada->setEstado (Tarea::ESTADO[1]);
    archivosTareas->actualizarEstadoTarea (tareaRechazada);
}

void TareaController::anularRechazo (int idTarea) {
    if (idTarea < 0) throw std::invalid_argument ("ID INVALIDO");
    Tarea* tareaBuscada = nullptr;
    int indice = 0;
    for (Tarea* t : listaTareasEnProceso) {
        if (t->getIdTarea () == idTarea) {
            tareaBuscada = t; listaTareasEnProceso.erase (listaTareasEnProceso.begin () + indice);  break;
        }
        indice++;
    }
    if (tareaBuscada != nullptr) {
        tareaBuscada->setEstado (Tarea::ESTADO[2]);
        listaTareasEnRevision->encolar (tareaBuscada);
        archivosTareas->actualizarEstadoTarea (tareaBuscada);
    }
}   

Tarea* TareaController::getSiguienteTareaProcesable () const {
    NodoTarea* nodoFrente = listaTareasEnRevision->getFrente ();
    Tarea* siguienteEnRevisar = nullptr;
    if (nodoFrente != nullptr) siguienteEnRevisar = nodoFrente->datos;
    return siguienteEnRevisar;
}

int TareaController::getUltimoId () const {
    return ultimoId;
}

// entrega la lista urgente en forma de vector
std::vector <Tarea*> TareaController::listarTareasUrgentes () const {
    return  listarTareas (listaTareasUrgentes);
}
// entrega la lista regulares en forma de vector

std::vector <Tarea*> TareaController::listarTareasRegulares () const {
    return  listarTareas (listaTareasRegulares);
}

// entrega la lista en proceso en forma de vector
std::vector <Tarea*> TareaController::listarTareasEnProceso () const {
    return this->listaTareasEnProceso;
}
// entrega la lista regulares en forma de vector

std::vector <Tarea*> TareaController::listarTareasEnRevision () const{
    return listarTareas (listaTareasEnRevision);
}

std::vector <Tarea*> TareaController::listarTareasCompletadas () const {
    return listarTareas (listaTareasCompletadas);
}

std::vector<Tarea*> TareaController::listarTodasLasTareasActivas() const {
    std::vector<Tarea*> vectorTotalTareas;

    NodoTarea* NodoActual = listaTareasRegulares->getFrente(); // Tareas regulares
    while (NodoActual != nullptr) {
        aplanarArbol(NodoActual->datos, vectorTotalTareas);
        NodoActual = NodoActual->siguiente;
    }

    NodoActual = listaTareasUrgentes->getFrente(); // Tareas urgentes
    while (NodoActual != nullptr) {
        aplanarArbol(NodoActual->datos, vectorTotalTareas);
        NodoActual = NodoActual->siguiente;
    }

    for (Tarea* tarea : listaTareasEnProceso) { // Tareas en proceso
        if (tarea != nullptr) { vectorTotalTareas.push_back(tarea); }
    }

    NodoActual = listaTareasEnRevision->getFrente(); // Tareas en revisión
    while (NodoActual != nullptr) {
        aplanarArbol(NodoActual->datos, vectorTotalTareas);
        NodoActual = NodoActual->siguiente;
    }

    return vectorTotalTareas;
}

bool TareaController::listaPorHacerVacia () { return (listaTareasRegulares->estaVacia () && listaTareasUrgentes->estaVacia () ); }
bool TareaController::listaEnProcesoVacia () {return listaTareasEnProceso.empty ();}
bool TareaController::listaEnRevisionVacia () { return listaTareasEnRevision->estaVacia (); }
bool TareaController::listaCompletadasVacia () { return listaTareasCompletadas->estaVacia ();}