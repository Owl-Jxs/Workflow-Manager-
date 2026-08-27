#include "CondicionUsuario.h"

bool CondicionUsuario::comparar(const Usuario& usuario1, const Usuario& usuario2) const {
    return usuario1.getId() < usuario2.getId();
}