#include "AgregarTareaComando.h"

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
