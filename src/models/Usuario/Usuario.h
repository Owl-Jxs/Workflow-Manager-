#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <functional>
#include <stdexcept>

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
	size_t hashContrasena; // Almacena el hash de la contraseña

    int validarId(int id);                                    // valida el id y lo devuelve si es correcto
    const std::string& validarNombre(const std::string& nombre); // valida el nombre y lo devuelve si es correcto
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
    
	void setHashContrasena(const std::string& contrasena);
	void setHashDirecto(size_t hash); // Permite establecer el hash directamente (para cargar desde archivo)
	size_t getHashContrasena() const;
};

#endif
