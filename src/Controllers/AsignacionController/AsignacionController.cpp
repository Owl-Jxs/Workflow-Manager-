# include "AsignacionController.h"

//                                === === ===     CONSTRUCTOR && DESTRUCTOR    === === ===
AsignacionController::AsignacionController (){
    this-> archivosAsignaciones = new AsignacionDataBase  ();
    this-> listaAsignaciones = new ListaAsignaciones ();
}

AsignacionController:: ~AsignacionController () {
    delete this->archivosAsignaciones;
    delete this->listaAsignaciones;
}

//                                 === === ===     METODOS PUBLICOS    === === ===

void AsignacionController::guardarAsignaciones () {
    archivosAsignaciones->guardarAsignacionesEnArchivo (listaAsignaciones);
}

void AsignacionController::cargarAsignaciones () {
    ListaAsignaciones* listaCargada = nullptr;

    try { //intentamos cargar la lista
        listaCargada = archivosAsignaciones->cargarAsignacionesDesdeArchivo ();
    } catch (std::exception&) {
        delete listaCargada; throw; //re-lanzamos sin cortar el tipo de la excepcion
    }
    if (listaAsignaciones != nullptr) delete listaAsignaciones; //se borra una posible lista anterior si la hay
    listaAsignaciones = listaCargada;
}

void AsignacionController::guardarAsignacionesCompletadas (std::vector<std::pair<int, int>> listaAsignaciones){
    archivosAsignaciones->guardarAsignacionesCompletadas (listaAsignaciones);
}

void AsignacionController::deshacerAsignacionesCompletadas (std::vector <std::pair<int, int>> listaAsignacionesRehacer){
    archivosAsignaciones->deshacerAsignacionesCompletadas (listaAsignacionesRehacer);
} 


void AsignacionController:: agregarAsignacion(int idTarea, int idUsuario) {
    if (idTarea < 0) throw std::invalid_argument ("el id de la tarea no puede ser negativo ");
    if (idUsuario < 0) throw std::invalid_argument ("el id del usuario no puede ser negativo ");
    if (listaAsignaciones->buscarAsignacion (idTarea, idUsuario) ) throw std::invalid_argument ("la asignacion ya existe ");

    listaAsignaciones->agregarAsignacion (idTarea, idUsuario);
    try {
    archivosAsignaciones->agregarAsignacion (idTarea, idUsuario);
    } catch (std::exception &e) {
        listaAsignaciones->eliminarAsignacion (idTarea, idUsuario); throw;
    }

}

void AsignacionController::eliminarAsignacion(int idTarea, int idUsuario) {
    if (idTarea < 0) throw std::invalid_argument ("el id de la tarea no puede ser negativo ");
    if (idUsuario < 0) throw std::invalid_argument ("el id del usuario no puede ser negativo ");
    if (! (listaAsignaciones->buscarAsignacion (idTarea, idUsuario) ) ) throw std::invalid_argument ("la asignacion no existe ");
    
    listaAsignaciones->eliminarAsignacion (idTarea, idUsuario);
    archivosAsignaciones->eliminarAsignacion (idTarea, idUsuario);
}

void AsignacionController::eliminarAsignacionPorIdTarea (int idTareaParam) {
    std::vector <int> id;
    id.push_back (idTareaParam);
    std::vector<std::pair <int, int>> pares = getAsignacionesResponsablesDeTarea (id);
    
    for (std::pair <int, int> par : pares) {
        int idTarea = par.first;
        int idUsuario = par.second;
        listaAsignaciones->eliminarAsignacion (idTarea, idUsuario);
        archivosAsignaciones->eliminarAsignacion (idTarea, idUsuario);
    }
}

void AsignacionController::eliminarAsignacionPorIdUsuario (int idUsuario){
    std::vector <int> ids;
    ids.push_back (idUsuario);
    std::vector<std::pair <int, int>> pares = getAsignacionesTareasPorUsuario (ids);
    
    for (std::pair <int, int> par : pares) {
        int idTarea = par.first;
        int idUsuarioPar = par.second;
        listaAsignaciones->eliminarAsignacion (idTarea, idUsuarioPar);
        archivosAsignaciones->eliminarAsignacion (idTarea, idUsuarioPar);
    }
}

bool AsignacionController::buscarAsignacion(int idTarea, int idUsuario) {
    return listaAsignaciones->buscarAsignacion (idTarea, idUsuario);
}


std::vector<std::pair <int, int>> AsignacionController::getAsignacionesResponsablesDeTarea (std::vector <int> idsTareas) {
    NodoAsignacion* actual = listaAsignaciones->getCabeza ();
    std::vector<std::pair <int, int>> vectorUsuariosResponsables;

    while (actual != nullptr) {
        for (int i: idsTareas) {
            if (actual->idTarea == i) {
                vectorUsuariosResponsables.push_back ( {i,actual->idUsuario});
            }
        }
        
        actual = actual->siguiente;
    }
    return vectorUsuariosResponsables;
}

std::vector<std::pair <int, int>> AsignacionController::getAsignacionesTareasPorUsuario (std::vector <int> idsResponsables) {
    NodoAsignacion* actual = listaAsignaciones->getCabeza ();
    std::vector<std::pair <int, int>> vectorTareasUsuario;

    while (actual != nullptr) {
       for (int i: idsResponsables) {
            if (actual->idUsuario == i) {
                vectorTareasUsuario.push_back  ({actual->idTarea, i});
            }
        }
        
        actual = actual->siguiente;
    }
    return vectorTareasUsuario;
}