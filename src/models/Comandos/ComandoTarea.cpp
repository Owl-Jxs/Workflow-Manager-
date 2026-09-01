#include "ComandoTarea.h"
#include <stdexcept>
#include <iostream>


// ==========================================
// AgregarTareaComando
// ==========================================

AgregarTareaComando::AgregarTareaComando(TareaController* controller, Tarea* tarea)
    : controller(controller), tarea(tarea), ejecutado(false), idTareaGuardado(tarea ? tarea->getIdTarea() : -1) {}

AgregarTareaComando::~AgregarTareaComando() {
    if (tarea != nullptr && !ejecutado) delete tarea;
}

void AgregarTareaComando::ejecutar() {
    if (ejecutado) return;
    controller->agregarTarea(tarea);
    idTareaGuardado = tarea->getIdTarea();
    ejecutado = true;
}

void AgregarTareaComando::deshacer() {
    if (!ejecutado) return;
    tarea = controller->eliminarTarea(idTareaGuardado);
    ejecutado = false;
}

std::string AgregarTareaComando::getAccionAuditoria() const {
    return "AGREGAR_TAREA";
}

int AgregarTareaComando::getIdTareaAuditoria() const {
    if (tarea != nullptr) return tarea->getIdTarea();
    return idTareaGuardado;
}


// ==========================================
// AsignarResponsableComando
// ==========================================

AsignarResponsableComando::AsignarResponsableComando (TareaController* controller,AsignacionController* ac, int usuaarioId)
    : controller(controller), controllerAsignacion (ac), idUsuario (usuaarioId), ejecutado(false) {}

void AsignarResponsableComando::ejecutar() {
    if (ejecutado) return;
    idsTarea = controller->delegarTarea ();
    try {
        for (int i : idsTarea) {
            controllerAsignacion->agregarAsignacion (i, idUsuario);
        }
        ejecutado = true;
    } catch (...) {
        // rollback parcial
        for (int i : idsTarea) {
            try { controllerAsignacion->eliminarAsignacion(i, idUsuario); } catch(...) {}
        }
        try { controller->deshacerDelegacion(idsTarea[0]); } catch(...) {}
        idsTarea.clear();
        throw;
    }
}

void AsignarResponsableComando::deshacer() {
    if (!ejecutado || idsTarea.empty()) return;
    controller->deshacerDelegacion (idsTarea[0]);
    for (int i : idsTarea) {
        try { controllerAsignacion->eliminarAsignacion (i, idUsuario); } catch(...) {}
    }
    ejecutado = false;
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
    : controller(controller), asignacionController (ac), tareaGuardada (tarea), idTareaGuardada(tarea ? tarea->getIdTarea() : -1), ejecutado (false) {}

validarTareaEnRevisionComando::~validarTareaEnRevisionComando() {
    // Tarea is owned by controller, do not delete
}

void validarTareaEnRevisionComando::ejecutar() {
    if (ejecutado) return;
    // capture ids before moving, tareaGuardada points to front task still in enRevision
    std::vector<int> idsTarea = controller->listarIdsArbol (tareaGuardada);
    controller->revisionExitosa ();
    // after revisionExitosa the task is in completadas; update id in case object moved
    if (tareaGuardada != nullptr) idTareaGuardada = tareaGuardada->getIdTarea();
    Asignaciones_Tarea_Usuario = asignacionController->getAsignacionesResponsablesDeTarea (idsTarea);
    asignacionController->guardarAsignacionesCompletadas (Asignaciones_Tarea_Usuario);
    ejecutado = true;
}

void validarTareaEnRevisionComando::deshacer() {
    if (!ejecutado) return;
    controller->anularRevisionExitosa(idTareaGuardada);
    asignacionController->deshacerAsignacionesCompletadas (Asignaciones_Tarea_Usuario);
    ejecutado = false;
}

std::string validarTareaEnRevisionComando::getAccionAuditoria() const {
    return "ACEPTAR_TAREA_COMPLETADA";
}

int validarTareaEnRevisionComando::getIdTareaAuditoria() const {
    return idTareaGuardada;
}


// ========================================
// RechazarTareaEnRevisionComando
// ==========================================

RechazarTareaEnRevisionComando::RechazarTareaEnRevisionComando (TareaController* controller,AsignacionController* ac, Tarea* tarea)
    : controller(controller), asignacionController (ac), tareaGuardada (tarea), idTareaGuardada(tarea ? tarea->getIdTarea() : -1), ejecutado (false) {}

RechazarTareaEnRevisionComando::~RechazarTareaEnRevisionComando() {
    // Tarea is owned by controller, do not delete
}

void RechazarTareaEnRevisionComando::ejecutar() {
    if (ejecutado) {
        std::cout << "La tarea ya fue rechazada" << std::endl;
        return;
    }
    controller->rechazarRevision();
    ejecutado = true;
}

void RechazarTareaEnRevisionComando::deshacer() {
    if (!ejecutado) return;
    controller->anularRechazo(idTareaGuardada);
    ejecutado = false;
}

std::string RechazarTareaEnRevisionComando::getAccionAuditoria() const {
    return "RECHAZAR_TAREA_COMPLETADA";
}

int RechazarTareaEnRevisionComando::getIdTareaAuditoria() const {
    return idTareaGuardada;
}


// ==========================================
// MandarARevisionComando
// ==========================================

MandarARevisionComando::MandarARevisionComando(TareaController* controller, int idTarea)
    : controller(controller), idTarea(idTarea), ejecutado(false) {}

void MandarARevisionComando::ejecutar() {
    if (ejecutado) return;
    controller->mandar_A_Revision(idTarea);
    ejecutado = true;
}

void MandarARevisionComando::deshacer() {
    if (!ejecutado) return;
    controller->deshacerMandarARevision(idTarea);
    ejecutado = false;
}

std::string MandarARevisionComando::getAccionAuditoria() const {
    return "ENVIAR_A_REVISION";
}

int MandarARevisionComando::getIdTareaAuditoria() const {
    return idTarea;
}

