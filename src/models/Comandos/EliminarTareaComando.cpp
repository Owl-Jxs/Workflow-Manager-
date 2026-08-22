#include "EliminarTareaComando.h"

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
