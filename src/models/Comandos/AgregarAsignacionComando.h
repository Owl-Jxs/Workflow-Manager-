#ifndef AGREGARASIGNACIONCOMANDO_H
#define AGREGARASIGNACIONCOMANDO_H

#include "IComando.h"
#include "../../Controllers/AsignacionController/AsignacionController.h"

class AgregarAsignacionComando : public IComando {
private:
    AsignacionController* controller;
    int idTarea;
    int idUsuario;
public:
    AgregarAsignacionComando(AsignacionController* controller, int idTarea, int idUsuario);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
