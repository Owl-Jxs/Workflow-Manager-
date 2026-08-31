# WorkflowManager — Proyecto 1 de Estructuras de Datos

# analisis de algoritmos de ordenamiento:

1. MergeSort:
   a. Relación en el tiempo de ejecucion :
      Si se tiene un vector de tamaño n, el tiempo de ejecución se define según esta formula:
               { O(1) si n <= 1           }
         T(n) ={ 2T (n/2) + O(n) si n > 1 }
   
   b. Desglose de la formula utilizada:
      Esta formula es de tipo "Vence y venceras" por lo que cuando se tiene una cantidad > 1 el algoritmo consta de dos         partes, la recursiva y la no recursiva. Las partes que la formula que este algoritmo usa son:
      T(n) = a*T (n\b) + F(n),    donde :
      a = cantidad de llamadas recursivas por nivel
      b = tamano en el que se dividira el algoritmo en la siguiente sub-llamada
      f(n) = el trabajo que se realizara en el nivel actual, esta es la parte independiente (no recursiva) de la formula.

   c. Explicacion en este caso concreto:
      Para explicar este algoritmo en concreto se define que cuando n > 1 se define lo siguiente
      T(n) ={ 2T (n/2) + O(n) si n > 1 },    donde:
      a = 2
      b = 2
      f(n) = O(n)
      de aquí podemos concluir que el algoritmo define su parte recursiva a la mitad (O sea, por cada llamada recursiva         su tamaño es la mitad del actual). También se observa que se llama 2 veces recursivamente, ademas de que cada nivel       Tiene un nivel de complejidad de O(n).

   d. Tabla de casos:
   | Escenario     | Complejidad Temporal | Complejidad Espacial | Explicación Breve | 
   | Mejor Caso    | O (n*log(n) )        | O(n)                 | el arreglo se divide aunque ya este ordenado
   | Caso Promedio | O (n*log(n) )        | O(n)                 | las mitades y fusiones son proporciones constantes
   | Peor Caso     | O (n*log(n) )        | O(n)                 | la profundidad de maxima siempre es log(n) 

   
# Bitácora de Inteligencia Artificial:

# Isabella Cubillo
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


## Bitácora Arleth Varela

1. Revisión de la estructura de QuickSort

  Estructura: 
  Algoritmo de ordenamiento QuickSort dentro de la carpeta `src/algorithms/`, siguiendo la estructura del proyecto y utilizando un template para que el algoritmo pueda trabajar con diferentes tipos de objetos.

  Prompt ingresado:
  "Tengo que implementar un algoritmo QuickSort en C++ para mi proyecto. Necesito que el ordenamiento pueda utilizarse tanto para objetos de tipo Tarea como para Usuario. ¿Me puedes ayudar a crear la estructura de los archivos y explicarme cómo debería organizar el código?"

  Revisión y ajuste humano aplicado: 
  Se utilizó la respuesta de la IA como guía para crear la estructura inicial. Posteriormente, se revisaron los nombres de las carpetas y las rutas de los archivos para adaptarlos a la estructura real del proyecto.


2. Implementación de QuickSort utilizando un template

  Estructura: 
  Implementación de uno de los algoritmos QuickSort mediante programación genérica, evitando crear una clase de ordenamiento independiente para `Tarea` y otra para `Usuario`.

  Prompt ingresado:
  "Quiero hacer QuickSort usando un solo template para poder ordenar tanto Tarea como Usuario. Los objetos se manejan mediante punteros. ¿Cómo puedo implementar el método de partición, el método recursivo y el método ordenar?"

  Revisión y ajuste humano aplicado: 
  Se revisó el código generado para comprender el funcionamiento de la partición y de las llamadas recursivas. Se verificó que QuickSort intercambiara los punteros de los objetos en lugar de realizar copias de los objetos completos.


3. Corrección de los include y organización de carpetas

  Estructura: 
  Organización de los archivos relacionados con QuickSort y corrección de las rutas de inclusión.

  Prompt ingresado:
  "¿Puedes revisar si los `#include` están correctos y decirme cómo deberían quedar las rutas entre QuickSort, ICondicion, CondicionTarea, CondicionUsuario, Tarea y Usuario?"

  Revisión y ajuste humano aplicado: 
  Se compararon las rutas propuestas con la estructura real del proyecto. Se corrigieron los `#include` para evitar rutas como `../src/...` cuando el archivo ya se encontraba dentro de `src/algorithms/QuickSort`. También se verificó que cada condición incluyera correctamente la interfaz `ICondicion` y la clase que necesitaba comparar.


4. Análisis de complejidad de QuickSort

  Estructura: 
  Justificación del análisis asintótico del algoritmo de ordenamiento implementado.

  Prompt ingresado:
  "Necesito incluir en el proyecto la justificación de la complejidad Big-O de QuickSort. ¿Puedes explicarme cuál es la complejidad en el caso promedio y en el peor caso y cómo justificarla brevemente en el trabajo?"

  Revisión y ajuste humano aplicado: 
  Se revisó la explicación proporcionada por la IA y se incorporó al trabajo la complejidad correspondiente al algoritmo. Se estableció que QuickSort tiene una complejidad promedio de **O(n log n)** y una complejidad de **O(n²)** en el peor caso, dependiendo de cómo se dividan los elementos alrededor del pivote.


5. Revisión e implementación de ViewTareas

  Estructura: 
  Implementación y revisión de la clase ViewTareas, encargada de mostrar las tareas y el tablero Kanban mediante consola.

  Prompt ingresado:
  "Podrías ayudarme a revisar si mi clase ViewTareas logra mostrar de la manera correcta el tablero Kanban en consola."

  Revisión y ajuste humano aplicado: 
  Se verificó la organización del tablero Kanban en los tres estados. 


5. Implementación de IComando y manejo del historial

  Estructura: 
  Estructuras necesarias para administrar las operaciones de deshacer y rehacer, tomando como referencia el UML del proyecto.

  Prompt ingresado: 
  "Podrías verificar que las pilas del gestor se estén implemnetando de la manera correcta según el UML del proyecto."

  Revisión y ajuste humano aplicado:
  Se revisó el UML para determinar que IComando debía funcionar como una interfaz con los métodos: ejecutar() y deshacer(. También se implementó una pila para almacenar objetos de tipo IComando y se estableció que GestorHistorial debía manejar dos pilas: deshacer y rehacer. Se revisó además el uso de runtime_error/stdexcept en la implementación de la pila para manejar situaciones como intentar retirar un elemento cuando la pila está vacía.


   
   
   




   
