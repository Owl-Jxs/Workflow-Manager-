#ifndef TAREA_H
#define TAREA_H

#include <iostream>
static const std::string STATE [3] = {
    "POR HACER",
    "EN PROGRESO",
    "COMPLETADA"
};

class Tarea {
private:
    int idTarea;
    std::string descripcionTarea;
    //ListaUsuarios* Encargados; <- Lista con los encargados de la tarea (Se ancla una lista de usuarios?)
    bool completada;
    std::string estado;
    Tarea* subTarea;
    Tarea* siguienteSubTarea;
    int cantidadSubTareas;


public:
//constructors and destructors
    Tarea(int id, std::string descripcion, std::string estado);
    ~Tarea();
//getters and setters
    void setIdTarea(int id);
    int getIdTarea() const; 

    void setDescripcionTarea(std::string descripcion);
    std::string getDescripcionTarea() const;
    
    void setCompletada(bool completada);
    bool isCompletada() const;
    
    void setSTate (std::string state);
    std::string getState() const;
    
    void agregarSubTarea(Tarea* subTarea);
    Tarea* getSubTarea() const;
    
    void setSiguienteSubTarea (Tarea* siguienteSubTarea);
    Tarea* getSiguienteSubTarea () const;
};//end class



#endif // TAREA_H