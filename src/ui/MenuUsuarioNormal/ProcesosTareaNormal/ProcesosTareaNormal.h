#ifndef PROCESOSTAREANORMAL_H
#define PROCESOSTAREANORMAL_H

#include "../../../Controllers/TareaController/TareaController.h"
#include "../../../Controllers/AsignacionController/AsignacionController.h"
#include "../../../models/Usuario/Usuario.h"
#include "../../../Utilities/ValidarEntrada.h"
#include "../../../structures/Gestor/GestorHistorial.h"
#include "../../../models/Comandos/ComandoTarea.h"

#include <iostream>
#include <string>

class ProcesosTareaNormal {
private:
    TareaController* tc;
    AsignacionController* ac;
    Usuario* usuarioActivo;
    GestorHistorial* gestorHistorial;

    void mostrarInformacionTarea(Tarea* tarea, bool mostrarSubTareas, int nivel);

public:
    ProcesosTareaNormal(TareaController* tc, AsignacionController* ac, Usuario* usuario, GestorHistorial* gH);

    void verMisTareasAsignadas();
    void verDetallesTarea();
    void enviarTareaARevision();
    void verTableroKanban();
};

#endif
