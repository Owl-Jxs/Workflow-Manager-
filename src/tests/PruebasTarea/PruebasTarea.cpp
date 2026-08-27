/*
#include "PruebasTarea.h"

#include "../../Controllers/TareaController/TareaController.h"
#include "../../models/Tarea/Tarea.h"
#include "../../persistence/TareaDataBase/TareaDataBase.h"
#include "../../structures/ColaFIFO/ColaFIFO.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
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

    class RespaldoArchivosTareas {
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
        RespaldoArchivosTareas() {
            std::filesystem::create_directories("data");

            const std::vector<std::string> rutas = {
                "data/tareas_regulares.csv",
                "data/tareas_urgentes.csv"
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

        ~RespaldoArchivosTareas() {
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

namespace PruebasTarea {

    void probarModeloTarea() {
        // --- constructor y getters ---
        Tarea tarea(1, "Comprar pan", false, Tarea::ESTADO[0]);
        verificar(tarea.getIdTarea() == 1, "id inicial");
        verificar(tarea.getDescripcionTarea() == "Comprar pan", "descripcion inicial");
        verificar(!tarea.getPrioridad(), "prioridad inicial");
        verificar(tarea.getEstado() == "POR HACER", "estado inicial");
        verificar(tarea.getIdPadre() == Tarea::sinPadre, "sin padre inicial");
        verificar(tarea.getTareaPadre() == nullptr, "puntero a padre nulo inicial");
        verificar(tarea.getCantidadSubTareas() == 0, "sin subtareas inicial");

        // --- setters ---
        tarea.setIdTarea(10);
        verificar(tarea.getIdTarea() == 10, "setIdTarea");
        tarea.setDescripcionTarea("Otra descripcion");
        verificar(tarea.getDescripcionTarea() == "Otra descripcion", "setDescripcionTarea");
        tarea.setPrioridad(true);
        verificar(tarea.getPrioridad(), "setPrioridad");
        tarea.setEstado("COMPLETADA");
        verificar(tarea.getEstado() == "COMPLETADA", "setEstado");

        // --- validaciones de parametros ---
        verificarQueLanza([&] { tarea.setIdTarea(-1); }, "id negativo rechazado");
        verificarQueLanza([&] { tarea.setDescripcionTarea(""); }, "descripcion vacia rechazada");
        verificarQueLanza([&] { tarea.setEstado("INVALIDO"); }, "estado invalido rechazado");
        verificarQueLanza([] { Tarea t(-5, "x", false, "POR HACER"); }, "constructor con id negativo rechazado");
        verificarQueLanza([] { Tarea t(1, "", false, "POR HACER"); }, "constructor con descripcion vacia rechazado");

        // --- arbol de subtareas ---
        Tarea* raiz = new Tarea(100, "Raiz", false, "POR HACER");
        Tarea* hijo1 = new Tarea(101, "Hijo 1", false, "POR HACER");
        Tarea* hijo2 = new Tarea(102, "Hijo 2", false, "POR HACER");

        raiz->agregarSubTarea(hijo1);
        raiz->agregarSubTarea(hijo2);
        verificar(raiz->getCantidadSubTareas() == 2, "cantidad de subtareas");
        verificar(raiz->getPrimerSubTarea() == hijo1, "primer subtarea");
        verificar(hijo1->getSiguienteSubTarea() == hijo2, "segunda subtarea como hermano");
        verificar(hijo1->getTareaPadre() == raiz, "puntero al padre del hijo");
        verificar(hijo1->getIdPadre() == 100, "id del padre del hijo");
        verificar(raiz->buscarSubTarea(102) == hijo2, "buscarSubTarea encuentra al hijo");
        verificar(raiz->buscarSubTarea(999) == nullptr, "buscarSubTarea no encuentra inexistente");

        // no se puede re-agregar una subtarea que ya tiene padre
        verificarQueLanza([&] { raiz->agregarSubTarea(hijo1); }, "reagregar subtarea con padre rechazado");

        // no se puede agregar un ancestro como subtarea (crearia ciclo)
        verificarQueLanza([&] { hijo1->agregarSubTarea(raiz); }, "agregar ancestro como subtarea rechazado");

        // no se puede agregar la tarea a si misma
        verificarQueLanza([&] { raiz->agregarSubTarea(raiz); }, "agregarse a si misma rechazado");

        // no se puede agregar una subtarea nula
        verificarQueLanza([&] { raiz->agregarSubTarea(nullptr); }, "subtarea nula rechazada");

        delete raiz; // borra en cascada a hijo1 y hijo2
    }

    void probarListaTareas() {
        ColaFIFO cola;
        verificar(cola.estaVacia(), "cola vacia inicial");

        Tarea* t1 = new Tarea(1, "Tarea 1", false, "POR HACER");
        Tarea* t2 = new Tarea(2, "Tarea 2", true, "POR HACER");

        cola.encolar(t1);
        cola.encolar(t2);
        verificar(!cola.estaVacia(), "cola no vacia tras encolar");
        verificar(cola.getFrente()->datos == t1, "frente es t1");
        verificar(cola.getCola()->datos == t2, "final es t2");

        // tarea repetida por id
        Tarea* duplicada = new Tarea(1, "otra descripcion", false, "POR HACER");
        verificarQueLanza([&] { cola.encolar(duplicada); }, "tarea repetida rechazada");
        delete duplicada;

        // tarea nula
        verificarQueLanza([&] { cola.encolar(nullptr); }, "tarea nula rechazada");

        // desencolar en orden FIFO
        Tarea* sacada1 = cola.desencolar();
        verificar(sacada1 == t1, "desencolar devuelve t1");
        delete sacada1; // la cola ya no es duena de la tarea desencolada

        Tarea* sacada2 = cola.desencolar();
        verificar(sacada2 == t2, "desencolar devuelve t2");
        delete sacada2;

        verificar(cola.estaVacia(), "cola vacia tras desencolar");
        verificarQueLanza([&] { cola.desencolar(); }, "desencolar cola vacia rechazado");
    }

    void probarTareaDataBase() {
        RespaldoArchivosTareas respaldo;
        TareaDataBase db;

        // --- guardar y cargar un arbol completo ---
        {
            ColaFIFO* lista = new ColaFIFO();
            Tarea* raiz = new Tarea(10, "Raiz", false, "POR HACER");
            Tarea* sub = new Tarea(11, "Sub", false, "EN PROCESO");
            raiz->agregarSubTarea(sub);
            lista->encolar(raiz);
            db.guardarListaEnArchivo(lista, false);
            delete lista;
        }

        ColaFIFO* cargada = db.cargarListaDelArchivo(false);
        verificar(cargada->getFrente() != nullptr, "lista cargada con tarea");
        verificar(cargada->getFrente()->datos->getIdTarea() == 10, "id de la raiz cargada");
        verificar(cargada->getFrente()->datos->getDescripcionTarea() == "Raiz", "descripcion de la raiz cargada");
        verificar(cargada->getFrente()->datos->getCantidadSubTareas() == 1, "subtarea cargada en el arbol");
        Tarea* subCargada = cargada->getFrente()->datos->getPrimerSubTarea();
        verificar(subCargada != nullptr, "puntero a la subtarea cargada");
        verificar(subCargada->getIdTarea() == 11, "id de la subtarea cargada");
        verificar(subCargada->getTareaPadre() == cargada->getFrente()->datos, "padre puntero de la subtarea cargada");
        delete cargada;

        // --- guardar una tarea nueva (append) y recargar ---
        Tarea* nueva = new Tarea(20, "Nueva urgente", true, "POR HACER");
        db.guardarNuevaTareaEnArchivo(nueva, true);
        delete nueva;

        ColaFIFO* urgentes = db.cargarListaDelArchivo(true);
        verificar(urgentes->getFrente() != nullptr, "tarea urgente cargada");
        verificar(urgentes->getFrente()->datos->getIdTarea() == 20, "id de la tarea urgente cargada");
        delete urgentes;

        // --- agregar una subtarea al archivo (append) y recargar ---
        {
            ColaFIFO* lista = new ColaFIFO();
            Tarea* raiz = new Tarea(30, "Raiz con subtarea", false, "POR HACER");
            lista->encolar(raiz);
            db.guardarListaEnArchivo(lista, false);
            delete lista;
        }

        Tarea* raizEnMemoria = new Tarea(30, "Raiz con subtarea", false, "POR HACER");
        Tarea* subNueva = new Tarea(31, "Sub nueva", false, "POR HACER");
        raizEnMemoria->agregarSubTarea(subNueva); // setea el idPadre de la subtarea
        db.guardarNuevaSubTareaEnArchivo(subNueva, false);
        delete raizEnMemoria; // borra en cascada a subNueva

        ColaFIFO* conSub = db.cargarListaDelArchivo(false);
        verificar(conSub->getFrente() != nullptr, "raiz presente tras agregar subtarea");
        verificar(conSub->getFrente()->datos->getCantidadSubTareas() == 1, "subtarea agregada al archivo");
        verificar(conSub->getFrente()->datos->getPrimerSubTarea()->getIdTarea() == 31, "id de la subtarea agregada");
        delete conSub;

        // --- descripciones con comas y comillas no rompen el csv ---
        {
            ColaFIFO* lista = new ColaFIFO();
            Tarea* especial = new Tarea(50, "Descripcion, con comas y \"comillas\"", false, "POR HACER");
            lista->encolar(especial);
            db.guardarListaEnArchivo(lista, false);
            delete lista;
        }

        ColaFIFO* conEspecial = db.cargarListaDelArchivo(false);
        verificar(conEspecial->getFrente()->datos->getDescripcionTarea() ==
                      "Descripcion, con comas y \"comillas\"",
                  "descripcion especial redondea correctamente");
        delete conEspecial;
    }

    void probarTareaController() {
        RespaldoArchivosTareas respaldo;

        TareaController controller;
        controller.cargarArchivos();

        Tarea* t1 = new Tarea(1, "Ctrl 1", false, "POR HACER");
        controller.agregarTarea(t1, false);

        Tarea* t2 = new Tarea(2, "Ctrl 2", true, "POR HACER");
        controller.agregarTarea(t2, true);

        // la subtarea queda bajo la propiedad de la tarea padre en la cola
        controller.agregarSubTarea(new Tarea(3, "Sub ctrl", false, "POR HACER"), 1, false);

        controller.guardarArchivos();

        // recargar desde archivo sin excepciones
        TareaController recargado;
        recargado.cargarArchivos();
        recargado.guardarArchivos();

        // padre inexistente debe lanzar
        TareaController otro;
        otro.cargarArchivos();
        Tarea* huerfana = new Tarea(99, "Huerfana", false, "POR HACER");
        verificarQueLanza([&] { otro.agregarSubTarea(huerfana, 777, false); },
                          "subtarea con padre inexistente rechazada");
        delete huerfana;
    }

    void ejecutarTodas() {
        try {
            probarModeloTarea();
            imprimirOk("probarModeloTarea");

            probarListaTareas();
            imprimirOk("probarListaTareas");

            probarTareaDataBase();
            imprimirOk("probarTareaDataBase");

            probarTareaController();
            imprimirOk("probarTareaController");

            std::cout << "Todas las pruebas pasaron correctamente." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Fallo en las pruebas: " << e.what() << std::endl;
            throw;
        }
    }
}
*/