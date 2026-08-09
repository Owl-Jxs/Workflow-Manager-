#ifndef TAREA_DATABASE_H
#define TAREA_DATABASE_H

#include "../../structures/ColaFIFO/colaFIFO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map> // para cargar las listas de tareas mucho mas rapido
#include <vector> 
class TareaDataBase {
private:
    ColaFIFO* listaTareasRegulares; // Lista de tareas regulares
    ColaFIFO* listaTareasUrgentes; // Lista de tareas prioritarias
    static const std::string FILENAME_TAREAS_REGULARES; // Nombre del archivo para guardar y cargar la base de datos
    static const std::string FILENAME_TAREAS_URGENTES; // Nombre del archivo para guardar y cargar la base de datos
//funciones para manipular directamente los archivos
    void guardarArbol (Tarea* arbolTarea, std::ofstream& archivo);
    void guardarLista (ColaFIFO* lista, std::string nombreArchivo);
    void guardarNuevaTarea (Tarea* tarea, std::string nombreArchivo);
    ColaFIFO* cargarLista (std::string nombreArchivo, bool prioridad);
//funciones para buscar en los archivos
    Tarea* buscarSubTarea (Tarea* tareaBuscada, int idBuscado);
 
    public:
//Constructores y destructores
    TareaDataBase();
    ~TareaDataBase();
//Funciones para manipular la base de datos de tareas
    void guardarBaseDeDatos();
    void cargarBaseDeDatos();
//funciones para manipular tareas
    void agregarTarea(Tarea* tarea);
    void agregarSubTarea (Tarea* tarea, int idTareaPadre, bool perteneceListaUrgente);
    //void completarTarea (bool listaUrgente); <--- Preguntar al profe si esas tareas se eliminan
    // void descompletarTarea (int idTarea); <--- Preguntar al profe sobre su gestion
    Tarea* completarTarea ();
   
};  

#endif