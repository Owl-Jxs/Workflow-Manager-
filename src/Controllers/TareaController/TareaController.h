#ifndef TAREACONTROLLER_H
#define TAREACONTROLLER_H

#include "../../persistence/TareaDataBase/TareaDataBase.h"
#include "../../structures/Colas/ColaFIFO/ColaFIFO.h"
#include "../../structures/Colas/ColaPrioridad/ColaPrioridad.h"
#include <vector>

class TareaController {
private:
    ColaFIFO* listaTareasRegulares; // Lista de tareas regulares
    ColaFIFO* listaTareasUrgentes; // Lista de tareas prioritarias
    std::vector <Tarea*> listaTareasEnProceso;
    ColaPrioridad* listaTareasEnRevision;
    ColaFIFO* listaTareasCompletadas;
    TareaDataBase* archivosTareas; //la data base de tareas
    int ultimoId;
    const int cantidadCiclosParaEscalar; 

//operaciones con arbol de tareas
    void aplanarArbol(Tarea* tarea, std::vector<Tarea*>& resultado) const;
    void aplanarIdsArbol (Tarea* tarea, std::vector<int>& resultado);
    void cambiarEstadoArbol (Tarea* tarea, std::string nuevoEstado);

//listar Tareas en vectores
    std::vector <Tarea*> listarTareas (Cola* cola) const; 
    std::vector <Tarea*> listarSubTareas (Tarea* tarea) const;    
    bool subTareasCompletas (Tarea* tarea);
    Tarea* buscarTareaEnLista (int idTarea, Cola* lista);
  

public:
    TareaController ();
    ~TareaController ();

//guardar/cargar todos los archivos
    void guardarArchivos ();
    void cargarArchivos ();

//funciones para manipular las tareas de la lista
    void agregarTarea(Tarea* tarea);
    void completarSubTarea (int idTarea);
    std::vector <int> delegarTarea ();// delega una tarea de Por hacer a un usuario y la transorma en En proceso
    void deshacerDelegacion (int idTarea); // devuelve una tarea de En proceso a Por hacer
    int mandar_A_Revision (int idTarea); // manda una tarea en Proceso A En revision, para que un admin la acepte o rechace
    void revisionExitosa (); // si una tarea es revisada exitosamente la pasa a la lista de tareas completada
    void anularRevisionExitosa (int idTarea);  //devuelve una tarea completada a en proceso
    void rechazarRevision (); 
    void anularRechazo (int idTarea);

//entregar elementos 
    Tarea* getSiguienteTareaProcesable () const; //entrega la siguiente tarea que se entregara
//buscar tareas
    Tarea* buscarTareaPorHacer (int idTarea);
    Tarea*  buscarTareaEnProceso (int idTarea);
    Tarea* buscarTareaEnRevision (int idTarea);
    Tarea* buscarTareaCompletada (int idTarea);
    
    Tarea* eliminarTarea(int idTarea);// Extrae una tarea de la cola indicada sin destruirla (para deshacer). Devuelve la tarea o nullptr.
    Tarea* escalarTarea();  // Mueve una tarea de la cola regular a la cola urgente (escalamiento SLA).
    int getUltimoId () const; //retoorna el ultimo id

//entregar listas
    std::vector <Tarea*> listarTareasUrgentes () const; // entrega la lista urgente en forma de vector
    std::vector <Tarea*> listarTareasRegulares () const; // entrega la lista regulares en forma de vector
    std::vector <Tarea*> listarTareasEnProceso () const; // entrega la lista en proceso en forma de vector
    std::vector <Tarea*> listarTareasEnRevision () const; // entrega la lista en revision en forma de vector
    std::vector <Tarea*> listarTareasCompletadas () const;//listar tareas en completas
    std::vector <Tarea*> listarTodasLasTareasActivas () const; //listar TODAS las tareas activas sin importar su estado o su priorida
    std::vector <int> listarIdsArbol (Tarea* tarea);//devuelve un vector con los ids de las tareas;

    bool listaPorHacerVacia ();
    bool listaEnProcesoVacia ();
    bool listaEnRevisionVacia ();
    bool listaCompletadasVacia ();
};

#endif