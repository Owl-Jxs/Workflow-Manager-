#ifndef TAREA_H
#define TAREA_H

#include <iostream>

class Tarea {
private:
    int idTarea;
    std::string descripcionTarea;
    bool completada;
    bool urgente;
    Tarea* subTarea;
    Tarea* siguienteSubTarea;
    int cantidadSubTareas;
public:
//constructors and destructors
    Tarea(int id, std::string descripcion, bool urgente);
    ~Tarea();
//getters and setters
    void setIdTarea(int id);
    int getIdTarea() const; 

    void setDescripcionTarea(std::string descripcion);
    std::string getDescripcionTarea() const;
    
    void setCompletada(bool completada);
    bool isCompletada() const;
    
    void setUrgente(bool urgente);
    bool isUrgente() const;
    
    void agregarSubTarea(Tarea* subTarea);
    Tarea* getSubTarea() const;
    
    void setSiguienteSubTarea(Tarea* siguienteSubTarea);
    Tarea* getSiguienteSubTarea() const;
};//end class



#endif // TAREA_H