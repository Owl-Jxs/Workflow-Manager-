#include "Procesos_Undo_Redo.h"

Procesos_Undo_Redo::Procesos_Undo_Redo (GestorHistorial* gestor) {
    this->gc = gestor;
}

void Procesos_Undo_Redo::rehacer () {
    try {
    gc->rehacer ();
    } catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
}

void Procesos_Undo_Redo::deshacer () {
    try {
    gc->deshacer ();
    } catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }   
}
