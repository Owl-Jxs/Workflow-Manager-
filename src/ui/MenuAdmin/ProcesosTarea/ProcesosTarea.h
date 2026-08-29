#ifndef PROCESOSTAREA_H
#define PROCESOSTAREA_H

#include "../../../Controllers/UsuarioController.h"
#include "../../../Controllers/TareaController/TareaController.h"
#include "../../../Controllers/AsignacionController/AsignacionController.h"
#include "../../../models/Usuario/Usuario.h"
#include "../../../Utilities/ValidarEntrada.h"
#include "../../../structures/Gestor/GestorHistorial.h"
#include "../../../models/Comandos/ComandoTarea.h"
#include "../../../algorithms/Condiciones/Icondicion.h"
#include "../../../algorithms/Condiciones/TareaCondicion.h"
#include "../../../algorithms/MergeSort/MergeSort.h"
#include "../../../algorithms/QuickSort/QuickSort.h"

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
    void mostrarInformacionTarea (Tarea* tarea, bool mostrarInfoSubTareas, int nivel);

public:
    ProcesosTarea (UsuarioController* _uc,  TareaController* _tc, AsignacionController* _ac, Usuario* _uA, GestorHistorial* _gH);
    
    void agregarTarea ();
    void agregarSubTarea ();
    void ActualizarTarea ();
    void asignarResponsable ();
    void verificarTareaEnColaRevision ();
    void eliminarTarea ();
    void mostrarTableroKanban ();
    void mostrarTareaPorId ();
    void ordenarLista ();
};

#endif