#  Sistema Policial de Gestión de Casos

Simulador interactivo de consola en **C++** para la administración de una central de detectives, procesamiento de escenas del crimen, interrogatorio de sospechosos y resolución de expedientes criminales en tiempo real mediante estructuras de datos dinámicas y estáticas.

---

## Tabla de Contenidos
1. [Requisitos y Compilación](#-requisitos-y-compilación)
2. [Estructura del Menú Principal](#-estructura-del-menú-principal)
3. [Manual de Operación (Paso a Paso)](#-manual-de-operación-paso-a-paso)
4. [Eventos en Tiempo Real](#-eventos-en-tiempo-real)
5. [Arquitectura de Estructuras de Datos](#-arquitectura-de-estructuras-de-datos)

---

## Estructura del Menú Principal

Al iniciar la aplicación, navega ingresando el número del módulo correspondiente (`1` al `5`) y presionando `Enter`:

| Opción | Módulo | Descripción Operativa |
| :---: | :--- | :--- |
| **1** |  Consultar detectives | Muestra la tabla del personal con su ID, nombre, estado actual (`Disponible` / `Ocupado`) e historial de casos resueltos con éxito. |
| **2** |  Sala de Evidencias | Permite recolectar nuevas pistas de la escena del crimen (máximo 4 por expediente) y consultar el historial apilado. |
| **3** |  Expedientes / Resolver Caso | Visualiza la cola de casos ordenada por urgencia, asigna un detective al caso prioritario y ejecuta la orden de arresto. |
| **4** |  Interrogar y Ver Perfiles | Despliega la ficha de los 5 sospechosos vinculados al caso actual, incluyendo sus 4 rasgos personales y su coartada declarada. |
| **5** |  Salir | Finaliza la ejecución y libera toda la memoria dinámica reservada. |

---

##  Manual de Operación (Paso a Paso)

> [!IMPORTANT]
> El sistema opera bajo una **Cola de Prioridad Estricta**: todas las acciones de investigación, interrogatorio y arresto se aplican **únicamente al caso ubicado en el frente de la cola** (el de mayor urgencia en ese instante).

### Paso 1: Consultar el Caso Prioritario y Asignar un Detective
1. En el menú principal, ingresa a **`3. Expedientes / Asignar y Resolver Caso`**.
2. Selecciona **`1. Ver cola de casos prioritarios`** para identificar el expediente ubicado en la primera fila de la tabla.
3. Selecciona **`2. Asignar Detective al Caso Prioritario`** e ingresa el ID (`1` a `4`) de cualquier agente con estado `Disponible`.

### Paso 2: Recolectar Evidencias en la Escena del Crimen
1. Regresa al menú principal e ingresa a **`2. Sala de Evidencias`**.
2. Selecciona **`2. Procesar nueva evidencia de la escena`**. Cada ejecución extrae aleatoriamente un rasgo real del culpable y lo guarda en la pila del caso.
3. Selecciona **`1. Ver historial de evidencias`** para leer las pistas recolectadas (ordenadas desde la más reciente hasta la más antigua).

> [!TIP]
> Debes recolectar **al menos 1 evidencia** para que el sistema te permita emitir un veredicto. Si recolectas las **4 evidencias posibles**, tendrás el perfil exacto del culpable sin margen de error.

### Paso 3: Interrogar Sospechosos y Cruzar Perfiles
1. Vuelve al menú principal y selecciona **`4. Interrogar y Ver Perfiles de Sospechosos`**.
2. El sistema mostrará a los **5 sospechosos** del caso activo junto con sus **4 rasgos conocidos** y su **coartada declarada**.
3. Compara las pistas de la Sala de Evidencias con los rasgos de cada sospechoso hasta identificar quién cumple con todas las pistas.

### Paso 4: Emitir Orden de Arresto y Cerrar el Caso
1. Ingresa nuevamente a **`3. Expedientes / Asignar y Resolver Caso`** y elige **`3. Emitir Veredicto y Cerrar Caso Urgente`**.
2. Ingresa el número (`1` a `5`) correspondiente al sospechoso que deseas arrestar:
   * ** Arresto Correcto:** El contador de casos resueltos del detective asignado aumenta en `+1`.
   * ** Arresto Erróneo:** El sistema notifica que arrestaste a un inocente y revela la identidad del verdadero culpable.
3. Al finalizar el veredicto, el expediente se elimina de la cola de pendientes y el detective vuelve automáticamente al estado `Disponible`.

---

##  Eventos en Tiempo Real

> [!WARNING]
> **Generador Automático de Alertas Policiales:**
> Si permaneces **60 segundos o más** navegando en los menús, ingresará automáticamente un nuevo expediente criminal (`[!] ALERTA POLICIAL`) con prioridad Alta (`1`) o Media (`2`).
> 
> Si estás investigando un caso de **Prioridad 2** y entra una alerta de **Prioridad 1**, el nuevo caso tomará el primer lugar de la cola inmediatamente para ser atendido con urgencia.

---

##  Arquitectura de Estructuras de Datos

* **Cola con Prioridad Dinámica (`ColaPrioridadCasos`):** Ordena los expedientes activos según su urgencia (`1: Alta`, `2: Media`, `3: Baja`) respetando el orden de llegada (FIFO) en caso de empate.
* **Pila Dinámica (`PilaEvidencias`):** Almacena bajo política LIFO (*Last In, First Out*) las pistas descubiertas dentro de cada caso.
* **Cola Circular Estática (`ColaCircularCoartadas`):** Rota perpetuamente un catálogo fijo de 10 coartadas en tiempo constante $O(1)$ mediante aritmética modular.
* **Arreglos Dinámicos Paralelos:** Gestionan en el *Heap* los nombres, disponibilidad y estadísticas de los detectives.
* **Registros (`struct`) y Arreglos Estáticos:** Modelan el catálogo global de 15 sospechosos y el subconjunto aleatorio de 5 sospechosos únicos por caso.
