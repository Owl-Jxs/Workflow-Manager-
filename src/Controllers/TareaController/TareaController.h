#ifndef TAREACONTROLLER_H
#define TAREACONTROLLER_H

#include "../../persistence/TareaDataBase/TareaDataBase.h"
#include "../../structures/ColaFIFO/ColaFIFO.h"
#include <vector>

class TareaController {
private:
    ColaFIFO* listaTareasRegulares; // Lista de tareas regulares
    ColaFIFO* listaTareasUrgentes; // Lista de tareas prioritarias
    TareaDataBase* archivosTareas; //la data base de tareas
    int ultimoId;

    void aplanarArbol(Tarea* tarea, std::vector<Tarea*>& resultado);
    std::vector <Tarea*> listarTareas (ColaFIFO* cola);
    std::vector <Tarea*> listarSubTareas (Tarea* tarea);
    
public:
    TareaController ();
    ~TareaController ();
//guardar/cargar todos los archivos
    void guardarArchivos ();
    void cargarArchivos ();
//funciones para manipular las tareas de la lista
    void agregarTarea(Tarea* tarea, bool perteneceListaUrgente);
    void agregarSubTarea (Tarea* tarea, int idTareaPadre, bool perteneceListaUrgente);
    // Busca una tarea por id en ambas colas (incluye subtareas). Devuelve nullptr si no existe.
    Tarea* buscarTarea(int idTarea);
    // Extrae una tarea de la cola indicada sin destruirla (para deshacer). Devuelve la tarea o nullptr.
    Tarea* eliminarTarea(int idTarea, bool perteneceListaUrgente);
    // Mueve una tarea de la cola regular a la cola urgente (escalamiento SLA).
    Tarea* escalarTarea(int idTarea);
    //retoorna el ultimo id
    int getUltimoId () const;
    // entrega la lista urgente en forma de vector
    std::vector <Tarea*> listarTareasUrgentes ();
    // entrega la lista regulares en forma de vector
    std::vector <Tarea*> listarTareasRegulares ();
    // Devuelve un vector con todas las subtareas de un tarea.
    std::vector<Tarea*> listarSubTareas(int id);
    //void completarTarea (bool listaUrgente); <--- Preguntar al profe si esas tareas se eliminan
    // void descompletarTarea (int idTarea); <--- Preguntar al profe sobre su gestion
    //void completarSubtarea 
    //void descompletarSubTarea
    //void completarTarea ();
};

#endif