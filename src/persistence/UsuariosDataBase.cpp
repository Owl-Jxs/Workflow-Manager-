#include "UsuarioDataBase.h"

const std::string UsuarioDataBase::FILENAME_USUARIOS = "usuarios.csv";

std::string UsuarioDataBase::formularLinea(Usuario* usuario)
{
    std::stringstream linea;

    std::string rol;

    if (usuario->getRol() == Usuario::Rol::ADMINISTRADOR)
    {
        rol = "ADMINISTRADOR";
    }
    else
    {
        rol = "USUARIO_NORMAL";
    }

    linea << usuario->getId() << ","
          << usuario->getNombre() << ","
          << rol;

    return linea.str();
}

void UsuarioDataBase::guardarLista(
    ListaDoble* lista,
    std::string nombreArchivo)
{
    if (lista == nullptr)
    {
        throw std::invalid_argument(
            "La lista de usuarios es nula."
        );
    }

    std::string nombreTemp = "TempUsuarios.csv";

    std::ofstream archivoTemp(nombreTemp);

    if (!archivoTemp.is_open())
    {
        throw std::runtime_error(
            "Error al abrir el archivo temporal de usuarios."
        );
    }

    std::vector<Usuario*> usuarios = lista->listarUsuarios();

    for (Usuario* usuario : usuarios)
    {
        if (usuario != nullptr)
        {
            archivoTemp << formularLinea(usuario) << std::endl;
        }
    }

    archivoTemp.close();

    std::remove(nombreArchivo.c_str());

    if (std::rename(
            nombreTemp.c_str(),
            nombreArchivo.c_str()) != 0)
    {
        throw std::runtime_error(
            "Error al renombrar el archivo temporal de usuarios."
        );
    }
}

ListaDoble* UsuarioDataBase::cargarLista(
    std::string nombreArchivo)
{
    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open())
    {
        throw std::runtime_error(
            "Error al abrir el archivo de usuarios."
        );
    }

    ListaDoble* nuevaLista = new ListaDoble();

    std::string linea;

    while (std::getline(archivo, linea))
    {
        std::stringstream lineaActual(linea);

        std::string idTexto;
        std::string nombre;
        std::string rolTexto;

        if (std::getline(lineaActual, idTexto, ',') &&
            std::getline(lineaActual, nombre, ',') &&
            std::getline(lineaActual, rolTexto))
        {
            int id = std::stoi(idTexto);

            Usuario::Rol rol;

            if (rolTexto == "ADMINISTRADOR")
            {
                rol = Usuario::Rol::ADMINISTRADOR;
            }
            else
            {
                rol = Usuario::Rol::USUARIO_NORMAL;
            }

            Usuario* usuario =
                new Usuario(id, nombre, rol);

            nuevaLista->agregarUsuario(usuario);
        }
    }

    archivo.close();

    return nuevaLista;
}

UsuarioDataBase::UsuarioDataBase()
{
}

UsuarioDataBase::~UsuarioDataBase()
{
}

void UsuarioDataBase::guardarListaEnArchivo(
    ListaDoble* listaCompleta)
{
    guardarLista(
        listaCompleta,
        FILENAME_USUARIOS
    );
}

ListaDoble* UsuarioDataBase::cargarListaDelArchivo()
{
    return cargarLista(FILENAME_USUARIOS);
}

void UsuarioDataBase::guardarNuevoUsuarioEnArchivo(
    Usuario* usuario)
{
    if (usuario == nullptr)
    {
        throw std::invalid_argument(
            "No se puede guardar un usuario nulo."
        );
    }

    std::ofstream archivo(
        FILENAME_USUARIOS,
        std::ios::app
    );

    if (!archivo.is_open())
    {
        throw std::runtime_error(
            "Error al abrir el archivo de usuarios."
        );
    }

    archivo << formularLinea(usuario) << std::endl;

    archivo.close();
}