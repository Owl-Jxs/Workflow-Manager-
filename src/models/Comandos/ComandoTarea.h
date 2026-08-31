#ifndef COMANDOTAREA_H
#define COMANDOTAREA_H

#include "IComando.h"
#include "../../models/Tarea/Tarea.h"
#include "../../Controllers/TareaController/TareaController.h"
#include "../../Controllers/AsignacionController/AsignacionController.h"
#include <string>
#include <vector>
#include <utility>

// ==========================================
// AgregarTareaComando
// ==========================================
class AgregarTareaComando : public IComando {
private:
    TareaController* controller;
    Tarea* tarea;
    bool ejecutado;
    int idTareaGuardado;
public:
    AgregarTareaComando(TareaController* controller, Tarea* tarea);
    ~AgregarTareaComando() override;
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
    bool nuevaPrioridad;
    bool prioridadAnterior;
    bool ejecutado;
public:
    ActualizarTareaComando(TareaController* controller, int idTarea, const std::string& nuevaDescripcion, bool nuevaPrioridad);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

// ==========================================
// AsignarResponsableComando
// ==========================================
class AsignarResponsableComando : public IComando {
private:
    TareaController* controller;
    AsignacionController* controllerAsignacion;
    std::vector <int> idsTarea;
    int idUsuario;
public:
    AsignarResponsableComando (TareaController* controller, AsignacionController* ac, int idUsuario);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria () const override;
};


// ==========================================
// EliminarTareaComando
// ==========================================
class EliminarTareaComando : public IComando {
private:
    TareaController* controller;
    AsignacionController* asignacionController;
    int idTarea;
    std::vector<std::pair <int, int>> Asignaciones_Tarea_Usuario;
    Tarea* tareaGuardada;
    bool ejecutado;
public:
    EliminarTareaComando(TareaController* controller, AsignacionController* ac, int idTarea);
    ~EliminarTareaComando() override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

// ==========================================
// validarTareaEnRevisionComando
// ==========================================
class validarTareaEnRevisionComando : public IComando {
private:
    TareaController* controller;
    AsignacionController* asignacionController;
    std::vector<std::pair <int, int>> Asignaciones_Tarea_Usuario;
    Tarea* tareaGuardada; // non-owning pointer, owned by controller
    int idTareaGuardada;
    bool ejecutado;
public:
    validarTareaEnRevisionComando (TareaController* controller, AsignacionController* ac, Tarea* tarea);
    ~validarTareaEnRevisionComando () override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};


// ==========================================
// RechazarTareaEnRevisionComando
// ==========================================
class RechazarTareaEnRevisionComando : public IComando {
private:
    TareaController* controller;
    AsignacionController* asignacionController;
    Tarea* tareaGuardada; // non-owning pointer
    int idTareaGuardada;
    bool ejecutado;
public:
    RechazarTareaEnRevisionComando (TareaController* controller, AsignacionController* ac, Tarea* tareaGuardada);
    ~RechazarTareaEnRevisionComando() override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};


// ==========================================
// MandarARevisionComando
// ==========================================
class MandarARevisionComando : public IComando {
private:
    TareaController* controller;
    int idTarea;
    bool ejecutado;
public:
    MandarARevisionComando(TareaController* controller, int idTarea);
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};


#endif
