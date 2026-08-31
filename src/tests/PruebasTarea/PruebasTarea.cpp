#include "PruebasTarea.h"

#include "../../Controllers/TareaController/TareaController.h"
#include "../../models/Tarea/Tarea.h"
#include "../../persistence/TareaDataBase/TareaDataBase.h"
#include "../../structures/Colas/ColaFIFO/ColaFIFO.h"
#include "../../structures/Colas/ColaPrioridad/ColaPrioridad.h"

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

    Tarea* buscarPorIdEnCola(Cola* cola, int id) {
        if (cola == nullptr) {
            return nullptr;
        }

        NodoTarea* actual = cola->getFrente();
        while (actual != nullptr) {
            if (actual->datos != nullptr && actual->datos->getIdTarea() == id) {
                return actual->datos;
            }
            actual = actual->siguiente;
        }
        return nullptr;
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
                "data/Tareas_Data.csv",
                "data/tareas_completadas.csv",
                "data/Temp.csv"
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
        Tarea tarea(1, "Comprar pan", false, Tarea::ESTADO[0]);
        verificar(tarea.getIdTarea() == 1, "id inicial");
        verificar(tarea.getDescripcionTarea() == "Comprar pan", "descripcion inicial");
        verificar(!tarea.getPrioridad(), "prioridad inicial");
        verificar(tarea.getEstado() == "POR HACER", "estado inicial");
        verificar(tarea.getIdPadre() == Tarea::sinPadre, "sin padre inicial");
        verificar(tarea.getTareaPadre() == nullptr, "puntero a padre nulo inicial");
        verificar(tarea.getCantidadSubTareas() == 0, "sin subtareas inicial");

        tarea.setIdTarea(10);
        verificar(tarea.getIdTarea() == 10, "setIdTarea");
        tarea.setDescripcionTarea("Otra descripcion");
        verificar(tarea.getDescripcionTarea() == "Otra descripcion", "setDescripcionTarea");
        tarea.setPrioridad(true);
        verificar(tarea.getPrioridad(), "setPrioridad");
        tarea.setEstado("COMPLETADA");
        verificar(tarea.getEstado() == "COMPLETADA", "setEstado");

        verificarQueLanza([&] { tarea.setIdTarea(-1); }, "id negativo rechazado");
        verificarQueLanza([&] { tarea.setDescripcionTarea(""); }, "descripcion vacia rechazada");
        verificarQueLanza([&] { tarea.setEstado("INVALIDO"); }, "estado invalido rechazado");
        verificarQueLanza([] { Tarea t(-5, "x", false, "POR HACER"); }, "constructor con id negativo rechazado");
        verificarQueLanza([] { Tarea t(1, "", false, "POR HACER"); }, "constructor con descripcion vacia rechazado");

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

        verificarQueLanza([&] { raiz->agregarSubTarea(hijo1); }, "reagregar subtarea con padre rechazado");
        verificarQueLanza([&] { hijo1->agregarSubTarea(raiz); }, "agregar ancestro como subtarea rechazado");
        verificarQueLanza([&] { raiz->agregarSubTarea(raiz); }, "agregarse a si misma rechazado");
        verificarQueLanza([&] { raiz->agregarSubTarea(nullptr); }, "subtarea nula rechazada");

        delete raiz;
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

        Tarea* duplicada = new Tarea(1, "otra descripcion", false, "POR HACER");
        verificarQueLanza([&] { cola.encolar(duplicada); }, "tarea repetida rechazada");
        delete duplicada;

        verificarQueLanza([&] { cola.encolar(nullptr); }, "tarea nula rechazada");

        Tarea* sacada1 = cola.desencolar();
        verificar(sacada1 == t1, "desencolar devuelve t1");
        delete sacada1;

        Tarea* sacada2 = cola.desencolar();
        verificar(sacada2 == t2, "desencolar devuelve t2");
        delete sacada2;

        verificar(cola.estaVacia(), "cola vacia tras desencolar");
        verificarQueLanza([&] { cola.desencolar(); }, "desencolar cola vacia rechazado");
    }

    void probarTareaDataBase() {
        RespaldoArchivosTareas respaldo;
        TareaDataBase db;

        {
            ColaFIFO regulares;
            ColaFIFO urgentes;
            ColaPrioridad revision;
            std::vector<Tarea*> enProceso;

            Tarea* raiz = new Tarea(10, "Raiz", false, "POR HACER");
            Tarea* sub = new Tarea(11, "Sub", false, "EN PROCESO");
            raiz->agregarSubTarea(sub);
            regulares.encolar(raiz);

            db.guardarTareasActivas(&regulares, &urgentes, enProceso, &revision);
        }

        ColaFIFO regularesCargadas;
        ColaFIFO urgentesCargadas;
        std::vector<Tarea*> enProcesoCargadas;
        ColaPrioridad revisionCargadas;
        int ultimoId = 0;
        db.cargarTareasActivas(&regularesCargadas, &urgentesCargadas, enProcesoCargadas, &revisionCargadas, ultimoId);

        verificar(regularesCargadas.getFrente() != nullptr, "lista cargada con tarea");
        verificar(regularesCargadas.getFrente()->datos->getIdTarea() == 10, "id de la raiz cargada");
        verificar(regularesCargadas.getFrente()->datos->getDescripcionTarea() == "Raiz", "descripcion de la raiz cargada");
        verificar(regularesCargadas.getFrente()->datos->getCantidadSubTareas() == 1, "subtarea cargada en el arbol");
        Tarea* subCargada = regularesCargadas.getFrente()->datos->getPrimerSubTarea();
        verificar(subCargada != nullptr, "puntero a la subtarea cargada");
        verificar(subCargada->getIdTarea() == 11, "id de la subtarea cargada");
        verificar(subCargada->getTareaPadre() == regularesCargadas.getFrente()->datos, "padre puntero de la subtarea cargada");

        Tarea* nueva = new Tarea(20, "Nueva urgente", true, "POR HACER");
        db.guardarNuevaTareaEnArchivo(nueva);
        delete nueva;

        ColaFIFO regularesTrasAppend;
        ColaFIFO urgentesTrasAppend;
        std::vector<Tarea*> enProcesoTrasAppend;
        ColaPrioridad revisionTrasAppend;
        int ultimoIdTrasAppend = 0;
        db.cargarTareasActivas(&regularesTrasAppend, &urgentesTrasAppend, enProcesoTrasAppend, &revisionTrasAppend, ultimoIdTrasAppend);
        verificar(buscarPorIdEnCola(&urgentesTrasAppend, 20) != nullptr || buscarPorIdEnCola(&regularesTrasAppend, 20) != nullptr,
                 "tarea urgente cargada");

        Tarea* raizConSub = new Tarea(30, "Raiz con subtarea", false, "POR HACER");
        Tarea* subNueva = new Tarea(31, "Sub nueva", false, "POR HACER");
        raizConSub->agregarSubTarea(subNueva);
        db.guardarNuevaTareaEnArchivo(raizConSub);
        delete raizConSub;

        ColaFIFO regularesConSub;
        ColaFIFO urgentesConSub;
        std::vector<Tarea*> enProcesoConSub;
        ColaPrioridad revisionConSub;
        int ultimoIdConSub = 0;
        db.cargarTareasActivas(&regularesConSub, &urgentesConSub, enProcesoConSub, &revisionConSub, ultimoIdConSub);
        Tarea* raizPersistida = regularesConSub.getFrente() != nullptr ? regularesConSub.getFrente()->datos : nullptr;
        verificar(raizPersistida != nullptr, "raiz presente tras agregar subtarea");
        verificar(raizPersistida->getCantidadSubTareas() >= 1, "subtarea agregada al archivo");

        Tarea* especial = new Tarea(50, "Descripcion, con comas y \"comillas\"", false, "POR HACER");
        db.guardarNuevaTareaEnArchivo(especial);
        delete especial;

        ColaFIFO regularesEspeciales;
        ColaFIFO urgentesEspeciales;
        std::vector<Tarea*> enProcesoEspeciales;
        ColaPrioridad revisionEspeciales;
        int ultimoIdEspeciales = 0;
        db.cargarTareasActivas(&regularesEspeciales, &urgentesEspeciales, enProcesoEspeciales, &revisionEspeciales, ultimoIdEspeciales);

        Tarea* especialCargada = buscarPorIdEnCola(&regularesEspeciales, 50);
        verificar(especialCargada != nullptr, "descripcion especial cargada");
        verificar(especialCargada->getDescripcionTarea() == "Descripcion, con comas y \"comillas\"",
                 "descripcion especial redondea correctamente");
    }

    void probarTareaController() {
        RespaldoArchivosTareas respaldo;

        TareaController controller;
        controller.cargarArchivos();

        Tarea* t1 = new Tarea(1, "Ctrl 1", false, "POR HACER");
        controller.agregarTarea(t1);

        Tarea* t2 = new Tarea(2, "Ctrl 2", true, "POR HACER");
        controller.agregarTarea(t2);

        Tarea* subCtrl = new Tarea(3, "Sub ctrl", false, "POR HACER");
        subCtrl->setIdPadre(1);
        controller.agregarTarea(subCtrl);

        controller.guardarArchivos();

        TareaController recargado;
        recargado.cargarArchivos();
        recargado.guardarArchivos();

        TareaController otro;
        otro.cargarArchivos();
        Tarea* huerfana = new Tarea(99, "Huerfana", false, "POR HACER");
        huerfana->setIdPadre(777);
        verificarQueLanza([&] { otro.agregarTarea(huerfana); },
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
