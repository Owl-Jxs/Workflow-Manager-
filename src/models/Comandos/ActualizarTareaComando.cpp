#include "ActualizarTareaComando.h"
#include <stdexcept>

ActualizarTareaComando::ActualizarTareaComando(
    TareaController* controller, int idTarea,
    const std::string& nuevaDescripcion, bool nuevaPrioridad)
    : controller(controller), idTarea(idTarea),
      nuevaDescripcion(nuevaDescripcion), nuevaPrioridad(nuevaPrioridad),
      ejecutado(false) {}

void ActualizarTareaComando::ejecutar() {
    Tarea* tarea = controller->buscarTarea(idTarea);
    if (tarea == nullptr)
        throw std::invalid_argument("No existe una tarea con el id proporcionado");

    if (!ejecutado) {
        descripcionAnterior = tarea->getDescripcionTarea();
        prioridadAnterior = tarea->getPrioridad();
    }

    tarea->setDescripcionTarea(nuevaDescripcion);
    tarea->setPrioridad(nuevaPrioridad);
    controller->guardarArchivos();
    ejecutado = true;
}

void ActualizarTareaComando::deshacer() {
    Tarea* tarea = controller->buscarTarea(idTarea);
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
