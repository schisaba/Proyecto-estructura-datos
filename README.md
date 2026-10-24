#  Sistema Policial de Gestión de Casos (Motor C++ & Interfaz Web)


Sistema interactivo de administración de una central de inteligencia policial. El núcleo lógico y todas las estructuras de datos (**Cola con Prioridad, Pila, Cola Circular y Registros**) se ejecutan de forma nativa en **C++** a través de un servidor local que se comunica en tiempo real con una **Interfaz Gráfica Web**.

---

##  Tabla de Contenidos
1. [Estructura de Archivos del Proyecto](#-estructura-de-archivos-del-proyecto)
2. [Requisitos e Instrucciones de Ejecución](#-requisitos-e-instrucciones-de-ejecución)
3. [Descripción de la Interfaz Gráfica](#-descripción-de-la-interfaz-gráfica)
4. [Manual de Usuario (Paso a Paso)](#-manual-de-usuario-paso-a-paso)
5. [Alertas Policiales en Tiempo Real](#-alertas-policiales-en-tiempo-real)
6. [Arquitectura de Estructuras de Datos](#-arquitectura-de-estructuras-de-datos)
7. [Autores y Declaración de Uso de IA](#-autores)

---

##  Estructura de Archivos del Proyecto

El proyecto está modularizado en 5 archivos para separar la lógica de estructuras de datos en C++ de la visualización gráfica:

| Archivo | Lenguaje | Rol en el Sistema |
| :--- | :---: | :--- |
| **`Estructuras.h`** | C++ | Contiene la definición e implementación de `struct Sospechoso`, `struct Detective`, `ColaCircularCoartadas`, `PilaEvidencias` (`NodoPila`) y `ColaPrioridadCasos` (`NodoCaso`). |
| **`servidor.cpp`** | C++ | Micro-servidor HTTP local en el puerto `8080` que mantiene las estructuras en memoria RAM, procesa las órdenes del usuario y exporta el estado en formato JSON. |
| **`index.html`** | HTML5 | Maquetación del panel de control policial dividido en tres columnas operativas. |
| **`styles.css`** | CSS3 | Diseño visual oscuro (*dark theme*), tarjetas de expedientes, etiquetas de prioridad y animaciones. |
| **`app.js`** | JavaScript | Conector asíncrono (`fetch`) que comunica los clics del navegador con el motor C++ y controla el cronómetro de alertas de 60 segundos. |

---

##  Requisitos e Instrucciones de Ejecución

Para ejecutar el proyecto necesitas un compilador de C++ (`g++`) en un entorno Linux/macOS o WSL y cualquier navegador web moderno.

```bash
# 1. Clonar el repositorio y entrar a la carpeta
git clone [https://github.com/schisaba/Proyecto-estructura-datos](https://github.com/schisaba/Proyecto-estructura-datos)
cd Proyecto-estructura-datos

# 2. Compilar el servidor C++ (incluye automáticamente Estructuras.h)
g++ servidor.cpp -o servidor

# 3. Iniciar el motor C++ en el puerto 8080
./servidor
```

Una vez que la terminal muestre el mensaje `MOTOR C++ ACTIVO EN: http://localhost:8080`, **abre el archivo `index.html` con doble clic en tu navegador** (sin cerrar la terminal).

---

## Descripción de la Interfaz Gráfica

La pantalla principal se divide en **4 zonas intuitivas**:

1. **Barra Superior y Cronómetro de Alertas:** Muestra el contador regresivo de **60 segundos** para la entrada automática de nuevos casos y un botón rojo (` Simular Alerta Ahora`) para inyectar expedientes de prueba al instante.
2. **Columna Izquierda (`Cola con Prioridad` y `Plantilla de Detectives`):**
   - En la parte superior lista todos los casos pendientes ordenados estrictamente por urgencia (`Alta (1)`, `Media (2)`, `Baja (3)`), resaltando en rojo el caso que está en la cabeza de la cola (`EN ATENCIÓN`).
   - En la parte inferior muestra las tarjetas de los 4 detectives con su estado (`Disponible` u `Ocupado`) y su puntaje de casos resueltos.
3. **Columna Central (`Sala de Evidencias - Pila`):** Cuenta con el botón para recolectar pistas de la escena del crimen y muestra visualmente la pila de evidencias, destacando siempre en la parte superior el **`[TOPE DE LA PILA]`** (última pista descubierta).
4. **Columna Derecha (`Perfiles e Interrogatorio`):** Despliega las tarjetas de los **5 sospechosos** vinculados al caso activo, mostrando su **coartada declarada** (asignada desde la Cola Circular), sus **4 rasgos personales** y el botón de orden de arresto.

---

##  Manual de Usuario (Paso a Paso)

> [!IMPORTANTE]
> Todas las acciones de investigación, interrogatorio y arresto se ejecutan **únicamente sobre el caso más urgente**, el cual se ubica siempre en la primera tarjeta de la **Cola con Prioridad** (`EN ATENCIÓN`).

### Paso 1: Asignar un Detective al Caso Prioritario
- Dirígete a la sección ** Plantilla de Detectives** (abajo a la izquierda).
- Haz clic sobre la tarjeta de cualquier detective que tenga el indicador verde **`● Disponible`**.
- Verás que el detective pasa a estado `Ocupado` y su nombre aparece vinculado en la tarjeta del caso activo.

### Paso 2: Recolectar Evidencias en la Pila
- En la columna central (** Sala de Evidencias**), haz clic en el botón azul **` Procesar Nueva Evidencia de la Escena`**.
- Cada clic descubre un rasgo real del culpable en C++ y lo apila en la cima de la estructura (` [TOPE DE LA PILA]`).
- Puedes recolectar desde **1 hasta un máximo de 4 evidencias** por caso.

> [!TIP]
> El sistema exige haber asignado un detective y haber procesado **al menos 1 evidencia** antes de permitirte arrestar a un sospechoso. Si recolectas las **4 evidencias**, solo uno de los 5 sospechosos coincidirá con las 4 pistas simultáneamente.

### Paso 3: Contrastar Perfiles y Emitir la Orden de Arresto
- Revisa la columna derecha (**Perfiles e Interrogatorio**) y compara las pistas apiladas en la columna central con las etiquetas de rasgos de los **5 sospechosos**.
- Cuando identifiques al culpable, haz clic en el botón rojo **` Emitir Orden de Arresto`** dentro de su tarjeta:
  - **Si aciertas:** El banner superior confirmará la captura del culpable y el contador de casos resueltos del detective sumará `+1`.
  - **Si fallas:** El banner notificará que arrestaste a un inocente y revelará el nombre del verdadero culpable.
- En ambos casos, el detective queda automáticamente `Disponible` y el expediente se desencola en C++, liberando toda su memoria dinámica y dando paso al siguiente caso en la cola.

---

##  Alertas Policiales en Tiempo Real

> [!CUIDADO]
> Cada vez que el reloj superior llega a **0 segundos** (o al presionar ` Simular Alerta Ahora`), el servidor C++ genera un nuevo crimen aleatorio y lo inserta en la **Cola con Prioridad**.
> 
> Si estabas investigando un caso de **Prioridad 2 (Media)** y entra una emergencia de **Prioridad 1 (Alta)**, el nuevo caso se colocará automáticamente en la primera posición de la cola para ser atendido de inmediato, conservando intacto todo el progreso (detective y pistas apiladas) del caso anterior.

---

## Arquitectura de Estructuras de Datos

- **Cola con Prioridad (`ColaPrioridadCasos` y `NodoCaso`):** Lista simplemente enlazada ordenada que ubica los expedientes activos según su nivel de urgencia (`1: Alta`, `2: Media`, `3: Baja`), respetando el orden cronológico de llegada (FIFO) en caso de empate.
- **Pila (`PilaEvidencias` y `NodoPila`):** Estructura enlazada bajo filosofia LIFO (*Last In, First Out*) anidada dentro de cada caso para almacenar las pistas descubiertas, mostrando siempre el hallazgo más reciente en la cima.
- **Cola Circular (`ColaCircularCoartadas`):** Contenedor de tamaño fijo que rota perpetuamente un catálogo de 10 coartadas en tiempo constante $O(1)$ mediante aritmética modular (`% 10`).
- **Registros (`struct Sospechoso` y `struct Detective`) y Arreglos Estáticos:**
  - `Sospechoso`: Modela el catálogo global (`CatalogoSospechosos[15]`), el subconjunto aleatorio de 5 sospechosos únicos por caso (`sospechosos[5]`) y sus 4 rasgos (`rasgos[4]`).
  - `Detective`: Agrupa el nombre, disponibilidad booleana y casos resueltos de la plantilla fija de 4 agentes de la central (`detectives[4]`).

---

## Autores

- **Sebastián Chisaba Cruz**
- **Ariadna Valentina Ramírez Santeliz**
- **Jerónimo Pascuas Barroso**

> **Nota de transparencia académica:** Se empleó un **45% de asistencia de Inteligencia Artificial** como apoyo en la estructuración de la documentación en línea (estándar Doxygen), maquetación de la interfaz gráfica web y refactorización/pulido de clases en C++.
