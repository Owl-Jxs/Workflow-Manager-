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
    } catch (std::exception& e) {
        delete  nuevaListaRegular; delete nuevaListaUrgente; 
        throw e; //lo lanzamos de nuevo
    }
//si habian datos anteriores los borramos
    if (listaTareasRegulares != nullptr) delete listaTareasRegulares;
    if (listaTareasUrgentes != nullptr) delete listaTareasUrgentes;
//cargamos las nuevas listas
    listaTareasRegulares = nuevaListaRegular;
    listaTareasUrgentes = nuevaListaUrgente;
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
    bool padreEncontrado = false;
    NodoTarea* nodoActual = ((perteneceListaUrgente) ? listaTareasUrgentes->getFrente () : listaTareasRegulares->getFrente ());

    while (nodoActual != nullptr && !padreEncontrado) { //buscamos la tarea padre 
        Tarea* tareaActual = nodoActual->datos;
        Tarea* encontrada = tareaActual->buscarSubTarea (idTareaPadre);
        if (encontrada != nullptr) {
            encontrada->agregarSubTarea (nuevaSubTarea);
            padreEncontrado = true;
       } else {
        nodoActual = nodoActual->siguiente;
       }
    }

//si no se encuentra la tarea padre
    if (!padreEncontrado) throw std::invalid_argument ("No existe ninguna tarea con el id pproporcionado"); 

    archivosTareas->guardarguardarNuevaSubTareaEnArchivo (nuevaSubTarea, perteneceListaUrgente);
}