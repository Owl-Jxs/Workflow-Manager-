#include "ComandoTarea.h"
#include <stdexcept>

// ==========================================
// AgregarTareaComando
// ==========================================

AgregarTareaComando::AgregarTareaComando(TareaController* controller, Tarea* tarea, bool urgente)
    : controller(controller), tarea(tarea), urgente(urgente) {}

void AgregarTareaComando::ejecutar() {
    controller->agregarTarea(tarea, urgente);
}

void AgregarTareaComando::deshacer() {
    controller->eliminarTarea(tarea->getIdTarea(), urgente);
}

std::string AgregarTareaComando::getAccionAuditoria() const {
    return "AGREGAR_TAREA";
}

int AgregarTareaComando::getIdTareaAuditoria() const {
    return tarea->getIdTarea();
}

// ==========================================
// ActualizarTareaComando
// ==========================================

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

// ==========================================
// EliminarTareaComando
// ==========================================

EliminarTareaComando::EliminarTareaComando(TareaController* controller, int idTarea, bool urgente)
    : controller(controller), idTarea(idTarea), urgente(urgente), tareaGuardada(nullptr) {}

EliminarTareaComando::~EliminarTareaComando() {
    if (tareaGuardada != nullptr) delete tareaGuardada;
}

void EliminarTareaComando::ejecutar() {
    tareaGuardada = controller->eliminarTarea(idTarea, urgente);
}

void EliminarTareaComando::deshacer() {
    if (tareaGuardada != nullptr) {
        controller->agregarTarea(tareaGuardada, urgente);
        tareaGuardada = nullptr;
    }
}

std::string EliminarTareaComando::getAccionAuditoria() const {
    return "ELIMINAR_TAREA";
}

int EliminarTareaComando::getIdTareaAuditoria() const {
    return idTarea;
}

// ==========================================
// CambiarEstadoTareaComando
// ==========================================

CambiarEstadoTareaComando::CambiarEstadoTareaComando(
    TareaController* controller, int idTarea, const std::string& nuevoEstado)
    : controller(controller), idTarea(idTarea), nuevoEstado(nuevoEstado), ejecutado(false) {}

void CambiarEstadoTareaComando::ejecutar() {
    Tarea* tarea = controller->buscarTarea(idTarea);
    if (tarea == nullptr)
        throw std::invalid_argument("No existe una tarea con el id proporcionado");
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
