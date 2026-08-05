#include "Tarea.h"

///constructores y destructores
Tarea::Tarea (int id, std::string descripcion, std::string prioridad, std::string estado, int idResponsable, int padreId) {
    this->idTarea = id;
    this->descripcionTarea = descripcion;
    this->prioridadTarea = prioridad;
    this->estadoTarea = estado;
    this->idUsuarioResponsable = idResponsable;
    this->idPadre = padreId;
    this->padre = nullptr;
    this->primerSubTarea = nullptr;
    this->siguienteSubTarea = nullptr;
    this->cantidadSubTareas = 0;
}

Tarea::~Tarea () {
    if (this->primerSubTarea != nullptr)        delete this->primerSubTarea;
    if (this->siguienteSubTarea != nullptr)     delete this->siguienteSubTarea;
}

///getters and setters
void Tarea::setIdTarea (int id) {
    this->idTarea = id;
}

int Tarea::getIdTarea () const {
    return this->idTarea;
}

void Tarea::setDescripcionTarea (std::string descripcion) {
    this->descripcionTarea = descripcion;
}

std::string Tarea::getDescripcionTarea () const {
    return this->descripcionTarea;
}

void Tarea::setPrioridad (std::string prioridad) {
    this->prioridadTarea = prioridad;
}

std::string Tarea::getPrioridad () const {
    return this->prioridadTarea;
}

void Tarea::setEstado (std::string estado) {
    this->estadoTarea = estado;
}

std::string Tarea::getEstado () const {
    return this->estadoTarea;
}

void Tarea::setIdUsuarioResponsable (int idResponsable) {
    this->idUsuarioResponsable = idResponsable;
}

int Tarea::getIdUsuarioResponsable () const {
    return this->idUsuarioResponsable;
}

void Tarea::setPadre (Tarea* padreTarea) {
    this->padre = padreTarea;
}

Tarea* Tarea::getPadre () const {
    return this->padre;
}

int Tarea::getPadreId () const {
    if (this->padre != nullptr)      return this->padre->idTarea;
    return this->idPadre;
}

void Tarea::agregarSubTarea (Tarea* subTarea) {
    if (subTarea == nullptr || subTarea == this) {
        return;
    }
    if (this->primerSubTarea == nullptr) {
        this->primerSubTarea = subTarea;
    } else {
        Tarea* actual = this->primerSubTarea;
        while (actual->siguienteSubTarea != nullptr) {
            actual = actual->siguienteSubTarea;
        }
        actual->siguienteSubTarea = subTarea;
    }
    subTarea->padre = this;
    subTarea->idPadre = this->idTarea;
    this->cantidadSubTareas++;
}

Tarea* Tarea::getPrimerSubTarea () const {
    return this->primerSubTarea;
}

void Tarea::setSiguienteSubTarea (Tarea* siguiente) {
    this->siguienteSubTarea = siguiente;
}

Tarea* Tarea::getSiguienteSubTarea () const {
    return this->siguienteSubTarea;
}

int Tarea::getCantidadSubTareas () const {
    return this->cantidadSubTareas;
}

///conexion con el archivo CSV
std::string Tarea::toCSV () const {
    return std::to_string(this->idTarea) + "," + this->descripcionTarea + ","
         + this->prioridadTarea + "," + this->estadoTarea + ","
         + std::to_string(this->idUsuarioResponsable) + ","
         + std::to_string(this->getPadreId());
}

Tarea Tarea::fromCSV (const std::string& linea) {
    std::string campos[6];
    int campoActual = 0;
    std::string buffer;
    bool dentroDeComillas = false;

    for (char c : linea) {
        if (c == '"') {
            dentroDeComillas = !dentroDeComillas;
        } else if (c == ',' && !dentroDeComillas) {
            campos[campoActual++] = buffer;
            buffer.clear();
        } else {
            buffer += c;
        }
    }
    campos[campoActual] = buffer;

    int id = std::stoi(campos[0]);
    std::string descripcion = campos[1];
    std::string prioridad = campos[2];
    std::string estado = campos[3];
    int idResponsable = std::stoi(campos[4]);
    int padreId = std::stoi(campos[5]);

    return Tarea(id, descripcion, prioridad, estado, idResponsable, padreId);
}
