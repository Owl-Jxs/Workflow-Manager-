#ifndef TAREA_DATABASE_H
#define TAREA_DATABASE_H

#include "../../structures/ColaFIFO/colaFIFO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map> // para cargar las listas de tareas mucho mas rapido
#include <vector> //para guardar temporalmente los datos cargados 
class TareaDataBase {
private:
    static const std::string FILENAME_TAREAS_REGULARES; // Nombre del archivo para guardar y cargar la base de datos
    static const std::string FILENAME_TAREAS_URGENTES; // Nombre del archivo para guardar y cargar la base de datos
    static const std::string ENUM_PRIORIDAD_TAREA [2]; //variable para guardar la prioridad

//funciones auxiliares para guardado y cargado
    std::string formularLinea (Tarea* tarea);
    void guardarArbol (Tarea* arbolTarea, std::ofstream& archivo);
    void guardarLista (ColaFIFO* lista, std::string nombreArchivo);
    void guardarNuevaTarea (Tarea* tarea, std::string nombreArchivo);
    ColaFIFO* cargarLista (std::string nombreArchivo);

//funciones para buscar en los archivos
    Tarea* buscarSubTarea (Tarea* tareaBuscada, int idBuscado);
 
public:
//Constructores y destructores
    TareaDataBase();
    ~TareaDataBase();

//Funciones para manipular la base de datos de tareas
    void guardarListaEnArchivo (ColaFIFO* listaCompleta, bool perteneceListaUrgente);
    ColaFIFO* cargarListaDelArchivo (bool perteneceListaUrgente);

//funciones para manipular tareas
    void guardarNuevaTareaEnArchivo (Tarea* tarea, bool perteneceListaUrgente);
    void guardarguardarNuevaSubTareaEnArchivo (Tarea* tarea, bool perteneceListaUrgente);
    //void completarTarea (bool listaUrgente); <--- Preguntar al profe si esas tareas se eliminan
    // void descompletarTarea (int idTarea); <--- Preguntar al profe sobre su gestion
    //void completarSubtarea 
    //void descompletarSubTarea
    //Tarea* completarTarea ();
   
};  

#endif