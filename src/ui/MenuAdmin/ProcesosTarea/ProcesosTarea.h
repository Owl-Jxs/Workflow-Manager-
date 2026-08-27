#ifndef PROCESOSTAREA_H
#define PROCESOSTAREA_H

#include "../../../Controllers/UsuarioController.h"
#include "../../../Controllers/TareaController/TareaController.h"
#include "../../../Controllers/AsignacionController/AsignacionController.h"
#include "../../../models/Usuario/Usuario.h"
#include "../../../Utilities/ValidarEntrada.h"
#include "../../../structures/Gestor/GestorHistorial.h"
#include "../../../models/Comandos/ComandoTarea.h"
#include "../../../models/Comandos/ComandoAsignacion.h"
#include "../ProcesosUsuario/ProcesosUsuario.h"

#include <exception>
#include <iostream>
class ProcesosTarea {
private:
    UsuarioController* uc;
    TareaController* tc;
    AsignacionController* ac;
    Usuario* usuarioActivo;
    GestorHistorial* gestorHistorial;

    Tarea* leerNuevaTarea ();
    Tarea* leerSubTarea ();
    void mostrarInformacionTarea (Tarea* tarea);
    void asignarResponsable (int idTarea);

public:
    ProcesosTarea (UsuarioController* _uc,  TareaController* _tc, AsignacionController* _ac, Usuario* _uA, GestorHistorial* _gH);

    void agregarTarea ();
    void agregarSubTarea ();
    void ActualizarTarea ();
    void eliminarTarea ();
    void mostrarTableroKanban ();
    void mostrarTarea ();
    void ordenarLista ();
    void listarArbolTarea ();
    //void buscarTareaPorId (); implementar cuando esten los 3 algoritmos 
};

#endif