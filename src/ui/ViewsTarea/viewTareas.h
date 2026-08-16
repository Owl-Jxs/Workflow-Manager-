#ifndef VIEWTAREAS_H
#define VIEWTAREAS_H
#include <vector>
#include "Tarea.h"
#include "ColaFIFO.h"

class ViewTareas {

private:
    void mostrarTarea(Tarea* tarea);
    void mostrarSubTareas(Tarea* tarea);

public:
    ViewTareas();
    void mostrarTableroKanban(ColaFIFO* listaUrgente, ColaFIFO* listaRegular);

};

#endif