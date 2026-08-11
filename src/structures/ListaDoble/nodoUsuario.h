#ifndef NODOUSUARIO_H
#define NODOUSUARIO_H

class Usuario;

class nodoUsuario {
public:
	Usuario* datos;
	nodoUsuario* siguiente;
	nodoUsuario* anterior;

	nodoUsuario(Usuario* datos = nullptr,
		nodoUsuario* siguiente = nullptr,
		nodoUsuario* anterior = nullptr)
		: datos(datos), siguiente(siguiente), anterior(anterior) {}

	~nodoUsuario() = default;
};

#endif