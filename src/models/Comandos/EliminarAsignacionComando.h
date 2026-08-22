#ifndef ELIMINARASIGNACIONCOMANDO_H
#define ELIMINARASIGNACIONCOMANDO_H

#include "IComando.h"
#include "../../Controllers/AsignacionController/AsignacionController.h"

class EliminarAsignacionComando : public IComando {
private:
    AsignacionController* controller;
    int idTarea;
    int idUsuario;
public:
    EliminarAsignacionComando(AsignacionController* controller, int idTarea, int idUsuario);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
