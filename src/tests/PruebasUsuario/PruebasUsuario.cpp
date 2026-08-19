#include "PruebasUsuario.h"

#include "../../models/Usuario/Usuario.h"
#include "../../structures/ListaDoble/ListaDoble.h"
#include "../../persistence/UsuarioDataBase/UsuarioDataBase.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
    void verificar(bool condicion, const std::string& mensaje) {
        if (!condicion) {
            throw std::runtime_error("Prueba fallida: " + mensaje);
        }
    }

    template <typename Funcion>
    void verificarQueLanza(Funcion funcion, const std::string& mensaje) {
        try {
            funcion();
        } catch (const std::exception&) {
            return;
        }

        throw std::runtime_error("Prueba fallida: " + mensaje);
    }

    class RespaldoArchivosUsuarios {
    private:
        struct Archivo {
            std::string ruta;
            bool existia;
            std::string contenido;
        };

        std::vector<Archivo> archivos;

        static std::string leerArchivo(const std::string& ruta) {
            std::ifstream entrada(ruta, std::ios::binary);
            return std::string(
                std::istreambuf_iterator<char>(entrada),
                std::istreambuf_iterator<char>()
            );
        }

        static void escribirArchivo(const std::string& ruta, const std::string& contenido) {
            std::ofstream salida(ruta, std::ios::binary | std::ios::trunc);
            if (!salida.is_open()) {
                throw std::runtime_error("No se pudo escribir el archivo de prueba: " + ruta);
            }
            salida << contenido;
        }

    public:
        RespaldoArchivosUsuarios() {
            std::filesystem::create_directories("data");

            const std::vector<std::string> rutas = {
                "data/usuarios.csv",
                "data/temporalUsuarios.csv"
            };

            for (const std::string& ruta : rutas) {
                Archivo archivo;
                archivo.ruta = ruta;
                archivo.existia = std::filesystem::exists(ruta);
                archivo.contenido = archivo.existia ? leerArchivo(ruta) : "";
                archivos.push_back(archivo);

                escribirArchivo(ruta, "");
            }
        }

        ~RespaldoArchivosUsuarios() {
            for (const Archivo& archivo : archivos) {
                try {
                    if (archivo.existia) {
                        escribirArchivo(archivo.ruta, archivo.contenido);
                    } else {
                        std::remove(archivo.ruta.c_str());
                    }
                } catch (...) {
                }
            }
        }
    };

    void imprimirOk(const std::string& nombrePrueba) {
        std::cout << "[OK] " << nombrePrueba << std::endl;
    }
}

namespace PruebasUsuario {

    void probarModeloUsuario() {
        // --- constructor por defecto ---
        Usuario porDefecto;
        verificar(porDefecto.getId() == 0, "id por defecto");
        verificar(porDefecto.getNombre() == "", "nombre por defecto");
        verificar(porDefecto.getRol() == Usuario::Rol::USUARIO_NORMAL, "rol por defecto");
        verificar(porDefecto.getHashContrasena() == 0, "hash por defecto en 0");

        // --- constructor con parametros ---
        Usuario admin(5, "Ana", Usuario::Rol::ADMINISTRADOR);
        verificar(admin.getId() == 5, "id con parametros");
        verificar(admin.getNombre() == "Ana", "nombre con parametros");
        verificar(admin.getRol() == Usuario::Rol::ADMINISTRADOR, "rol con parametros");

        // --- constructor con rol por defecto ---
        Usuario normal(7, "Beto");
        verificar(normal.getRol() == Usuario::Rol::USUARIO_NORMAL, "rol por defecto en constructor con parametros");

        // --- setters y getters ---
        admin.setId(9);
        verificar(admin.getId() == 9, "setId");
        admin.setNombre("Carlos");
        verificar(admin.getNombre() == "Carlos", "setNombre");
        admin.setRol(Usuario::Rol::USUARIO_NORMAL);
        verificar(admin.getRol() == Usuario::Rol::USUARIO_NORMAL, "setRol a normal");
        admin.setRol(Usuario::Rol::ADMINISTRADOR);
        verificar(admin.getRol() == Usuario::Rol::ADMINISTRADOR, "setRol a administrador");

        // --- hash de contrasena ---
        verificar(admin.getHashContrasena() == 0, "hash inicial en 0 tras constructor");
        admin.setHashContrasena("miClaveSecreta");
        verificar(admin.getHashContrasena() != 0, "setHashContrasena genera un hash distinto de 0");
        Usuario igual(0, "x");
        igual.setHashContrasena("miClaveSecreta");
        verificar(admin.getHashContrasena() == igual.getHashContrasena(), "misma clave genera el mismo hash");
        admin.setHashDirecto(12345);
        verificar(admin.getHashContrasena() == 12345, "setHashDirecto asigna el valor exacto");

        // --- validaciones de parametros ---
        verificarQueLanza([&] { admin.setId(-1); }, "id negativo rechazado");
        verificarQueLanza([&] { admin.setNombre(""); }, "nombre vacio rechazado");
        verificarQueLanza([&] { admin.setHashContrasena(""); }, "contrasena vacia rechazada");
        verificarQueLanza([] { Usuario u(-5, "x"); }, "constructor con id negativo rechazado");
        verificarQueLanza([] { Usuario u(1, ""); }, "constructor con nombre vacio rechazado");
    }

    void probarListaDoble() {
        ListaDoble lista;

        Usuario* ana = new Usuario(1, "Ana", Usuario::Rol::ADMINISTRADOR);
        Usuario* beto = new Usuario(2, "Beto");
        Usuario* carla = new Usuario(3, "Carla", Usuario::Rol::ADMINISTRADOR);

        // agregar (nulo lanza)
        verificarQueLanza([&] { lista.agregarUsuario(nullptr); }, "usuario nulo rechazado");
        lista.agregarUsuario(ana);
        lista.agregarUsuario(beto);
        lista.agregarUsuario(carla);

        std::vector<Usuario*> todos = lista.listarUsuarios();
        verificar(todos.size() == 3, "se agregaron 3 usuarios");
        verificar(todos[0] == ana && todos[1] == beto && todos[2] == carla, "orden de insercion");

        // agregar duplicado lanza
        Usuario* anaDuplicada = new Usuario(1, "Ana Duplicada");
        verificarQueLanza([&] { lista.agregarUsuario(anaDuplicada); }, "usuario con id duplicado rechazado");
        delete anaDuplicada;

        // buscar
        verificar(lista.buscarPorId(2) == beto, "buscarPorId encuentra");
        verificar(lista.buscarPorId(99) == nullptr, "buscarPorId no encuentra");

        // ids negativos rechazados
        verificarQueLanza([&] { lista.buscarPorId(-1); }, "buscarPorId con id negativo rechazado");
        verificarQueLanza([&] { lista.actualizarUsuario(-1, ana); }, "actualizarUsuario con id negativo rechazado");
        verificarQueLanza([&] { lista.eliminarUsuario(-1); }, "eliminarUsuario con id negativo rechazado");

        // actualizar existente
        Usuario* nuevoBeto = new Usuario(2, "Beto Actualizado");
        verificar(lista.actualizarUsuario(2, nuevoBeto), "actualizarUsuario existente devuelve true");
        verificar(lista.buscarPorId(2) == nuevoBeto, "actualizarUsuario reemplaza el objeto");
        verificar(lista.buscarPorId(2)->getNombre() == "Beto Actualizado", "actualizarUsuario aplica los nuevos datos");
        verificar(lista.listarUsuarios().size() == 3, "cantidad se mantiene tras actualizar");

        // actualizar con el mismo objeto no debe borrarlo (use-after-free)
        Usuario* mismo = lista.buscarPorId(3);
        verificar(lista.actualizarUsuario(3, mismo), "actualizarUsuario con el mismo objeto devuelve true");
        verificar(lista.buscarPorId(3) == mismo, "el mismo objeto sigue vivo tras actualizar");

        // actualizar inexistente / nulo
        verificar(!lista.actualizarUsuario(999, nuevoBeto), "actualizarUsuario inexistente devuelve false");
        verificar(!lista.actualizarUsuario(1, nullptr), "actualizarUsuario nulo devuelve false");

        // eliminar del medio
        verificar(lista.eliminarUsuario(2), "eliminarUsuario medio devuelve true");
        verificar(lista.buscarPorId(2) == nullptr, "usuario del medio eliminado");
        todos = lista.listarUsuarios();
        verificar(todos.size() == 2 && todos[0] == ana && todos[1] == carla, "lista tras eliminar del medio");

        // eliminar cabeza
        verificar(lista.eliminarUsuario(1), "eliminarUsuario cabeza devuelve true");
        todos = lista.listarUsuarios();
        verificar(todos.size() == 1 && todos[0] == carla, "lista tras eliminar cabeza");

        // eliminar inexistente
        verificar(!lista.eliminarUsuario(1), "eliminarUsuario inexistente devuelve false");

        // eliminar el ultimo elemento
        verificar(lista.eliminarUsuario(3), "eliminarUsuario ultimo devuelve true");
        todos = lista.listarUsuarios();
        verificar(todos.empty(), "lista vacia al final");
        verificar(lista.buscarPorId(3) == nullptr, "sin referencias tras vaciar la lista");
    }

    void probarUsuarioDataBase() {
        RespaldoArchivosUsuarios respaldo;
        UsuarioDataBase db;

        // archivo vacio -> lista vacia (cubre la primera ejecucion)
        ListaDoble* cargada = db.cargarUsuariosDesdeArchivo();
        verificar(cargada->listarUsuarios().empty(), "cargar con archivo vacio devuelve lista vacia");
        delete cargada;

        // round-trip guardar -> cargar (roles y hash incluidos)
        {
            ListaDoble lista;
            Usuario* ana = new Usuario(1, "Ana", Usuario::Rol::ADMINISTRADOR);
            ana->setHashContrasena("claveDeAna");
            lista.agregarUsuario(ana);
            lista.agregarUsuario(new Usuario(2, "Beto"));
            db.guardarUsuariosEnArchivo(&lista);
        }

        cargada = db.cargarUsuariosDesdeArchivo();
        std::vector<Usuario*> usuarios = cargada->listarUsuarios();
        verificar(usuarios.size() == 2, "round-trip conserva la cantidad");
        verificar(usuarios[0]->getId() == 1 && usuarios[0]->getNombre() == "Ana"
                      && usuarios[0]->getRol() == Usuario::Rol::ADMINISTRADOR,
                  "usuario 1 correcto tras round-trip");
        Usuario anaEsperada(0, "x");
        anaEsperada.setHashContrasena("claveDeAna");
        verificar(usuarios[0]->getHashContrasena() == anaEsperada.getHashContrasena(),
                  "hash del usuario 1 se conserva tras round-trip");
        verificar(usuarios[1]->getId() == 2 && usuarios[1]->getNombre() == "Beto"
                      && usuarios[1]->getRol() == Usuario::Rol::USUARIO_NORMAL,
                  "usuario 2 correcto tras round-trip");
        verificar(usuarios[1]->getHashContrasena() == 0,
                  "usuario sin clave conserva hash en 0 tras round-trip");
        delete cargada;

        // agregarUsuario (append) y recargar
        Usuario* especial = new Usuario(3, "Carla, con \"comillas\"", Usuario::Rol::ADMINISTRADOR);
        db.agregarUsuario(especial);
        delete especial; // la base de datos no es duena del puntero; solo escribio la linea

        cargada = db.cargarUsuariosDesdeArchivo();
        usuarios = cargada->listarUsuarios();
        verificar(usuarios.size() == 3, "append agrega el tercer usuario");
        verificar(usuarios[2]->getNombre() == "Carla, con \"comillas\"", "nombre con comas y comillas redondea");
        delete cargada;

        // id duplicado -> lanza
        Usuario* duplicada = new Usuario(2, "Duplicada");
        verificarQueLanza([&] { db.agregarUsuario(duplicada); }, "id duplicado rechazado");
        delete duplicada;

        // usuario nulo -> lanza
        verificarQueLanza([&] { db.agregarUsuario(nullptr); }, "usuario nulo rechazado");

        // eliminarUsuario del archivo y recargar
        db.eliminarUsuario(2);
        cargada = db.cargarUsuariosDesdeArchivo();
        usuarios = cargada->listarUsuarios();
        verificar(usuarios.size() == 2 && usuarios[0]->getId() == 1 && usuarios[1]->getId() == 3,
                  "eliminarUsuario elimina del archivo");
        delete cargada;

        // eliminarUsuario inexistente -> lanza
        verificarQueLanza([&] { db.eliminarUsuario(999); }, "eliminarUsuario inexistente rechazado");

        // eliminarUsuario con id negativo -> lanza
        verificarQueLanza([&] { db.eliminarUsuario(-1); }, "eliminarUsuario con id negativo rechazado");

        // guardar con lista nula -> lanza
        verificarQueLanza([&] { db.guardarUsuariosEnArchivo(nullptr); }, "guardar lista nula rechazado");
    }

    void ejecutarTodas() {
        try {
            probarModeloUsuario();
            imprimirOk("probarModeloUsuario");

            probarListaDoble();
            imprimirOk("probarListaDoble");

            probarUsuarioDataBase();
            imprimirOk("probarUsuarioDataBase");

            std::cout << "Todas las pruebas de Usuario pasaron correctamente." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Fallo en las pruebas de Usuario: " << e.what() << std::endl;
            throw;
        }
    }
}
