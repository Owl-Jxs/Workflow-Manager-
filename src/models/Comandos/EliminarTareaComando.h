#ifndef ELIMINARTAREACOMANDO_H
#define ELIMINARTAREACOMANDO_H

#include "IComando.h"
#include "../../models/Tarea/Tarea.h"
#include "../../Controllers/TareaController/TareaController.h"

class EliminarTareaComando : public IComando {
private:
    TareaController* controller;
    int idTarea;
    bool urgente;
    Tarea* tareaGuardada; // tarea extraida, para poder rehacer
public:
    EliminarTareaComando(TareaController* controller, int idTarea, bool urgente);
    ~EliminarTareaComando() override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
