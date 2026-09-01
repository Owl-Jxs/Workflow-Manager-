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
        Tarea* sub = tareaActual->buscarSubTarea(idTarea);
        if (sub != nullptr && sub != tareaActual) return sub;
        if (sub == tareaActual) return sub;
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
        std::cerr << "Advertencia: error al cargar tareas activas: " << e.what() << " - se inicializan listas vacias." << std::endl;
        for (Tarea* t : listaTareasEnProceso) { delete t; }
        listaTareasEnProceso.clear();
        delete listaTareasRegulares;
        delete listaTareasUrgentes;
        delete listaTareasEnRevision;
        delete listaTareasCompletadas;
        this-> listaTareasRegulares = new ColaFIFO ();
        this-> listaTareasUrgentes = new ColaFIFO ();
        this-> listaTareasEnRevision = new ColaPrioridad ();
        this-> listaTareasCompletadas = new ColaFIFO ();
        ultimoId = 0;     
    }
    try {
        archivosTareas->cargarTareasCompletadas (listaTareasCompletadas, ultimoId);
    } catch (const std::exception& e) {
        std::cerr << "Advertencia: error al cargar tareas completadas: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Advertencia: error desconocido al cargar tareas completadas" << std::endl;
    }
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
        Tarea* rollback = eliminarTarea(tarea->getIdTarea());
        if (rollback != nullptr) delete rollback;
        else {
            // fallback: if eliminarTarea did not find (e.g., subtask parent not in queue), try manual detach
            // but eliminarTarea now handles subtasks, so this is safety
            delete tarea;
        }
        ultimoId--;
        throw;
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
        Tarea* sub = t->buscarSubTarea(idTarea);
        if (sub != nullptr && sub != t) return sub;
        if (sub == t) return sub;
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
    bool esActiva = (eliminada != nullptr);
    if (eliminada == nullptr) {
        eliminada = listaTareasRegulares->extraerTarea (idTarea);
        esActiva = (eliminada != nullptr);
    }
    
    if (eliminada == nullptr){
        int indice = 0;
        for (size_t i = 0; i < listaTareasEnProceso.size(); ++i) {
            Tarea* t = listaTareasEnProceso[i];
            if (t->getIdTarea () == idTarea) {
                eliminada = t;
                listaTareasEnProceso.erase (listaTareasEnProceso.begin () + (int)i);
                esActiva = true;
                break;
            }
            Tarea* sub = t->eliminarSubTarea(idTarea);
            if (sub != nullptr) {
                eliminada = sub;
                esActiva = true;
                // persistir cambio de cantidadSubTareas del padre
                try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(...) {}
                break;
            }
        }
    }
    
    if (eliminada == nullptr) {
        eliminada = listaTareasEnRevision->extraerTarea (idTarea);
        esActiva = (eliminada != nullptr);
    }

    if (eliminada == nullptr) {
        eliminada = listaTareasCompletadas->extraerTarea (idTarea);
        if (eliminada != nullptr) {
            try { archivosTareas->eliminarRegistroTareaCompletada(listarIdsArbol(eliminada)); } catch(...) {}
            return eliminada;
        }
    }

    if (eliminada != nullptr && esActiva) {
        try { archivosTareas->eliminarTareaActiva (listarIdsArbol (eliminada)); } catch(...) {}
    }
    return eliminada;
}

Tarea* TareaController::escalarTarea () {
    if (listaTareasRegulares->estaVacia()) return nullptr;
    // Recolectar ids de tareas regulares que cumplen condicion (evitar modificar mientras se itera)
    std::vector<int> idsAescalar;
    NodoTarea* nodo = listaTareasRegulares->getFrente();
    while (nodo != nullptr) {
        if (nodo->datos != nullptr && nodo->datos->getCiclosEspera() >= cantidadCiclosParaEscalar) {
            idsAescalar.push_back(nodo->datos->getIdTarea());
        }
        nodo = nodo->siguiente;
    }
    Tarea* ultimaEscalada = nullptr;
    for (int id : idsAescalar) {
        Tarea* t = listaTareasRegulares->extraerTarea(id);
        if (t != nullptr) {
            t->setPrioridad(true);
            t->reiniciarCiclosEspera();
            listaTareasUrgentes->encolar(t);
            try { archivosTareas->actualizarEstadoTarea(t); } catch(...) {}
            ultimaEscalada = t;
        }
    }
    return ultimaEscalada;
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
    // Persistir todo el arbol (incluye subtareas) reescribiendo archivo activo
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(const std::exception& e) { throw; }

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
    if (tareaDerrogada == nullptr) throw std::invalid_argument("Tarea no encontrada en proceso para deshacer delegacion");
    cambiarEstadoArbol ( tareaDerrogada, Tarea::ESTADO[0]);
    Cola* lista = ((tareaDerrogada->getPrioridad () ) ? listaTareasUrgentes : listaTareasRegulares);
    lista->encolar (tareaDerrogada);
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(const std::exception& e) { throw; }
}

int TareaController::mandar_A_Revision (int idTarea) {
    if (idTarea < 0) throw std::invalid_argument ("Id invalido");

    Tarea* tarea = nullptr;
    int indiceRoot = -1;
    Tarea* padreDeSubtarea = nullptr;

    for (size_t i = 0; i < listaTareasEnProceso.size(); ++i) {
        Tarea* t = listaTareasEnProceso[i];
        if (t->getIdTarea() == idTarea) {
            tarea = t;
            indiceRoot = (int)i;
            break;
        }
        Tarea* sub = t->buscarSubTarea(idTarea);
        if (sub != nullptr && sub != t) {
            tarea = sub;
            padreDeSubtarea = t;
            break;
        }
    }
    if (tarea == nullptr) throw std::invalid_argument ("Tarea no encontrada en proceso. Verifique que el ID corresponde a una tarea EN PROCESO y que este asignada.");

    // Si la tarea tiene subtareas, permitir el envio aunque no esten completadas:
    // auto-completar jerarquia no es necesario, simplemente permitir el pase a revision
    // (cambiarEstadoArbol movera todo el subarbol). No bloquear por subTareasCompletas.
    // Si se requiere validacion estricta, descomentar:
    // if (!subTareasCompletas(tarea)) throw std::invalid_argument("Tarea con subTareas pendientes, completelas antes de mandarla a revision");

    if (padreDeSubtarea != nullptr) {
        // Es una subtarea: extraer solo el subarbol de su padre
        Tarea* extraida = padreDeSubtarea->eliminarSubTarea(idTarea);
        if (extraida == nullptr) throw std::invalid_argument("No se pudo extraer la subtarea del padre");
        tarea = extraida;
    } else {
        // Es una tarea raiz: remover del vector
        listaTareasEnProceso.erase(listaTareasEnProceso.begin() + indiceRoot);
    }

    cambiarEstadoArbol(tarea, Tarea::ESTADO[2]);
    listaTareasEnRevision->encolar(tarea);
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(const std::exception& e) { throw; }
    return tarea->getIdTarea();
}

void TareaController::deshacerMandarARevision(int idTarea) {
    if (idTarea < 0) throw std::invalid_argument("Id invalido");
    Tarea* tarea = listaTareasEnRevision->extraerTarea(idTarea);
    if (tarea == nullptr) throw std::invalid_argument("Tarea no encontrada en revision para deshacer");
    cambiarEstadoArbol(tarea, Tarea::ESTADO[1]);
    listaTareasEnProceso.push_back(tarea);
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(...) { throw; }
}

void TareaController::revisionExitosa () {
    if (listaTareasEnRevision->estaVacia()) throw std::invalid_argument("No hay tareas en cola de revision");
    Tarea* tareaCompletada = listaTareasEnRevision->desencolar ();
    cambiarEstadoArbol(tareaCompletada, Tarea::ESTADO[3]);
    listaTareasCompletadas->encolar (tareaCompletada);
    // Mover del archivo activo al de completadas (incluye subtareas)
    try {
        archivosTareas->eliminarTareaActiva(listarIdsArbol(tareaCompletada));
    } catch(...) {}
    try { archivosTareas->registrarTareaCompletada (tareaCompletada); } catch(...) { throw; }
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(...) {}
}

void TareaController::anularRevisionExitosa (int idTarea) {
    Tarea* tareaAnulada = listaTareasCompletadas->extraerTarea (idTarea);
    if (tareaAnulada == nullptr) throw std::invalid_argument ("Tarea completa no existente");
    cambiarEstadoArbol(tareaAnulada, Tarea::ESTADO[2]);
    listaTareasEnRevision->encolar (tareaAnulada);
    try { archivosTareas->eliminarRegistroTareaCompletada(listarIdsArbol(tareaAnulada)); } catch(...) {}
    try { archivosTareas->guardarNuevaTareaEnArchivo(tareaAnulada); } catch(...) { throw; }
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(...) {}
}

void TareaController::rechazarRevision (){
    if (listaTareasEnRevision->estaVacia()) throw std::invalid_argument("No hay tareas en cola de revision");
    Tarea* tareaRechazada = listaTareasEnRevision->desencolar ();
    listaTareasEnProceso.push_back (tareaRechazada);
    cambiarEstadoArbol(tareaRechazada, Tarea::ESTADO[1]);
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(...) { throw; }
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
    if (tareaBuscada == nullptr) throw std::invalid_argument("Tarea no encontrada en proceso para anular rechazo");
    cambiarEstadoArbol(tareaBuscada, Tarea::ESTADO[2]);
    listaTareasEnRevision->encolar (tareaBuscada);
    try { archivosTareas->guardarTareasActivas(listaTareasRegulares, listaTareasUrgentes, listaTareasEnProceso, listaTareasEnRevision); } catch(...) { throw; }
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
        if (tarea != nullptr) { aplanarArbol(tarea, vectorTotalTareas); }
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