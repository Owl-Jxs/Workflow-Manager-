#include "TareaController.h"

//                              === === === METODOS PRIVATE  === === ===

 void TareaController:: aplanarArbol(Tarea* tarea, std::vector<Tarea*>& resultado) {
    if (tarea == nullptr) return;
    resultado.push_back(tarea);
    aplanarArbol(tarea->getPrimerSubTarea(), resultado);
    aplanarArbol(tarea->getSiguienteSubTarea(), resultado);
}

std::vector <Tarea*> TareaController::listarTareas (ColaFIFO* cola) {
    std::vector <Tarea*> lista;
    NodoTarea* nodoActual = cola->getFrente ();
    
    while (nodoActual != nullptr) {
        lista.push_back (nodoActual->datos);
        nodoActual = nodoActual->siguiente;
    }
    return lista;
}

std::vector <Tarea*> TareaController::listarSubTareas (Tarea* tarea) {
    std::vector <Tarea*> lista;
    Tarea* subTareaActual = tarea->getPrimerSubTarea ();
    aplanarArbol (subTareaActual, lista); 
    return lista;
}



//                              === === === CONSTRUCTOR Y DESCRUCTOR === === ===
TareaController::TareaController () {
    this->listaTareasRegulares = new ColaFIFO ();
    this->listaTareasUrgentes = new ColaFIFO ();
    this->archivosTareas = new TareaDataBase ();
    this->ultimoId = 0;
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
    bool errorCargaListaRegular = false, errorCargaListaUrgente = false;
//intentamos cargar los archivos lista por lista
    int idMaxRegular = 0;
    int idMaxUrgente = 0;

    try { //regular
        nuevaListaRegular = archivosTareas->cargarListaDelArchivo (false, idMaxRegular);
    } catch (std::exception &e ) {
        std::cout << e.what () << std::endl; errorCargaListaRegular = true;
        delete  nuevaListaRegular; idMaxRegular = 0;        
    }
    try { //urgente
        nuevaListaUrgente = archivosTareas->cargarListaDelArchivo (true, idMaxUrgente);
    } catch (std::exception &e) {
        std::cout << e.what () << std::endl; errorCargaListaUrgente = true;
        delete nuevaListaUrgente; idMaxUrgente = 0;         
    }

//si habian datos anteriores los borramos solo si no hubo error al cargar los archivos
    if (listaTareasRegulares != nullptr && !errorCargaListaRegular) delete listaTareasRegulares;
    if (listaTareasUrgentes != nullptr && !errorCargaListaUrgente) delete listaTareasUrgentes;

//cargamos las nuevas listas y el ultimo id
    if (!errorCargaListaRegular) listaTareasRegulares = nuevaListaRegular;
    if (!errorCargaListaUrgente) listaTareasUrgentes = nuevaListaUrgente;
    ultimoId = ((idMaxUrgente > idMaxRegular) ? idMaxUrgente : idMaxRegular); 
}


void TareaController::agregarTarea (Tarea* tarea, bool perteneceListaUrgente) {
    if (tarea == nullptr) throw std::invalid_argument ("Tarea no puede ser nula");

    int idNuevo = ultimoId + 1;
    tarea->setIdTarea (idNuevo);
    ultimoId++;

    ColaFIFO* lista = ((perteneceListaUrgente) ? listaTareasUrgentes : listaTareasRegulares);
    
    lista->encolar (tarea);

    try{
        archivosTareas->guardarNuevaTareaEnArchivo (tarea, perteneceListaUrgente);
    } catch (std::exception &e) {
        delete eliminarTarea (tarea->getIdTarea (), perteneceListaUrgente); throw;
    }
 }

void TareaController::agregarSubTarea (Tarea* nuevaSubTarea, int idTareaPadre, bool perteneceListaUrgente) {
    ColaFIFO* listaPadre = nullptr;
    Tarea* padre = buscarTarea (idTareaPadre);

//si no se encuentra la tarea padre
    if (padre == nullptr) throw std::invalid_argument ("No existe ninguna tarea con el id proporcionado");

//asignamos un nuevo id
    int nuevoId = ultimoId + 1;
    nuevaSubTarea->setIdTarea (nuevoId);
    ultimoId++;

    padre->agregarSubTarea (nuevaSubTarea);
    bool padreEsUrgente = (listaPadre == listaTareasUrgentes);

    try {
    archivosTareas->guardarNuevaSubTareaEnArchivo (nuevaSubTarea, padreEsUrgente);

    } catch  (std::exception &e) {
        padre->eliminarSubTarea (nuevaSubTarea->getIdPadre ());
    }
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

int TareaController::getUltimoId () const {
    return ultimoId;
}

// entrega la lista urgente en forma de vector
std::vector <Tarea*> TareaController::listarTareasUrgentes () {
    return listarTareas (listaTareasUrgentes);
}
// entrega la lista regulares en forma de vector
std::vector <Tarea*> TareaController::listarTareasRegulares () {
    return listarTareas (listaTareasRegulares);
}

std::vector<Tarea*> TareaController::listarSubTareas(int id) {
    Tarea* tareaBuscada = buscarTarea (id);
    std::vector <Tarea*> v; 
    if (tareaBuscada !=  nullptr) {
        v = listarSubTareas (tareaBuscada);
    }
    return v;
}
