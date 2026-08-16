#include "ListaDoble.h"
using namespace std;

//lista doble
ListaDoble::ListaDoble()
{
	cabeza = nullptr;
	cola = nullptr;
	cantidad = 0;
}



ListaDoble::~ListaDoble()
{
	nodoUsuario* actual = cabeza;

	while (actual != nullptr)
	{
		nodoUsuario* siguiente = actual->siguiente;

		delete actual->datos;
		delete actual;

		actual = siguiente;
	}

	cabeza = nullptr;
	cola = nullptr;
	cantidad = 0;
}



void ListaDoble::agregarUsuario(Usuario* usuario)
{
	if (usuario == nullptr)
	{
		return;
	}

	nodoUsuario* nuevo = new nodoUsuario(usuario);

	// Caso 1: la lista está vacía
	if (cabeza == nullptr)
	{
		cabeza = nuevo;
		cola = nuevo;
	}
	else
	{
		// El nuevo nodo apunta hacia atrás a la cola
		nuevo->anterior = cola;

		// La antigua cola apunta al nuevo nodo
		cola->siguiente = nuevo;

		// La nueva cola es el nuevo nodo
		cola = nuevo;
	}

	cantidad++;
}



// BUSCAR USUARIO POR ID


Usuario* ListaDoble::buscarPorId(int id)
{
	nodoUsuario* actual = cabeza;

	while (actual != nullptr)
	{
		if (actual->datos->getId() == id)
		{
			return actual->datos;
		}

		actual = actual->siguiente;
	}

	return nullptr;
}



// ACTUALIZAR USUARIO


bool ListaDoble::actualizarUsuario(int id, Usuario* usuario)
{
	if (usuario == nullptr)
	{
		return false;
	}

	nodoUsuario* actual = cabeza;

	while (actual != nullptr)
	{
		if (actual->datos->getId() == id)
		{
			if (actual->datos != usuario) // evita borrar y reasignar el mismo objeto (use-after-free)
			{
				delete actual->datos;
				actual->datos = usuario;
			}

			return true;
		}

		actual = actual->siguiente;
	}

	return false;
}



// ELIMINAR USUARIO


bool ListaDoble::eliminarUsuario(int id)
{
	nodoUsuario* actual = cabeza;

	while (actual != nullptr)
	{
		if (actual->datos->getId() == id)
		{
			// ----------------------------------
			// Actualizar cabeza
			// ----------------------------------

			if (actual == cabeza)
			{
				cabeza = actual->siguiente;
			}

			// ----------------------------------
			// Actualizar cola
			// ----------------------------------

			if (actual == cola)
			{
				cola = actual->anterior;
			}

			// ----------------------------------
			// Conectar nodo anterior
			// ----------------------------------

			if (actual->anterior != nullptr)
			{
				actual->anterior->siguiente = actual->siguiente;
			}

			// ----------------------------------
			// Conectar nodo siguiente
			// ----------------------------------

			if (actual->siguiente != nullptr)
			{
				actual->siguiente->anterior = actual->anterior;
			}

			delete actual->datos;
			delete actual;

			cantidad--;

			return true;
		}

		actual = actual->siguiente;
	}

	return false;
}


// ==========================================
// LISTAR USUARIOS
// ==========================================

vector<Usuario*> ListaDoble::listarUsuarios()
{
	vector<Usuario*> usuarios;

	nodoUsuario* actual = cabeza;

	while (actual != nullptr)
	{
		usuarios.push_back(actual->datos);

		actual = actual->siguiente;
	}

	return usuarios;
}