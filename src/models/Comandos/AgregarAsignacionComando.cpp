#include "AgregarAsignacionComando.h"

AgregarAsignacionComando::AgregarAsignacionComando(AsignacionController* controller, int idTarea, int idUsuario)
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
