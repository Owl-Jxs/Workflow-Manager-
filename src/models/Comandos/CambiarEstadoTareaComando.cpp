#include "CambiarEstadoTareaComando.h"
#include <stdexcept>

CambiarEstadoTareaComando::CambiarEstadoTareaComando(TareaController* controller, int idTarea, const std::string& nuevoEstado)
    : controller(controller), idTarea(idTarea), nuevoEstado(nuevoEstado), ejecutado(false) {}

void CambiarEstadoTareaComando::ejecutar() {
    Tarea* tarea = controller->buscarTarea(idTarea);
    if (tarea == nullptr) throw std::invalid_argument("No existe una tarea con el id proporcionado");
    if (!ejecutado) estadoAnterior = tarea->getEstado();
    tarea->setEstado(nuevoEstado);
    controller->guardarArchivos();
    ejecutado = true;
}

void CambiarEstadoTareaComando::deshacer() {
    Tarea* tarea = controller->buscarTarea(idTarea);
    if (tarea == nullptr) return;
    tarea->setEstado(estadoAnterior);
    controller->guardarArchivos();
}

std::string CambiarEstadoTareaComando::getAccionAuditoria() const {
    return "CAMBIAR_ESTADO_TAREA";
}

int CambiarEstadoTareaComando::getIdTareaAuditoria() const {
    return idTarea;
}
