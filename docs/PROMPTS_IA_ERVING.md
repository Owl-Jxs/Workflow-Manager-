# Registro de Uso de IA - Erving

Este documento registra el uso de herramientas de Inteligencia Artificial (Claude) durante el desarrollo de WorkflowManager, incluyendo los prompts empleados y el proceso de validación humana aplicado a cada resultado, conforme a lo solicitado en la rúbrica del proyecto (criterio "Integración de IA (QA)").

\---

## 1\. Estructura de carpetas del repositorio

**Prompt:** Consulté a la IA cómo podría organizarse la estructura de carpetas del proyecto en GitHub para un sistema con múltiples estructuras de datos, algoritmos, persistencia e interfaz.

**Resultado de la IA:** Sugirió una estructura con `data`, `docs`, `src`, `tests` en la raíz, y dentro de `src`: `algorithms`, `models`, `persistence`, `structures`, `ui`, y el `main`.

**Validación humana:** Revisé que la separación tuviera sentido para el alcance del proyecto (modelos separados de estructuras, persistencia aislada del resto) y la implementé en el repositorio. Es la estructura que el equipo está usando actualmente.

\---

## 2\. Manejo de roles en la clase Usuario

**Prompt:** Con base en la Aclaratoria Oficial #1 (que definía dos roles: Administrador y Usuario Normal, cada uno con su propio menú), pregunté cómo debía guardarse el rol dentro de la clase `Usuario`.

**Resultado de la IA:** Sugirió usar un `enum class` declarado dentro de la sección pública de la misma clase `Usuario`, con dos valores fijos: `ADMINISTRADOR` y `USUARIO\_NORMAL`. Adicionalmente, al revisar el código que ya tenía escrito, sugirió usar lista de inicialización en el constructor en vez de asignación con `this->`.

**Validación humana:** Entendí el motivo del `enum class` (valores fijos y con alcance controlado, evita errores de comparación con strings sueltos) y lo apliqué en `Usuario.h`. Sobre la lista de inicialización, confirmé que es más eficiente para este caso porque evita construir y luego reasignar los atributos, y ajusté el constructor de esa forma.

\---

## 3\. Diseño del UML de la Semana 1 (estructuras y decisiones de diseño)

**Prompt:** Con `Usuario` y `Tarea` casi terminados, pedí ayuda para armar un UML de la semana 1 que organizara lo ya construido y lo que faltaba (nodos, campo de responsable en `Tarea`, clase de Historial), y para encontrar una herramienta adecuada para diagramarlo.

**Resultado de la IA:** Se recomendó PlantUML como herramienta (por su integración con texto/código versionable en GitHub) y se generó el código del diagrama, incluyendo la decisión de que `Tarea` referenciara al usuario responsable por **ID** en vez de por puntero directo, para evitar un *dangling pointer* si el usuario se elimina del sistema.

**Validación humana:** Antes de aceptar la sugerencia del ID, razoné el escenario de qué pasaría si un usuario responsable se elimina en ambos casos (puntero vs ID), y confirmé que el ID era más seguro, consistente con el patrón que ya usábamos en `idTareaPadre`.

Adicionalmente, Daniel (compañero de equipo) observó que el `nodoTarea` del diagrama no necesitaba puntero al nodo anterior, ya que ese nodo sería exclusivo de una Cola FIFO. Le pregunté a la IA por qué una Cola FIFO no necesita ese puntero, y la respuesta fue que una estructura FIFO solo se mueve en una dirección (de quien entra a quien sale), sin necesidad de recorrer hacia atrás. Validé el razonamiento con el ejemplo de una fila física y confirmé el cambio, quitando el puntero `anterior` de `nodoTarea` en el diagrama.

\---

*Última actualización: 6 de agosto, 2026.*

