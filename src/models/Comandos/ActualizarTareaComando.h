#ifndef ACTUALIZARTAREACOMANDO_H
#define ACTUALIZARTAREACOMANDO_H

#include "IComando.h"
#include "../../models/Tarea/Tarea.h"
#include "../../Controllers/TareaController/TareaController.h"
#include <string>

class ActualizarTareaComando : public IComando {
private:
    TareaController* controller;
    int idTarea;
    std::string nuevaDescripcion;
    std::string descripcionAnterior;
    bool nuevaPrioridad;
    bool prioridadAnterior;
    bool ejecutado;
public:
    ActualizarTareaComando(TareaController* controller, int idTarea,
                           const std::string& nuevaDescripcion, bool nuevaPrioridad);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
