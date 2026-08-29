#ifndef  ASIGNACIONCONTROLLER_H
#define ASIGNACIONCONTROLLER_H
#include <vector>
#include "../../persistence/AsignacionDataBase/AsignacionDataBase.h"
#include "../../structures/Asignacion/ListaAsignaciones/ListaAsignaciones.h"
#include <vector>

class AsignacionController {
private:
    ListaAsignaciones* listaAsignaciones;
    AsignacionDataBase* archivosAsignaciones; 
public:
//constructores y destructor
    AsignacionController ();
    ~AsignacionController();

//manipular los archivos de datos
    void guardarAsignaciones();
    void cargarAsignaciones();

    void guardarAsignacionesCompletadas (std::vector<std::pair<int, int>> listaAsignacionesRehacer);
    void deshacerAsignacionesCompletadas (std::vector <std::pair<int, int>> listaAsignacionesRehacer); 
//manipular asignaciones individuales
    void agregarAsignacion(int idTarea, int idUsuario);
    void eliminarAsignacion(int idTarea, int idUsuario);
    void eliminarAsignacionPorIdTarea (int idTarea);
    void eliminarAsignacionPorIdUsuario (int idUsuario);

    bool buscarAsignacion(int idTarea, int idUsuario);

    std::vector<std::pair <int, int>> getAsignacionesResponsablesDeTarea (std::vector <int> idsTareas);
    std::vector<std::pair <int, int>> getAsignacionesTareasPorUsuario (std::vector <int> idsResponsables);


};

#endif