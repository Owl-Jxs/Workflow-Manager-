#ifndef ASIGNACIONDATABASE_H
#define ASIGNACIONDATABASE_H

#include "../../structures/Asignacion/ListaAsignaciones/ListaAsignaciones.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector> 
class AsignacionDataBase {
private:
    static const std::string FILENAME_ASIGNACIONES; // Nombre del archivo para almacenar las asignaciones
    static const std::string FILENAME_ASIGNACIONES_COMPLETADAS; //nombre para las asignaciones completadas
    void eliminarAsignacionesDeArchivo (std::vector <std::pair <int, int>> listaAsignacionesRehacer, std::string nombreArchivo);
    void agregarAsignacionesDeArchivo (std::vector <std::pair <int, int>> listaAsignacionesRehacer, std::string nombreArchivo);
public:
    AsignacionDataBase();
    ~AsignacionDataBase();
    void guardarAsignacionesEnArchivo(ListaAsignaciones* lista);
    ListaAsignaciones* cargarAsignacionesDesdeArchivo();
    void guardarAsignacionesCompletadas (std::vector <std::pair<int, int>> listaAsignacionesRehacer);
    void deshacerAsignacionesCompletadas (std::vector <std::pair<int, int>> listaAsignacionesRehacer); 

    void agregarAsignacion(int idTarea, int idUsuario);
    void eliminarAsignacion(int idTarea, int idUsuario);
};
// Contenido de la clase AsignacionDataBase

#endif // ASIGNACIONDATABASE_H