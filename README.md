# WorkflowManager — Proyecto 1 de Estructuras de Datos
# Bitácora de Inteligencia Artificial:
Isabella Cubillo
1. Implementación de la estructura ListaDoble:
   
   Estructura:
   Clase ListaDoble y su relación con la estructura nodoUsuario, utilizada para almacenar objetos de tipo Usuario mediante una lista doblemente enlazada.
   
   Prompt ingresado:
   "Estoy implementando una lista doblemente enlazada para almacenar usuarios dentro de mi proyecto en C++. Ya tengo creado el nodo nodoUsuario y cuento con el diagrama UML de referencia. ¿Puedes ayudarme a revisar qué métodos y atributos necesito implementar en ListaDoble y orientarme sobre cómo organizarlos?"

   Revisión y ajuste humano aplicado:
   Se utilizó la respuesta de la IA como guía para comprender la implementación de una lista doblemente enlazada. Luego, se revisó la estructura del proyecto para adaptar los nombres de archivos, rutas de inclusión y la interacción con la clase Usuario.

2. Implementación de métodos en ListaDoble

   Estructura:
   agregarUsuario(Usuario* usuario)
   buscarPorId(int id)
   actualizarUsuario(int id, Usuario* usuario)
   eliminarUsuario(int id)
   listarUsuarios()

   Prompt ingresado:
   "Ya tengo definida la estructura básica de mi clase ListaDoble. Necesito orientación para implementar correctamente los métodos agregarUsuario, buscarPorId,         actualizarUsuario, eliminarUsuario y listarUsuarios, especialmente en el manejo de los punteros siguiente y anterior."

   Revisión y ajuste humano aplicado:
   El código fue revisado para verificar la lógica de recorrido de la lista y el manejo de los punteros siguiente y anterior.

3. Implementación de UsuarioController

   Estructura:
   Coordinar la gestión de usuarios entre la estructura ListaDoble y la persistencia mediante UsuarioDataBase.

   Prompt ingresado:
   "Necesito implementar una clase llamada UsuarioController sin modificar innecesariamente las estructuras existentes. La clase debe contener los siguientes atributos:

   ListaDoble* listaUsuarios
   UsuarioDataBase* archivoUsuarios

   Y los siguientes métodos:

   Constructor y destructor.
   guardarUsuarios().
   cargarUsuarios().
   agregarUsuario(Usuario* usuario).
   actualizarUsuario(int id, Usuario* usuario): bool.
   eliminarUsuario(int idUsuario).
   buscarUsuarioPorId(int idUsuario): Usuario*.

   Ayúdame primero a entender cómo debe relacionarse el controlador con las clases existentes. Después propón una implementación compatible con esta arquitectura. Explícame    qué partes debo adaptar según los métodos que ya existen en mi proyecto."

   Revisión y ajuste humano aplicado:
   Se revisaron las clases existentes del proyecto para verificar los nombres y parámetros reales de los métodos de ListaDoble, Usuario y UsuarioDataBase. La propuesta         obtenida se adaptó a la arquitectura que ya estaba siendo desarrollada por el equipo.

4. Implementación UsuarioView

   Estructura:
   Diseño y apoyo en la implementación de la clase UsuarioView, encargada de la interacción por consola para la gestión de usuarios.

    Prompt ingresado:
   "Estoy trabajando en un sistema de consola en C++ y necesito implementar una clase llamada UsuarioView. El proyecto ya cuenta con un UsuarioController, una clase Usuario       y utilidades para validar entradas.
   La clase debe tener los siguientes atributos:

   UsuarioController* uc
   GestorHistorial* historial

   Debe incluir los siguientes métodos:

   Constructor.
   mostrarMenuAdministrador().
   mostrarMenuUsuarioNormal().
   leerNuevoUsuario(): Usuario*.
   leerIdUsuario(): int.

   Ayúdame a organizar las responsabilidades correctamente. La vista debe encargarse de mostrar opciones y solicitar datos, mientras que la lógica de gestión debe             permanecer en el controlador. Propón una estructura de menú clara y considera validaciones para evitar que el programa falle cuando el usuario ingrese datos inválidos."
   
   Revisión y ajuste humano aplicado:
   La propuesta fue utilizada como guía para separar correctamente la lógica de presentación de la lógica del sistema. Se revisó que UsuarioView no duplicara                   responsabilidades propias de UsuarioController.

5. Implementación MenuPrincipal:

   Estructura:
   Encargado de coordinar el inicio de sesión y mostrar las opciones correspondientes según el rol del usuario.

   Prompt ingresado:
   "Mi equipo ya cuenta con las clases UsuarioController, TareaController, AsignacionController y GestorHistorial.
   Necesito implementar una clase MenuPrincipal con los siguientes atributos:

   UsuarioController* uc
   TareaController* tc
   AsignacionController* ac
   GestorHistorial* historial

   Debe incluir:

   Constructor.
   iniciarSesion(), que solicite un ID de usuario y valide su existencia y rol.
   mostrarMenuSegunRol(Usuario* usuario).
   mostrarMenuAdministrador().
   mostrarMenuUsuarioNormal().

   Ayúdame a definir el flujo general del programa respetando estos requisitos:

   El administrador tiene acceso completo al sistema.
   El usuario normal tiene acceso restringido.
   El sistema debe manejar entradas inválidas sin cerrarse inesperadamente.

   No generes una arquitectura completamente nueva. La solución debe integrarse con los controladores y estructuras que ya existen en el proyecto. Explícame cómo conectar       las clases y qué validaciones deberían realizarse."

   Revisión y ajuste humano aplicado:
   La integración final requirió ajustar llamadas entre MenuPrincipal, los controladores y las vistas ya existentes en el proyecto.

6. Actualización de PruebasTarea.cpp con IA.

   Estructura:
   Poner a prueba las distintas funciones realizadas por Tarea

   Prompt ingresado:
   "Actualiza el código PruebasTarea.cpp conforme a los cambios que se hayan realizado en el respectivo módulo de Tareas."

   Revisión y ajuste humano aplicado:
   Se revisó y verificó que los ajustes implementados por la IA estuvieran correctos en el código.
   
   
   




   
