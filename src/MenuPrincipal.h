#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
//controllers
#include "Controllers/UsuarioController.h"
#include "Controllers/TareaController/TareaController.h"
#include "Controllers/AsignacionController/AsignacionController.h"
#include "structures/Gestor/GestorHistorial.h"

//vistas
#include "ui/UsuarioView/UsuarioView.h"
#include "ui/ViewsTarea/viewTareas.h"

#include "models/Usuario/Usuario.h"

class MenuPrincipal {
private:
    UsuarioController* uc;
    TareaController* tc;
    AsignacionController* ac;
    GestorHistorial* gestorHistorial;

    UsuarioView* uv;
    ViewTareas* tv;

    Usuario* usuarioActivo;

    void crearPrimerUsuario ();
    void iniciarSesion();
    void mostrarMenuAdministrador(Usuario* usuario);
    void mostrarMenuUsuarioNormal(Usuario* usuario);

public:
    MenuPrincipal();
    ~MenuPrincipal();
    void ejecutar ();
    
};

#endif
