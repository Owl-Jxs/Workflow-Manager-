#ifndef TAREA_DATABASE_H
#define TAREA_DATABASE_H

#include "../../structures/Colas/Cola.h"
#include "../../models/Tarea/Tarea.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map> // para cargar las listas de tareas mucho mas rapido
#include <vector> //para guardar en proceso 

class TareaDataBase {
    
private:
    static const std::string FILENAME_TAREAS_ACTIVAS; //nombre del archivo
    static const std::string FILENAME_TAREAS_COMPLETADAS; // nombre para las tareas completas
    static const std::string ENUM_PRIORIDAD_TAREA [2]; //variable para guardar la prioridad

//funciones auxiliares para guardado y cargado
    std::string formularLinea (Tarea* tarea); //corregida

    void guardarArbol (Tarea* arbolTarea, std::ofstream& archivo); //corregida
    void guardarLista (Cola* lista, std::ofstream& archivo); //corregida
    void guardarVector (const std::vector<Tarea*>& enProceso, std::ofstream& archivo); //corregida
    void guardarNuevaTarea (Tarea* tarea, std::ofstream& archivo); 

//funciones para eliminar una tarea
    void eliminarTarea (std::vector<int> idArbolTarea, std::string nombreArchivo);

public:
//Constructores y destructores
    TareaDataBase();
    ~TareaDataBase();

//Funciones para manipular la base de datos de tareas
    void cargarTareasActivas(Cola* regulares, Cola* urgentes, std::vector<Tarea*>& enProceso, Cola* enRevision, int &ultimoId); //corregida
    void guardarTareasActivas(Cola* regulares, Cola* urgentes, const std::vector<Tarea*>& enProceso, Cola* enRevision); //corregida
    void guardarTareasCompletadas (Cola* listaTareasCompletas); //corregida
    void eliminarTareaActiva (std::vector<int> idArbolTarea);
//funciones para manipular tareas
    void guardarNuevaTareaEnArchivo (Tarea* tarea);
    void actualizarEstadoTarea (Tarea* tareaActualizada);
    void registrarTareaCompletada (Tarea* tarea);
    void eliminarRegistroTareaCompletada  (std::vector<int> idArbolTarea);
    void cargarTareasCompletadas (Cola* completadas, int &ultimoId);
   
};  

#endif