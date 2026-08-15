#ifndef VIEWTAREAS_H
#define VIEWTAREAS_H
#include "../../Controllers/TareaController/TareaController.h"
#include "../../models/Tarea/Tarea.h"
#include "../../structures/ColaFIFO/colaFIFO.h"
#include "../../structures/ColaFIFO/NodoTarea/NodoTarea.h"

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