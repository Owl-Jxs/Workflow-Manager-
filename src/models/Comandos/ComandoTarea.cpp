#include "ComandoTarea.h"
#include <stdexcept>
#include <iostream>


// ==========================================
// AgregarTareaComando
// ==========================================

AgregarTareaComando::AgregarTareaComando(TareaController* controller, Tarea* tarea)
    : controller(controller), tarea(tarea) {}

AgregarTareaComando::~AgregarTareaComando() {
    if (tarea != nullptr) delete tarea;
}

void AgregarTareaComando::ejecutar() {
    controller->agregarTarea(tarea);
}

void AgregarTareaComando::deshacer() {
    tarea = controller->eliminarTarea(tarea->getIdTarea());
}

std::string AgregarTareaComando::getAccionAuditoria() const {
    return "AGREGAR_TAREA";
}

int AgregarTareaComando::getIdTareaAuditoria() const {
    return tarea->getIdTarea();
}


// ==========================================
// AsignarResponsableComando
// ==========================================

AsignarResponsableComando::AsignarResponsableComando (TareaController* controller,AsignacionController* ac, int usuaarioId)
    : controller(controller), controllerAsignacion (ac), idUsuario (usuaarioId) {}

void AsignarResponsableComando::ejecutar() {
    idsTarea = controller->delegarTarea ();
    for (int i : idsTarea) {
        controllerAsignacion->agregarAsignacion (i, idUsuario);
    }
}

void AsignarResponsableComando::deshacer() {
    if (!idsTarea.empty()) {
        controller->deshacerDelegacion (idsTarea[0]);
        for (int i : idsTarea) {
            controllerAsignacion->eliminarAsignacion (i, idUsuario);
        }
    }
}

std::string AsignarResponsableComando::getAccionAuditoria() const {
    return "AGREGAR_RESPONSABLE";
}

int AsignarResponsableComando::getIdTareaAuditoria() const {
    return idsTarea.empty() ? -1 : idsTarea[0];
}


// ==========================================
// ActualizarTareaComando
// ==========================================

ActualizarTareaComando::ActualizarTareaComando (TareaController* controller, int idTarea,const std::string& nuevaDescripcion, bool nuevaPrioridad)
 : controller(controller), idTarea(idTarea), nuevaDescripcion(nuevaDescripcion), nuevaPrioridad(nuevaPrioridad), ejecutado(false) {}

void ActualizarTareaComando::ejecutar() {
    Tarea* tarea = controller->buscarTareaPorHacer (idTarea);
    if (tarea == nullptr) tarea = controller->buscarTareaEnProceso (idTarea);
    if (tarea == nullptr) throw std::invalid_argument("No existe una tarea con el id proporcionado");

    if (!ejecutado) {
        descripcionAnterior = tarea->getDescripcionTarea();
        prioridadAnterior = tarea->getPrioridad();
    }

    tarea->setDescripcionTarea(nuevaDescripcion);   tarea->setPrioridad(nuevaPrioridad);
    controller->guardarArchivos();      ejecutado = true;
}

void ActualizarTareaComando::deshacer() {
    Tarea* tarea = controller->buscarTareaPorHacer (idTarea);
    if (tarea == nullptr) tarea = controller->buscarTareaEnProceso (idTarea);
    if (tarea == nullptr) return;

    tarea->setDescripcionTarea(descripcionAnterior);
    tarea->setPrioridad(prioridadAnterior);
    controller->guardarArchivos();
}

std::string ActualizarTareaComando::getAccionAuditoria() const {
    return "ACTUALIZAR_TAREA";
}

int ActualizarTareaComando::getIdTareaAuditoria() const {
    return idTarea;
}

// ==========================================
// EliminarTareaComando
// ==========================================

EliminarTareaComando::EliminarTareaComando(TareaController* controller,AsignacionController* ac, int idTarea)
    : controller(controller), asignacionController (ac), idTarea(idTarea), tareaGuardada(nullptr), ejecutado (false) {}

EliminarTareaComando::~EliminarTareaComando() {
    if (tareaGuardada != nullptr && ejecutado) delete tareaGuardada;
}

void EliminarTareaComando::ejecutar() {
    Tarea* tarea = controller->buscarTareaPorHacer(idTarea);
    if (tarea == nullptr) tarea = controller->buscarTareaEnProceso(idTarea);
    if (tarea == nullptr) tarea = controller->buscarTareaEnRevision(idTarea);
    if (tarea == nullptr) tarea = controller->buscarTareaCompletada(idTarea);
    
    std::vector<int> idsTarea;
    if (tarea != nullptr) {
        idsTarea = controller->listarIdsArbol(tarea);
        Asignaciones_Tarea_Usuario = asignacionController->getAsignacionesResponsablesDeTarea(idsTarea);
    }
    
    tareaGuardada = controller->eliminarTarea(idTarea);

    for (std::pair <int, int> i :Asignaciones_Tarea_Usuario) {
    int idTarea = i.first;
    int idUsuario = i.second;
    asignacionController->eliminarAsignacion (idTarea, idUsuario);
    }

    ejecutado = true;
}

void EliminarTareaComando::deshacer() {
    if (tareaGuardada != nullptr) {
        ejecutado = false;
        controller->agregarTarea(tareaGuardada);
        tareaGuardada = nullptr;

        for (std::pair <int, int> i : Asignaciones_Tarea_Usuario){
            int idTarea = i.first;
            int idUsuario = i.second;
            asignacionController->agregarAsignacion (idTarea, idUsuario);   
        }
    }

}

std::string EliminarTareaComando::getAccionAuditoria() const {
    return "ELIMINAR_TAREA";
}

int EliminarTareaComando::getIdTareaAuditoria() const {
    return idTarea;
}


// ==========================================
// validarTareaEnRevisionComando
// ==========================================

validarTareaEnRevisionComando::validarTareaEnRevisionComando (TareaController* controller,AsignacionController* ac, Tarea* tarea)
    : controller(controller), asignacionController (ac), tareaGuardada (tarea), ejecutado (false) {}

validarTareaEnRevisionComando::~validarTareaEnRevisionComando() {
    if (tareaGuardada != nullptr && ejecutado) delete tareaGuardada;
}

void validarTareaEnRevisionComando::ejecutar() {
    controller->revisionExitosa ();
    std::vector<int> idsTarea = controller->listarIdsArbol (tareaGuardada);
    //idTarea, idsuario
    Asignaciones_Tarea_Usuario = asignacionController->getAsignacionesResponsablesDeTarea (idsTarea);
    asignacionController->guardarAsignacionesCompletadas (Asignaciones_Tarea_Usuario);
    ejecutado = true;
}

void validarTareaEnRevisionComando::deshacer() {
    if (tareaGuardada != nullptr) {
        ejecutado = false;
        controller->agregarTarea(tareaGuardada);
        tareaGuardada = nullptr;
        asignacionController->deshacerAsignacionesCompletadas (Asignaciones_Tarea_Usuario);
    }

}

std::string validarTareaEnRevisionComando::getAccionAuditoria() const {
    return "ACEPTAR_TAREA_COMPLETADA";
}

int validarTareaEnRevisionComando::getIdTareaAuditoria() const {
    return tareaGuardada->getIdTarea ();
}


// ========================================
// RechazarTareaEnRevisionComando
// ==========================================

RechazarTareaEnRevisionComando::RechazarTareaEnRevisionComando (TareaController* controller,AsignacionController* ac, Tarea* tarea)
    : controller(controller), asignacionController (ac), tareaGuardada (tarea), ejecutado (false), numDeshacer (0) {}

RechazarTareaEnRevisionComando::~RechazarTareaEnRevisionComando() {
    if (tareaGuardada != nullptr && ejecutado) delete tareaGuardada;
}

void RechazarTareaEnRevisionComando::ejecutar() {
    if (!ejecutado){
        controller->anularRevisionExitosa (tareaGuardada->getIdTarea ());
        ejecutado = true;
    } 
    else {
        std::cout << "LA tarea ya ingreso en la cola de revision"  << std::endl;
    }
}

void RechazarTareaEnRevisionComando::deshacer() {
    if (numDeshacer < 1){
        controller->mandar_A_Revision (tareaGuardada->getIdTarea ());
        numDeshacer++;
    } else {
        std::cout << "La tarea esta ingresada en una posicion mas atras en la cola de revision"  << std::endl;
    }
    
}

std::string RechazarTareaEnRevisionComando::getAccionAuditoria() const {
    return "RECHAZAR_TAREA_COMPLETADA";
}

int RechazarTareaEnRevisionComando::getIdTareaAuditoria() const {
    return tareaGuardada->getIdTarea ();
}


// ==========================================
// MandarARevisionComando
// ==========================================

MandarARevisionComando::MandarARevisionComando(TareaController* controller, int idTarea)
    : controller(controller), idTarea(idTarea), ejecutado(false) {}

void MandarARevisionComando::ejecutar() {
    if (!ejecutado) {
        controller->mandar_A_Revision(idTarea);
        ejecutado = true;
    } else {
        std::cout << "La tarea ya fue enviada a revision" << std::endl;
    }
}

void MandarARevisionComando::deshacer() {
    if (ejecutado) {
        controller->anularRechazo(idTarea);
        ejecutado = false;
    }
}

std::string MandarARevisionComando::getAccionAuditoria() const {
    return "ENVIAR_A_REVISION";
}

int MandarARevisionComando::getIdTareaAuditoria() const {
    return idTarea;
}

