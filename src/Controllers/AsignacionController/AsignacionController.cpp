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

//                                  === === ===     METODOS PUBLICOS    === === ===

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

void AsignacionController:: agregarAsignacion(int idTarea, int idUsuario) {
    if (idTarea < 0) throw std::invalid_argument ("el id de la tarea no puede ser negativo ");
    if (idUsuario < 0) throw std::invalid_argument ("el id del usuario no puede ser negativo ");
    if (listaAsignaciones->buscarAsignacion (idTarea, idUsuario) ) throw std::invalid_argument ("la asignacion ya existe ");

    listaAsignaciones->agregarAsignacion (idTarea, idUsuario);
    archivosAsignaciones->agregarAsignacion (idTarea, idUsuario);

}

void AsignacionController::eliminarAsignacion(int idTarea, int idUsuario) {
    if (idTarea < 0) throw std::invalid_argument ("el id de la tarea no puede ser negativo ");
    if (idUsuario < 0) throw std::invalid_argument ("el id del usuario no puede ser negativo ");
    if (! (listaAsignaciones->buscarAsignacion (idTarea, idUsuario) ) ) throw std::invalid_argument ("la asignacion no existe ");

    listaAsignaciones->eliminarAsignacion (idTarea, idUsuario);
    archivosAsignaciones->eliminarAsignacion (idTarea, idUsuario);
}

bool AsignacionController::buscarAsignacion(int idTarea, int idUsuario) {
    return listaAsignaciones->buscarAsignacion (idTarea, idUsuario);
}

 std::vector<int> AsignacionController::getAsignacionesResponsablesDeTarea (int idTarea) {
    NodoAsignacion* actual = listaAsignaciones->getCabeza ();
    std::vector <int> vectorUsuariosResponsables;

    while (actual != nullptr) {
        if (actual->idTarea == idTarea) {
            vectorUsuariosResponsables.push_back (actual->idUsuario);
        }
        actual = actual->siguiente;
    }
    return vectorUsuariosResponsables;
}

std::vector<int> AsignacionController::getAsignacionesTareasPorUsuario (int idUsuario) {
    NodoAsignacion* actual = listaAsignaciones->getCabeza ();
    std::vector <int> vectorTareasUsuario;

    while (actual != nullptr) {
        if (actual->idUsuario == idUsuario) {
            vectorTareasUsuario.push_back (actual->idTarea);
        }
        actual = actual->siguiente;
    }
    return vectorTareasUsuario;
}