#ifndef CAMBIARESTADOTAREACOMANDO_H
#define CAMBIARESTADOTAREACOMANDO_H

#include <string>
#include "IComando.h"
#include "../../models/Tarea/Tarea.h"
#include "../../Controllers/TareaController/TareaController.h"

class CambiarEstadoTareaComando : public IComando {
private:
    TareaController* controller;
    int idTarea;
    std::string nuevoEstado;
    std::string estadoAnterior;
    bool ejecutado;
public:
    CambiarEstadoTareaComando(TareaController* controller, int idTarea, const std::string& nuevoEstado);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
