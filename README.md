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

1. Revisión de la estructura de QuickSort:
Estructura: 
   - Algoritmo de ordenamiento QuickSort dentro de la carpeta `src/algorithms/`, siguiendo la estructura del proyecto y utilizando un template para que el algoritmo pueda trabajar con diferentes tipos de objetos.

Prompt ingresado:
   - "Tengo que implementar un algoritmo QuickSort en C++ para mi proyecto. Necesito que el ordenamiento pueda utilizarse tanto para objetos de tipo Tarea como para Usuario. ¿Me puedes ayudar a crear la estructura de los archivos y explicarme cómo debería organizar el código?"

Revisión y ajuste humano aplicado: 
   - Se utilizó la respuesta de la IA como guía para crear la estructura inicial. Posteriormente, se revisaron los nombres de las carpetas y las rutas de los archivos para adaptarlos a la estructura real del proyecto.

2. Implementación de QuickSort utilizando un template:
Estructura: 
   - Implementación de uno de los algoritmos QuickSort mediante programación genérica, evitando crear una clase de ordenamiento independiente para `Tarea` y otra para `Usuario`.

Prompt ingresado:
   - "Quiero hacer QuickSort usando un solo template para poder ordenar tanto Tarea como Usuario. Los objetos se manejan mediante punteros. ¿Cómo puedo implementar el método de partición, el método recursivo y el método ordenar?"

Revisión y ajuste humano aplicado: 
   - Se revisó el código generado para comprender el funcionamiento de la partición y de las llamadas recursivas. Se verificó que QuickSort intercambiara los punteros de los objetos en lugar de realizar copias de los objetos completos.

3. Corrección de los include y organización de carpetas:
Estructura: 
   - Organización de los archivos relacionados con QuickSort y corrección de las rutas de inclusión.

Prompt ingresado:
    - "¿Puedes revisar si los "#include" están correctos y decirme cómo deberían quedar las rutas entre QuickSort, ICondicion, CondicionTarea, CondicionUsuario, Tarea y Usuario?"

Revisión y ajuste humano aplicado: 
   - Se compararon las rutas propuestas con la estructura real del proyecto. Se corrigieron los `#include` para evitar rutas como `../src/...` cuando el archivo ya se encontraba dentro de `src/algorithms/QuickSort`. También se verificó que cada condición incluyera correctamente la interfaz `ICondicion` y la clase que necesitaba comparar.


4. Análisis de complejidad de QuickSort:
Estructura: 
    - Justificación del análisis asintótico del algoritmo de ordenamiento implementado.

Prompt ingresado:
   - "Necesito incluir en el proyecto la justificación de la complejidad Big-O de QuickSort. ¿Puedes explicarme cuál es la complejidad en el caso promedio y en el peor caso y cómo justificarla brevemente en el trabajo?"

Revisión y ajuste humano aplicado: 
   - Se revisó la explicación proporcionada por la IA y se incorporó al trabajo la complejidad correspondiente al algoritmo. Se estableció que QuickSort tiene una complejidad promedio de **O(n log n)** y una complejidad de **O(n²)** en el peor caso, dependiendo de cómo se dividan los elementos alrededor del pivote.

5. Revisión e implementación de ViewTareas:
Estructura: 
   - Implementación y revisión de la clase ViewTareas, encargada de mostrar las tareas y el tablero Kanban mediante consola.

Prompt ingresado:
   - "Podrías ayudarme a revisar si mi clase ViewTareas logra mostrar de la manera correcta el tablero Kanban en consola."

Revisión y ajuste humano aplicado: 
     - Se verificó la organización del tablero Kanban en los tres estados. 

6. Implementación de IComando y manejo del historial:
Estructura: 
   - Estructuras necesarias para administrar las operaciones de deshacer y rehacer, tomando como referencia el UML del proyecto.

Prompt ingresado: 
   - "Podrías verificar que las pilas del gestor se estén implemnetando de la manera correcta según el UML del proyecto."

Revisión y ajuste humano aplicado:
   - Se revisó el UML para determinar que IComando debía funcionar como una interfaz con los métodos: ejecutar() y deshacer(. También se implementó una pila para almacenar objetos de tipo IComando y se estableció que GestorHistorial debía manejar dos pilas: deshacer y rehacer. Se revisó además el uso de runtime_error/stdexcept en la implementación de la pila para manejar situaciones como intentar retirar un elemento cuando la pila está vacía.
  
# Daniel Alvarado
1. Rediseño de TareaController refactorizado para un flujo mas completo en el "Ciclo" de vida de las tareas:
   
Estructura:
   - Distribucion de las tareas segun su estado y prioridad en tareaController
   
Prompts ingresados:
   - "en mi proyecto, en una parte tengo colas de tareas urgentes y regulares, pero cuando se completan no se que hacer, si debo de guardarlas en la base de datos, crear una cola aparte para las tareas completadas (o usar un vector, ya que al cerrar el sistema no deberian volverse a cargarse)"
   
   - "estaba viendo y en mi proyecto separe las colas de tarea entre urgentes y regulares, y cada tarea tiene 3 estados ( "En proceso", "Revision", "completada"). Para las completadas las deje en otra cola aparte pero las demas las deje en dos colas, una de regulares y otra de urgentes. El problema es que de esta manera para completar una tarea o mandarla a revisar no se extrae la del frente (aunque para mandarla a revisar es depende del usuario, ya que el solo puede mandar a revisar sus propias tareas). Deberia refactorizar mi codigo en 3 colas diferentes y hacer que las urgentes vayan primero? 

Revisión y ajuste humano aplicado:
   - Con las respuestas y el siguiendo a partir de estos dos prompts llegue a una conclusión. Ajuste el diseño para que en el controlador existieran 4 estructuras. 3 Colas FIFO (para regulares y urgentes que estén en "por hacer" además de una para las tareas completadas) y un vector para las tareas que estén en proceso. 

2.  Rediseño de TareaDataBase refactorizado consecuente del cambio de tareaController:
   
Estructura:
   - Métodos De Cargar/guardar las tareas ademas de creacion de otro archivo .csv para las tareas completadas

Prompts ingresados:
   - "siguiendo con el diseno, lo hare como te comente antes. Pero eso como afectaria en la database? Primeramente pense en hacer un archivo por estado de cada tarea pero luego pense en agrupar todas en 2, uno para las completadas y otro para las activas, ya que todas son tareas por igual. Como afectaria la carga de las tareas al arrancar el programa? Ya que son varias listas"
   - "debido a que en la lista de guardados también estan las subtareas, implemente este método private para cargar las listas, pero ahora que se cargan todas a la vez como procederían. Se harían dos métodos privados, uno para cargar las listas principales y otro para cargar sus subtareas? o que seria lo mas eficiente?"
  
Revisión y ajuste humano aplicado:
   - Cree varias tareas de diferentes tipos y procedí a guardarlas y cargarlas. Como ajustes, plantee el orden en el que se  guardaría las tareas además en el controlador de tarea ante una excepción al cargar los archivos hice que las listas no quedaran como nullptr

3. Creación del código para el algoritmo de mergesort
   
Estructura:
   - Para ahorrar tiempo el algoritmo del merge sort se lo pedi a la AI, con los parametros y objetivos adecuados

Prompts ingresados:
   - "Hazme el algoritmo merge Sort en una class template que reciba como parametros un vector por referencia donde viviran los datos a ordenar ademas de un puntero a una clase llamada Icondicion, que tendra un metodo  bool llamado comparar que recibira dos punteros de tipo Template y comparara si el objeto 1 va primero o no que el objeto 2, decidiendo asi quien va primero en el vector

Revisión y ajuste humano aplicado:
   - Me asegure que los parametros y su utilizacion fueran las correctas.

4. Testeo del proyecto para busqueda de errores:
Estructura:
   - Alrededor de todo el proyecto, fue revisando errores de compilacion, bugs o errores de interraccion entre clases

Prompts ingresados:
   - "Tengo este proyecto (archivo .zip) de estructuras de datos, revisa todos los archivos que hay hasta el momento para encontrar posibles bugs; errores de diseño, conexiones o compilación; o cualquier otro posible error en el sistema. Para esto encuentra solo la ubicación del error y su motivo, yo me encargare de hacer los ajustes necesarios"

Revision y ajuste humano aplicado:
   - Por cada error/bug encontrado fui buscando su origen y ajustando el programa para corregirlo personalmente según lo mas conveniente para el proyecto. 

## Erving Sequeira

## Entrada 1. Organización general del proyecto

* **Función o estructura:** Organización general de carpetas del proyecto y estructura base del repositorio.
* **Herramienta utilizada:** Claude Sonnet 5.
* **Prompt utilizado:**

  > "Estoy armando un proyecto en C++ de Estructuras de Datos que necesita manejar usuarios, tareas, colas, pilas y árboles, con persistencia en CSV y una interfaz de consola. ¿Cómo debería organizar las carpetas del proyecto?"
* **Respuesta de la IA:** Se sugirió separar los archivos de datos y código fuente mediante las carpetas `data/` y `src/`. Dentro de `src/` se propusieron subcarpetas como `algorithms/`, `models/`, `persistence/`, `structures/`, `ui/` y el archivo `main.cpp`. También se explicó el uso de archivos `.gitkeep` para mantener carpetas vacías dentro del repositorio de GitHub.
* **Revisión humana:** El equipo adaptó la propuesta a las necesidades reales del proyecto. Se agregó la carpeta `controllers/` para seguir un patrón MVC, `utilities/` para centralizar validaciones y captura de entradas del usuario, y `tests/` como carpeta independiente para las pruebas unitarias. En las pruebas se concentró el uso de IA en dos archivos relacionados con usuarios y tareas.

## Entrada 2. Modelo `Usuario`

* **Función o estructura:** Clase `Usuario`, incluyendo sus atributos y métodos principales.
* **Herramienta utilizada:** Gemini 3.6 flash.
* **Prompt utilizado:**

  > "Ya tengo una base básica del modelo Usuario, ¿qué opinas de la estructura? ¿Está bien planteada?"
* **Respuesta de la IA:** Se recomendó utilizar un `enum class Rol { ADMINISTRADOR, USUARIO_NORMAL }` en lugar de almacenar el rol como `string`, con el objetivo de reducir errores de escritura y representar los valores permitidos de forma más controlada. También se recomendó almacenar un hash de la contraseña en lugar de la contraseña directamente y agregar validaciones en los métodos setter del modelo.
* **Revisión humana:** Durante la implementación se detectó un problema con el método `setHashContrasena`. Este método funcionaba correctamente al crear un usuario nuevo, pero al cargar usuarios desde un archivo CSV el hash ya estaba calculado. Aplicar nuevamente el proceso de hash provocaba que las credenciales dejaran de coincidir durante el inicio de sesión. Para solucionar esto se creó `setHashDirecto`, utilizado exclusivamente cuando el valor cargado desde el archivo ya corresponde a un hash.

## Entrada 3. `NodoUsuario` y estructuras enlazadas

* **Función o estructura:** `NodoUsuario` y criterio utilizado para los nodos de listas doblemente enlazadas, colas y pilas.
* **Herramienta utilizada:** Claude Sonnet 5.
* **Prompt utilizado:**

  > "Los nodos normalmente usan punteros, ¿debería usar puntero incluso para los datos que son el usuario?"
* **Respuesta de la IA:** Se recomendó utilizar punteros tanto para el dato almacenado por el nodo como para sus enlaces. Para el dato, la razón principal fue evitar copiar objetos completos innecesariamente. Para los enlaces, se explicó que los punteros son necesarios para construir correctamente las estructuras dinámicas enlazadas y evitar una composición recursiva directa de nodos.
* **Revisión humana:** El equipo adoptó este criterio de forma consistente en las diferentes estructuras dinámicas del proyecto. El mismo enfoque se utilizó en las listas doblemente enlazadas, colas estándar, colas de prioridad y pilas.

## Entrada 4. `UsuarioDataBase`

* **Función o estructura:** Persistencia de usuarios mediante archivos CSV.
* **Herramienta utilizada:** Claude Sonnet 5.
* **Prompt utilizado:**

  > "Nunca he trabajado con archivos CSV, solo conozco un poco de lectura y escritura de archivos para persistencia. ¿Cómo debería estructurar la clase para guardar y cargar usuarios desde CSV?"
* **Respuesta de la IA:** Se propuso dividir las responsabilidades de la clase mediante métodos privados de apoyo, como `formularLinea`, `rolATexto` y `existeUsuarioConID`. Los métodos públicos de guardado y carga utilizarían estas funciones auxiliares para organizar el procesamiento del archivo CSV.
* **Revisión humana:** Se adaptaron las rutas para que los archivos de persistencia fueran almacenados dentro de la carpeta `data/` del proyecto. También se identificó y corrigió el problema relacionado con el hash de las contraseñas al cargar usuarios. Para evitar que el hash almacenado en el CSV fuera procesado nuevamente, se utilizó `setHashDirecto`.

## Entrada 5. Comandos y auditoría

* **Función o estructura:** Clases `ComandoAsignacion`, `ComandoTarea`, `ComandoUsuario` e integración con `Gestor` para implementar auditoría.
* **Herramienta utilizada:** Claude Sonnet 5.
* **Prompt utilizado:**

  > "Debo crear los comandos tomando en cuenta que esta semana se agregó la petición de que debe haber una auditoría según 'Aclaratoria 3' que guarde logs sobre comandos, justo va bien porque ya teníamos planeados los comandos, pero creo debo modificar GestorHistorial para que reciba la auditoría y tras cada ejecutar, deshacer, rehacer y todos los comandos de usuario o tarea queden guardados en logs."
* **Respuesta de la IA:** Se propuso implementar los comandos mediante una interfaz `IComando` y modificar `GestorHistorial` para registrar automáticamente las operaciones realizadas. De esta forma, las acciones de ejecutar, deshacer y rehacer podrían quedar registradas en el sistema de auditoría.
* **Revisión humana:** En lugar de crear un archivo independiente para cada comando, se decidió agrupar los comandos en tres clases generales: `ComandoAsignacion`, `ComandoTarea` y `ComandoUsuario`. Además, se agregaron los métodos `getAccionAuditoria` y `getIdTareaAuditoria` para proporcionar a `GestorHistorial` la información necesaria de manera uniforme.

## Entrada 6. Menú de tareas para usuario normal

* **Función o estructura:** Procesos correspondientes al menú de tareas del usuario normal mediante `procesosTareaNormal`.
* **Herramienta utilizada:** Claude Sonnet 5.
* **Prompt utilizado:**

  > "Supongo lo mejor será crear, como se hizo para el menú de admin, los procesosTareaNormal."
* **Respuesta de la IA:** Se recomendó mantener una estructura similar a la utilizada en el menú de administrador, pero adaptando las opciones disponibles a los permisos correspondientes al usuario normal.
* **Revisión humana:** Durante la implementación se corrigió la lógica relacionada con la visualización de subtareas. Cuando la tarea seleccionada corresponde a una subtarea, se debe mostrar su propio subárbol. Cuando corresponde a una tarea raíz, se deben mostrar las tareas hijas correspondientes.

## Entrada 7. Depuración general del proyecto

* **Función o estructura:** Revisión general del proyecto, incluyendo memoria dinámica, estructuras de datos, persistencia e interfaz de usuario.

* **Herramienta utilizada:** Open Code MiMo 2.5.

* **Prompt utilizado:**

  > "¿Puedes revisar el proyecto en busca de errores de sintaxis y lógica?"

* **Respuesta de la IA:** La revisión se realizó en varias rondas. Entre las sugerencias y correcciones realizadas se encontraron:

  1. Corrección de problemas relacionados con `use after free`, `return` después de `delete`, un índice inalcanzable, un error en `eliminarAsignacionPorIdUsuario` y diferentes posibles desreferencias de punteros nulos.
  2. Liberación de las colas originales en antes de crear nuevas estructuras y liberación de las tareas almacenadas en `listaTareasEnProceso` dentro del destructor.
  3. Corrección de impresiones en consola que no mostraban la información esperada.
  4. Corrección de problemas relacionados con la búsqueda y eliminación de subárboles de tareas en `TareaController` y en la interfaz.
  5. Corrección de mensajes que no eran visibles debido a la ausencia de una pausa después de utilizar `system("cls")`, además de otros ajustes menores.

* **Revisión humana:** Las sugerencias fueron revisadas manualmente antes de incorporarse al proyecto. Algunas correcciones no eran aplicables al código existente o se basaban en interpretaciones incorrectas del proyecto. En esos casos, los cambios fueron descartados o modificados por el equipo. Posteriormente se realizaron nuevos commits con las correcciones consideradas apropiadas.
