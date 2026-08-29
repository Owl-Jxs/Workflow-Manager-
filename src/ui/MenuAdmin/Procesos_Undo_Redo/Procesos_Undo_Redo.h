#ifndef PROCESOS_UNDO_REDO_H
#define PROCESOS_UNDO_REDO_H
#include "../../../structures/Gestor/GestorHistorial.h"

#include <iostream>

class Procesos_Undo_Redo {
private:
    GestorHistorial* gc;

public:
Procesos_Undo_Redo (GestorHistorial* gestor);

void rehacer ();
void deshacer ();

};





#endif