#ifndef TAREACONTROLLER_H
#define TAREACONTROLLER_H

#include "../../persistence/TareaDataBase/TareaDataBase.h"
#include "../../structures/ColaFIFO/ColaFIFO.h"

class TareaController {
private:
    ColaFIFO* listaTareasRegulares; // Lista de tareas regulares
    ColaFIFO* listaTareasUrgentes; // Lista de tareas prioritarias
    TareaDataBase* archivosTareas; //la data base de tareas

public:
    TareaController ();
    ~TareaController ();
//guardar/cargar todos los archivos
    void guardarArchivos ();
    void cargarArchivos ();
//funciones para manipular las tareas de la lista
    void agregarTarea(Tarea* tarea, bool perteneceListaUrgente);
    void agregarSubTarea (Tarea* tarea, int idTareaPadre, bool perteneceListaUrgente);
    //void completarTarea (bool listaUrgente); <--- Preguntar al profe si esas tareas se eliminan
    // void descompletarTarea (int idTarea); <--- Preguntar al profe sobre su gestion
    //void completarSubtarea 
    //void descompletarSubTarea
    //void completarTarea ();
};

#endif