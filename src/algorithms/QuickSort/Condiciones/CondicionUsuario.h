#ifndef CONDICIONUSUARIO_H
#define CONDICIONUSUARIO_H
#include "ICondicion.h"
#include "../../models/Usuario/Usuario.h"

class CondicionUsuario : public ICondicion<Usuario> {
public:
    bool comparar(const Usuario& usuario1, const Usuario& usuario2) const override;
    virtual ~CondicionUsuario() {}
};

#endif