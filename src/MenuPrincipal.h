#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include "Controllers/UsuarioController.h"
#include "Controllers/TareaController/TareaController.h"
#include "Controllers/AsignacionController/AsignacionController.h"
#include "structures/Gestor/GestorHistorial.h"
#include "persistence/AuditoriaDataBase/AuditoriaDataBase.h"
#include "ui/UsuarioView.h"
#include "models/Usuario/Usuario.h"

class MenuPrincipal {
private:
    UsuarioController* uc;
    TareaController* tc;
    AsignacionController* ac;
    UsuarioView* uv;
    AuditoriaDataBase* auditoria;
    GestorHistorial* gestorHistorial;

    void mostrarMenuSegunRol(Usuario* usuario);
    void mostrarMenuAdministrador(Usuario* usuario);
    void mostrarMenuUsuarioNormal(Usuario* usuario);

public:
    MenuPrincipal();
    ~MenuPrincipal();
    void iniciarSesion();
};

#endif
