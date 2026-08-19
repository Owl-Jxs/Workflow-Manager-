#ifndef VIEWTAREAS_H
#define VIEWTAREAS_H
#include <vector>
#include "../../models/Tarea/Tarea.h"
#include "../../structures/ColaFIFO/colaFIFO.h"

class ViewTareas {

private:
    void mostrarTarea(Tarea* tarea);
    void mostrarSubTareas(Tarea* tarea);

public:
    ViewTareas();
    void mostrarTableroKanban(ColaFIFO* listaUrgente, ColaFIFO* listaRegular);

};

#endif
