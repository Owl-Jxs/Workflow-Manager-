#ifndef COMANDOASIGNACION_H
#define COMANDOASIGNACION_H

#include "IComando.h"
#include "../../Controllers/AsignacionController/AsignacionController.h"

// ==========================================
// AgregarAsignacionComando
// ==========================================
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

// ==========================================
// EliminarAsignacionComando
// ==========================================
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
