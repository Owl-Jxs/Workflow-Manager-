#ifndef CONDICIONTAREA_H
#define CONDICIONTAREA_H
#include "ICondicion.h"
#include "../../models/Tarea/Tarea.h"

class CondicionTarea : public ICondicion<Tarea> {
public:
    bool comparar(const Tarea& tarea1, const Tarea& tarea2) const override;
    virtual ~CondicionTarea() {}
};

#endif