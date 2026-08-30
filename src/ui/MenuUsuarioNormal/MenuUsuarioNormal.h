#ifndef MENUUSUARIONORMAL_H
#define MENUUSUARIONORMAL_H

#include "../../Controllers/TareaController/TareaController.h"
#include "../../Controllers/AsignacionController/AsignacionController.h"
#include "../../models/Usuario/Usuario.h"
#include "../../structures/Gestor/GestorHistorial.h"
#include "ProcesosTareaNormal/ProcesosTareaNormal.h"

class MenuUsuarioNormal {
private:
    TareaController* tc;
    AsignacionController* ac;
    Usuario* usuarioActivo;
    GestorHistorial* gestorHistorial;
    ProcesosTareaNormal* procesosTareas;

public:
    MenuUsuarioNormal(TareaController* tc, AsignacionController* ac, Usuario* usuario, GestorHistorial* gH);
    ~MenuUsuarioNormal();
    void mostrarMenuUsuarioNormal();
};

#endif
