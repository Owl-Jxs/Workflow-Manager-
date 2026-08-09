#include "Tarea.h"
//atributos static
 const std::string Tarea::ESTADO [3] = {
    "POR HACER",
    "EN PROCESO",
    "COMPLETADA"
};
const int Tarea::sinPadre = -1;

//private functions
void Tarea::esAncestro (const Tarea* buscado) const {
    const Tarea* actual = this->TareaPadre;
    while (actual != nullptr) {
        if (actual == buscado) {
            throw std::invalid_argument ("Operación inválida: 'buscado' es un ancestro de 'this', lo que crearía un ciclo en la jerarquía.");
        }
        actual = actual->TareaPadre;
    }
}

void Tarea::validarId (int id) {
    if (id < 0) {
        throw std::invalid_argument ("ID inválido: debe ser un número entero no negativo.");
    }
}

void Tarea::validarDescripcion (std::string descripcion) {
    if (descripcion.empty()) {
        throw std::invalid_argument ("Descripción inválida: no puede estar vacía.");
    }
}

void Tarea::validarEstado (std::string estado) {
    bool estadoValido = false;
    for (const auto& e : ESTADO) {
        if (estado == e) {
            estadoValido = true;
            break;
        }
    }
    if (!estadoValido) throw std::invalid_argument ("Estado inválido: debe ser 'POR HACER', 'EN PROCESO' o 'COMPLETADA'.");
}

///constructores y destructores
Tarea::Tarea (int id, std::string descripcion, bool prioridad, std::string estado) {
//validamos los parametros
    validarId(id);
    validarDescripcion(descripcion);
    validarEstado(estado);
//asignamos los parametros a los atributos
    this->idTarea = id;
    this->descripcionTarea = descripcion;
    this->prioridadTarea = prioridad;
    this->estadoTarea = estado;
//asignamos valores por defecto a los atributos relacionados con las subtareas
    this->idTareaPadre = sinPadre; // Inicializar con un valor que indique que no tiene padre
    this->primerSubTarea = nullptr;
    this->siguienteSubTarea = nullptr;
    this->cantidadSubTareas = 0;
    this->TareaPadre = nullptr; // Inicializar con nullptr para indicar que no tiene padre
}

Tarea::~Tarea () {
    if (this->primerSubTarea != nullptr) {
        Tarea* actual = this->primerSubTarea;
        while (actual != nullptr) {
            Tarea* siguiente = actual->siguienteSubTarea;
            delete actual;
            actual = siguiente;
        } 
    }
}

///getters and setters
void Tarea::setIdTarea (int id) {
    validarId(id); //validamos el parametro
    this->idTarea = id;
}

int Tarea::getIdTarea () const {
    return this->idTarea;
}

void Tarea::setDescripcionTarea (std::string descripcion) {
    validarDescripcion(descripcion); //validamos el parametro
    this->descripcionTarea = descripcion;
}

std::string Tarea::getDescripcionTarea () const {
    return this->descripcionTarea;
}

void Tarea::setPrioridad (bool prioridad) {
    this->prioridadTarea = prioridad;
}

bool Tarea::getPrioridad () const {
    return this->prioridadTarea;
}

void Tarea::setEstado (std::string estado) {
    validarEstado(estado); //validamos el parametro
    this->estadoTarea = estado;
}

std::string Tarea::getEstado () const {
    return this->estadoTarea;
}


int Tarea::getPadreId () const {
    return this->idTareaPadre;
}

void Tarea::agregarSubTarea (Tarea* subTarea) {
    if (subTarea == nullptr || subTarea == this)    throw std::invalid_argument ("Subtarea inválida: no puede ser nula ni la misma tarea.");
    if (subTarea->idTareaPadre != sinPadre) throw std::invalid_argument ("Subtarea inválida: ya tiene un padre asignado.");
    if (subTarea->siguienteSubTarea != nullptr) throw std::invalid_argument ("Subtarea inválida: ya tiene un hermano siguiente asignado.");
    esAncestro(subTarea); // Verifica si 'subTarea' es un ancestro de 'this'

    if (this->primerSubTarea == nullptr) {
        this->primerSubTarea = subTarea;
    } else {
        Tarea* actual = this->primerSubTarea;
        while (actual->siguienteSubTarea != nullptr) {
            if (actual->siguienteSubTarea == subTarea) {
                throw std::invalid_argument ("Subtarea inválida: ya está agregada como subtarea.");
            }
            actual = actual->siguienteSubTarea;
        }
        actual->siguienteSubTarea = subTarea;
    }
    subTarea->idTareaPadre = this->idTarea;
    subTarea->TareaPadre = this; // Establecer el puntero al padre 
    this->cantidadSubTareas++;
}

Tarea* Tarea::getPrimerSubTarea () const {
    return this->primerSubTarea;
}

void Tarea::setSiguienteSubTarea (Tarea* siguiente) {
    if (siguiente == nullptr || siguiente == this)  throw std::invalid_argument ("Subtarea inválida: no puede ser nula ni la misma tarea.");
    if (siguiente->idTareaPadre != sinPadre) throw std::invalid_argument ("Subtarea inválida: ya tiene un padre asignado.");
    if (siguiente->siguienteSubTarea != nullptr) throw std::invalid_argument ("Subtarea inválida: ya tiene un hermano siguiente asignado.");
    esAncestro(siguiente); // Verifica si 'siguiente' es un ancestro de 'this'

    siguiente->idTareaPadre = this->idTareaPadre; // hereda el padre del hermano
    siguiente->TareaPadre = this->TareaPadre; // hereda el puntero al padre
    this->siguienteSubTarea = siguiente;
}

Tarea* Tarea::getSiguienteSubTarea () const {
    return this->siguienteSubTarea;
}
void Tarea::setIdPadre (int id) {
    validarId (id);
    this ->idTareaPadre = id;
}
int Tarea::getIdPadre () const {
    return this->idTareaPadre;
}

void Tarea::setTareaPadre (Tarea* padre) {
    if (padre == this)  throw std::invalid_argument ("Padre inválido: no puede ser nulo ni la misma tarea.");
   esAncestro(padre); // Verifica si 'padre' es un ancestro de 'this'
    this->TareaPadre = padre;
    if (padre != nullptr) {
        this->idTareaPadre = padre->getIdTarea(); 
    } else {
        this->idTareaPadre = sinPadre; // Si no hay padre, asignar sinPadre
    }
}

Tarea* Tarea::getTareaPadre () const {
    return this->TareaPadre;
} 

int Tarea::getCantidadSubTareas () const {
    return this->cantidadSubTareas;
}