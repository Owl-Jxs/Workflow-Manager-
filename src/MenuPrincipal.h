#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
//controllers
#include "Controllers/UsuarioController.h"
#include "Controllers/TareaController/TareaController.h"
#include "Controllers/AsignacionController/AsignacionController.h"
#include "structures/Gestor/GestorHistorial.h"
#include "ui/MenuAdmin/MenuAdmin.h"
//vistas

#include "models/Usuario/Usuario.h"

class MenuPrincipal {
private:
    UsuarioController* uc;
    TareaController* tc;
    AsignacionController* ac;
    GestorHistorial* gestorHistorial;
    MenuAdmin* menuAdmin;
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
