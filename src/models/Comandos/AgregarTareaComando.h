#ifndef AGREGARTAREACOMANDO_H
#define AGREGARTAREACOMANDO_H

#include "IComando.h"
#include "../../models/Tarea/Tarea.h"
#include "../../Controllers/TareaController/TareaController.h"

class AgregarTareaComando : public IComando {
private:
    TareaController* controller;
    Tarea* tarea;
    bool urgente;
public:
    AgregarTareaComando(TareaController* controller, Tarea* tarea, bool urgente);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
