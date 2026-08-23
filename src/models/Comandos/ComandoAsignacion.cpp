#include "ComandoAsignacion.h"

// ==========================================
// AgregarAsignacionComando
// ==========================================

AgregarAsignacionComando::AgregarAsignacionComando(
    AsignacionController* controller, int idTarea, int idUsuario)
    : controller(controller), idTarea(idTarea), idUsuario(idUsuario) {}

void AgregarAsignacionComando::ejecutar() {
    controller->agregarAsignacion(idTarea, idUsuario);
}

void AgregarAsignacionComando::deshacer() {
    controller->eliminarAsignacion(idTarea, idUsuario);
}

std::string AgregarAsignacionComando::getAccionAuditoria() const {
    return "AGREGAR_ASIGNACION";
}

int AgregarAsignacionComando::getIdTareaAuditoria() const {
    return idTarea;
}

// ==========================================
// EliminarAsignacionComando
// ==========================================

EliminarAsignacionComando::EliminarAsignacionComando(
    AsignacionController* controller, int idTarea, int idUsuario)
    : controller(controller), idTarea(idTarea), idUsuario(idUsuario) {}

void EliminarAsignacionComando::ejecutar() {
    controller->eliminarAsignacion(idTarea, idUsuario);
}

void EliminarAsignacionComando::deshacer() {
    controller->agregarAsignacion(idTarea, idUsuario);
}

std::string EliminarAsignacionComando::getAccionAuditoria() const {
    return "ELIMINAR_ASIGNACION";
}

int EliminarAsignacionComando::getIdTareaAuditoria() const {
    return idTarea;
}
