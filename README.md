# Workflow-Manager



\# WorkflowManager — Proyecto 1 de Estructuras de Datos



Sistema de gestión de tareas con listas, pilas, colas y árboles en C++.



\## Cómo abrir el proyecto



1\. Cloná el repo.

2\. Abrí la carpeta (no un archivo individual) en tu IDE:

&#x20;  - \*\*Visual Studio\*\*: Archivo → Abrir → Carpeta...

&#x20;  - \*\*CLion\*\*: File → Open → seleccioná la carpeta raíz

&#x20;  - \*\*VSCode\*\*: necesitás la extensión "CMake Tools"

3\. El IDE va a detectar el `CMakeLists.txt` solo y configurar el proyecto.



\## Estructura de includes



Todos los `#include` entre archivos del proyecto van con la ruta

completa desde `src/`. Ejemplo, si estás en `ui/Menu.cpp` y necesitás

`Usuario.h` (que vive en `models/`):



&#x20;   #include "models/Usuario.h"



\## Estado del proyecto



🚧 En desarrollo — Semana 1 de 5.

