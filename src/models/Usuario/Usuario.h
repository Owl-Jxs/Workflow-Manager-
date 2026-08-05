#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario {
public:
    enum class Rol {
        ADMINISTRADOR,
        USUARIO_NORMAL
    };

private:
    int idUsuario;
    std::string nombreUsuario;
    Rol rolUsuario;

public:
    // Constructores y destructor
    Usuario();
    Usuario(int id, const std::string& nombre, Rol rol = Rol::USUARIO_NORMAL);
    ~Usuario();

    // Getters y Setters
    void setId(int id);
    int getId() const;

    void setNombre(const std::string& nombre);
    std::string getNombre() const;

    void setRol(Rol rol);
    Rol getRol() const;
};

#endif
