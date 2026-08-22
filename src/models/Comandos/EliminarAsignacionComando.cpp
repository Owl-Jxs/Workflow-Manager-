#include "EliminarAsignacionComando.h"

EliminarAsignacionComando::EliminarAsignacionComando(AsignacionController* controller, int idTarea, int idUsuario)
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
