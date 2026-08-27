#include "CondicionTarea.h"

bool CondicionTarea::comparar(const Tarea& tarea1, const Tarea& tarea2) const {
    return tarea1.getIdTarea() < tarea2.getIdTarea();
}