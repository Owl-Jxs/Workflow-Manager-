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
