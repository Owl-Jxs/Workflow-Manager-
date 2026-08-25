#ifndef COMANDOTAREA_H
#define COMANDOTAREA_H

#include "IComando.h"
#include "../../models/Tarea/Tarea.h"
#include "../../Controllers/TareaController/TareaController.h"
#include <string>

// ==========================================
// AgregarTareaComando
// ==========================================
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

// ==========================================
// AgregarSubTareaComando
// ==========================================
class AgregarSubTareaComando : public IComando {
private:
    TareaController* controller;
    int idPadre;
    Tarea* tarea;
    bool urgente;
public:
    AgregarSubTareaComando (TareaController* controller, int idPadre, Tarea* tarea, bool urgente);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};


// ==========================================
// ActualizarTareaComando
// ==========================================
class ActualizarTareaComando : public IComando {
private:
    TareaController* controller;
    int idTarea;
    std::string nuevaDescripcion;
    std::string descripcionAnterior;
    std::string estadoAnterior;
    std::string nuevoEstado;
    bool nuevaPrioridad;
    bool prioridadAnterior;
    bool ejecutado;
public:
    ActualizarTareaComando(TareaController* controller, int idTarea,
                           const std::string& nuevaDescripcion, bool nuevaPrioridad, std::string nuevoEstado);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

// ==========================================
// EliminarTareaComando
// ==========================================
class EliminarTareaComando : public IComando {
private:
    TareaController* controller;
    int idTarea;
    bool urgente;
    Tarea* tareaGuardada;
public:
    EliminarTareaComando(TareaController* controller, int idTarea, bool urgente);
    ~EliminarTareaComando() override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

// ==========================================
// CambiarEstadoTareaComando
// ==========================================
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
