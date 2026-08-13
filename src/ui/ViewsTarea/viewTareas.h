#ifndef VIEWTAREAS_H
#define VIEWTAREAS_H
#include "TareaController.h"
#include "Tarea.h"
#include "ColaFIFO.h"
#include "nodoTarea.h"

class ViewTareas {
private:
    TareaController* tc;
    Tarea* buscarTarea(int idTarea);//busca una tarea por ID en las dos colas
    void mostrarSubTareas(Tarea* tarea, int nivel);

public:
    ViewTareas(TareaController* tc);
    void mostrarTableroKanban();
    void crearTarea();
    void crearSubTarea();
    void mostrarTarea(Tarea* tarea);
};

#endif