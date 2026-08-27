#ifndef MENUADMIN_H
#define MENUADMIN_H

#include "../../Controllers/UsuarioController.h"
#include "../../Controllers/TareaController/TareaController.h"
#include "../../Controllers/AsignacionController/AsignacionController.h"
#include "../../models/Usuario/Usuario.h"
#include "../../Utilities/ValidarEntrada.h"
#include "../../structures/Gestor/GestorHistorial.h"
#include "../../models/Comandos/ComandoUsuario.h"
#include "ProcesosUsuario/ProcesosUsuario.h"
#include "ProcesosTarea/ProcesosTarea.h"

#include <iostream>
#include <string>

class MenuAdmin
{
private:
    UsuarioController* uc;
    TareaController* tc;
    AsignacionController* ac;
    Usuario* usuarioActivo;
    GestorHistorial* gestorHistorial;
    ProcesosUsuario* procesosUsuarios;
    ProcesosTarea* procesosTareas;
public:
    MenuAdmin(UsuarioController* _uc,  TareaController* _tc, AsignacionController* _ac, Usuario* _uA, GestorHistorial* _gH);
    ~MenuAdmin ();
    void mostrarMenuAdministrador();
    void mostrarMenuGestionUsuarios ();
    void mostrarMenuGestionTareas ();
};


#endif