#ifndef ASIGNACIONDATABASE_H
#define ASIGNACIONDATABASE_H

#include "../../structures/Asignacion/ListaAsignaciones/ListaAsignaciones.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

class AsignacionDataBase {
private:
    static const std::string FILENAME_ASIGNACIONES; // Nombre del archivo para almacenar las asignaciones
public:
    AsignacionDataBase();
    ~AsignacionDataBase();
    void guardarAsignacionesEnArchivo(ListaAsignaciones* lista);
    ListaAsignaciones* cargarAsignacionesDesdeArchivo();
    void agregarAsignacion(int idTarea, int idUsuario);
    void eliminarAsignacion(int idTarea, int idUsuario);
    bool buscarAsignacion(int idTarea, int idUsuario);
};
// Contenido de la clase AsignacionDataBase

#endif // ASIGNACIONDATABASE_H