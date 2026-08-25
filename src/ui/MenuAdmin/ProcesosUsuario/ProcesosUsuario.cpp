#include "ProcesosUsuario.h"

ProcesosUsuario::ProcesosUsuario (UsuarioController* _uc, Usuario* _uA, GestorHistorial* _gH) {
    this-> uc = _uc;
    this->usuarioActivo = _uA;
    this->gestorHistorial = _gH;

}

Usuario* ProcesosUsuario::leerNuevoUsuario() {
    int id;
    std::string nombre;
    int opcionRol;
    Usuario::Rol rol;
    Usuario* nuevoUsuario;
    id = ValidarEntrada::validarCodigoNumerico ("Ingrese su identificacion", 9);
    nombre =ValidarEntrada::validarNombreCompleto ();

    std::cout << "\nSeleccione el rol:\n";
    std::cout << "1. Administrador\n";
    std::cout << "2. Usuario Normal\n";
    opcionRol = ValidarEntrada::validarEntradaRango ("Ingrese su opcion", 1,2);

    switch (opcionRol) {
        case 1: {
            rol = Usuario::Rol::ADMINISTRADOR; break;
        }
        case 2: {
            rol = Usuario::Rol::USUARIO_NORMAL; break;
        }
            
    };
    
    nuevoUsuario = new Usuario (id, nombre, rol);
    std::string contrasena = ValidarEntrada::validarContrasena ("Ingrese la contrasena: ", '*');
    nuevoUsuario->setHashContrasena (contrasena);
    return nuevoUsuario;
}

void ProcesosUsuario::agregarUsuario  () {
    Usuario* usuario = leerNuevoUsuario();
    try {
        AgregarUsuarioComando* comandoAgregar = new AgregarUsuarioComando (uc,usuario);
        gestorHistorial->ejecutarComando (comandoAgregar);
        std::cout << "Usuario agregado correctamente.\n";

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        delete usuario;
    }
}

void ProcesosUsuario::actualizarUsuario () {
    int id = ValidarEntrada::validarCodigoNumerico ("Ingrese el id del usuario buscado", 9);
    Usuario* usuarioActual = uc->buscarUsuarioPorId(id);
    Usuario* nuevoUsuario = new Usuario (usuarioActual);

    if (usuarioActual == nullptr) {
        std::cout << "No existe un usuario con ese ID.\n";
        return;
    }

    std::cout << "\nUsuario encontrado:\n";
    std::cout << "ID: " << usuarioActual->getId() << std::endl;
    std::cout << "Nombre: " << usuarioActual->getNombre() << std::endl;

    bool cambioNombre = ValidarEntrada::respuestas_Si_O_No ("Cambiar el nombre", "mantener nombre actual");
    if (cambioNombre) {
        std::string nuevoNombre = ValidarEntrada::validarNombreCompleto ();
        nuevoUsuario->setNombre (nuevoNombre);
    }

    bool cambioRol = ValidarEntrada::respuestas_Si_O_No ("Cambiar el rol", "mantener el Rol actual");
    if (cambioRol) {
        std::cout << "\nNuevo rol:\n";
        std::cout << "1. Administrador\n";
        std::cout << "2. Usuario Normal\n";
        int opcionRol = ValidarEntrada::validarEntradaRango ("Ingrese su opcion", 1,2);

        Usuario::Rol rol = ((opcionRol == 1) ? Usuario::Rol::ADMINISTRADOR : Usuario::Rol::USUARIO_NORMAL); 
        nuevoUsuario->setRol (rol);
    }

    bool cambioContrasena = ValidarEntrada::respuestas_Si_O_No ("Cambiar la contrasena", "Mantener actual");
    if (cambioContrasena) {
        std::string nuevaContra = ValidarEntrada::validarContrasena ("Ingrese su nueva contrasena", '*');
        nuevoUsuario->setHashContrasena (nuevaContra);
    }
    
    try {
        ActualizarUsuarioComando* actualizacion = new ActualizarUsuarioComando (uc, id, nuevoUsuario);
        gestorHistorial->ejecutarComando (actualizacion);
        std::cout << "Usuario actualizado" << std::endl;
    } catch (std::exception &e) {
        std::cout << "Error al actualizar usuario" << std::endl; throw;
    }
}

void ProcesosUsuario::eliminarUsuario () {
    int id = ValidarEntrada::validarCodigoNumerico ("Digite el id del usuario", 9);
    Usuario* usuario = uc->buscarUsuarioPorId(id);

    if (usuario == nullptr) {
        std::cout << "No existe un usuario con ese ID.\n"; return;
    }

    std::cout << "\nUsuario encontrado:\n";
    std::cout << "ID: " << usuario->getId() << std::endl;
    std::cout << "Nombre: " << usuario->getNombre() << std::endl;
    std::cout << "¿Esta seguro de eliminarlo? \n";
    bool eliminarlo = ValidarEntrada::respuestas_Si_O_No ("Si", "No");
     
    if (eliminarlo) { 
        try {
            EliminarUsuarioComando* eliminar = new EliminarUsuarioComando (uc, id);
            gestorHistorial->ejecutarComando (eliminar);
            std::cout << "Usuario eliminado correctamente.\n";
        } catch (std::exception &e) {
            std::cout << "No se pudo eliminar el usuario.\n"; throw;
        }
    } else {
        std::cout << "Operacion cancelada.\n";
    }
}

void ProcesosUsuario::mostrarLista () {
    std::vector<Usuario*> usuarios = uc->listarUsuarios();
    if (usuarios.empty()) 
    {
        std::cout << "No hay usuarios registrados.\n"; return;
    }

    std::cout << "\n========== LISTA DE USUARIOS ==========\n";
    for (Usuario* usuario : usuarios) {
        
        std::cout << "ID: " << usuario->getId() << std::endl;
        std::cout << "Nombre: " << usuario->getNombre() << std::endl;
        std::cout << ((usuario->getRol() == Usuario::Rol::ADMINISTRADOR) ? "Rol: ADMINISTRADOR\n" : "Rol: USUARIO_NORMAL\n");
        std::cout << "--------------------------------------\n";
    }
}

void ProcesosUsuario::mostrarUsuarioPorId () {
    int id =  ValidarEntrada::validarCodigoNumerico ("Ingrese el id del usuario", 9);
    Usuario* usuario = uc->buscarUsuarioPorId(id);

    if (usuario == nullptr) {
        std::cout << "No existe un usuario con ese ID.\n"; return;
    }

    std::cout << "\n========== USUARIO ENCONTRADO ==========\n";
    std::cout << "ID: " << usuario->getId() << std::endl;
    std::cout << "Nombre: " << usuario->getNombre() << std::endl;
    ((usuario->getRol() == Usuario::Rol::ADMINISTRADOR) ? "Rol: ADMINISTRADOR\n" : "Rol: USUARIO_NORMAL\n");
}
