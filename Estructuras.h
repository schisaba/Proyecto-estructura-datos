/**
 * @file Estructuras.h
 * @authors Sebastian Chisaba Cruz
 *          Ariadna Valentina Ramirez Santeliz
 *          Jeronimo Pascuas Barroso
 * @brief Cabecera con la implementación de las estructuras de datos del sistema
 * policial.
 * @details Contiene los Registros (struct Sospechoso y Detective), la Cola
 * Circular de coartadas, la Pila de evidencias y la Cola
 * con Prioridad de casos.
 * @note Declaración de Uso de IA: Se empleó un 45% de asistencia de
 * Inteligencia Artificial para la estructuración de la documentación, interfaz
 * web y refactorización de clases.
 */

#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// ============================================================================
// 1. REGISTROS (STRUCT) PARA SOSPECHOSOS Y DETECTIVES
// ============================================================================

/**
 * @struct Sospechoso
 * @brief Registro que modela la ficha de un sospechoso en el catálogo y dentro
 * de cada caso.
 */
struct Sospechoso {
  string nombre;    // Nombre completo del ciudadano
  string rasgos[4]; // Arreglo estático con sus 4 características particulares
  string coartada;  // Declaración asignada desde la Cola Circular
  bool esCulpable;  // Bandera que indica si es el autor real del crimen en el
                    // caso activo
};

/**
 * @struct Detective
 * @brief Registro que agrupa el estado operativo y estadísticas de cada agente
 * policial.
 */
struct Detective {
  string nombre;   // Nombre del detective
  bool disponible; // Estado actual (true: libre para asignar, false: ocupado en
                   // un caso)
  int casosResueltos; // Contador histórico de arrestos exitosos
};

/**
 * @brief Catálogo global estático con los 15 perfiles base de sospechosos.
 */
Sospechoso CatalogoSospechosos[15] = {
    {"Carlos Mendoza",
     {"tiene cabello castaño", "usa un llavero de Petro", "usa lentes",
      "no utiliza redes sociales"},
     "",
     false},
    {"Elena Suarez",
     {"es otaku", "bebe alcohol", "usa un llavero de Petro",
      "no utiliza redes sociales"},
     "",
     false},
    {"Roberto Silva",
     {"usa un llavero de Petro", "tiene ojos azules", "tiene un perro",
      "va al gimnasio"},
     "",
     false},
    {"Sofia Gomez",
     {"usa bicicleta", "bebe alcohol", "tiene ojos azules",
      "tiene cabello castaño"},
     "",
     false},
    {"Diego Alvarez",
     {"bebe alcohol", "usa lentes", "usa un llavero de Petro",
      "es de baja estatura"},
     "",
     false},
    {"Valeria Rios",
     {"usa un llavero de Petro", "tiene cabello castaño", "tiene ojos azules",
      "frecuenta la biblioteca"},
     "",
     false},
    {"Gabriel Torres",
     {"es de baja estatura", "bebe alcohol", "no usa redes sociales",
      "tiene ojos azules"},
     "",
     false},
    {"Isabella Vega",
     {"no utiliza redes sociales", "va al gimnasio", "es de baja estatura",
      "es otaku"},
     "",
     false},
    {"Lucas Morales",
     {"va al gimnasio", "bebe alcohol", "tiene un perro", "es otaku"},
     "",
     false},
    {"Camila Ramirez",
     {"tiene cabello castaño", "tiene un perro", "tiene ojos azules",
      "es otaku"},
     "",
     false},
    {"Mateo Benitez",
     {"va al gimnasio", "usa lentes", "usa bicicleta", "tiene un perro"},
     "",
     false},
    {"Maria Santeliz",
     {"es de baja estatura", "va al gimnasio", "frecuenta la biblioteca",
      "no utiliza redes sociales"},
     "",
     false},
    {"Tomas Castro",
     {"tiene un perro", "es de baja estatura", "usa un llavero de Petro",
      "frecuenta la biblioteca"},
     "",
     false},
    {"Mariana Paredes",
     {"usa bicicleta", "usa lentes", "frecuenta la biblioteca",
      "va al gimnasio"},
     "",
     false},
    {"Javier Blanco",
     {"frecuenta la biblioteca", "usa lentes", "es otaku",
      "tiene cabello castaño"},
     "",
     false},
};

// ============================================================================
// 2. COLA CIRCULAR PARA COARTADAS
// ============================================================================

/**
 * @class ColaCircularCoartadas
 * @brief Estructura circular de 10 posiciones que rota coartadas en tiempo
 * O(1).
 */
class ColaCircularCoartadas {
public:
  string elementos[10]; // Arreglo fijo de 10 declaraciones
  int cabeza;           // Índice del frente de la cola
  int ultimo;           // Índice del final de la cola
  int tamano;           // Cantidad de elementos cargados

  /**
   * @brief Constructor que inicializa los índices y precarga las 10 coartadas
   * del sistema.
   */
  ColaCircularCoartadas() {
    cabeza = 0;
    ultimo = -1;
    tamano = 0;

    encolar("Asegura que estaba cenando solo en Burger King.");
    encolar("Afirma que se encontraba viendo una batalla de aura.");
    encolar("Declara que estuvo paseando a su pez.");
    encolar("Sostiene que estaba en su casa durmiendo.");
    encolar("Manifiesta que asistia a una conferencia de negocios.");
    encolar("Afirma que estaba en casa de su madre.");
    encolar("Asegura que se encontraba entrenando en el gimnasio.");
    encolar("Sostiene que no se encontraba en la ciudad.");
    encolar("Se encontraba en su casa preparando la cena.");
    encolar("Estaba en la biblioteca estudiando.");
  }

  /**
   * @brief Inserta una coartada al final de la cola usando aritmética modular
   * (% 10).
   */
  void encolar(string coartada) {
    if (tamano == 10)
      return;
    ultimo = (ultimo + 1) % 10;
    elementos[ultimo] = coartada;
    tamano++;
  }

  /**
   * @brief Extrae la coartada en la cabeza y la reinserta al final para
   * mantener el giro infinito.
   * @return string Declaración asignada al sospechoso.
   */
  string desencolarYRotar() {
    if (tamano == 0)
      return "Sin coartada.";
    string dato = elementos[cabeza];
    cabeza = (cabeza + 1) % 10;
    ultimo = (ultimo + 1) % 10;
    elementos[ultimo] = dato;
    return dato;
  }
};

// Instancia global de la cola circular
ColaCircularCoartadas CoartadasCirculares;

// ============================================================================
// 3. PILA PARA EL HISTORIAL DE EVIDENCIAS
// ============================================================================

/**
 * @struct NodoPila
 * @brief Nodo autorreferenciado para almacenar una pista dentro de la Pila de
 * Evidencias.
 */
struct NodoPila {
  string descripcionEvidencia;
  NodoPila *sig;
  NodoPila(string d) : descripcionEvidencia(d), sig(NULL) {}
};

/**
 * @class PilaEvidencias
 * @brief Estructura donde la última pista
 * queda en el tope.
 */
class PilaEvidencias {
public:
  NodoPila *cabeza; // Puntero al tope de la pila

  PilaEvidencias() { cabeza = NULL; }

  /**
   * @brief Destructor que desapila y libera toda la memoria dinámica al
   * cerrarse el caso.
   */
  ~PilaEvidencias() {
    while (cabeza != NULL) {
      desapilar();
    }
  }

  /**
   * @brief Operación Push: Inserta una nueva evidencia en la cima de la pila en
   * O(1).
   */
  void apilar(string evidencia) {
    NodoPila *nuevo = new NodoPila(evidencia);
    nuevo->sig = cabeza;
    cabeza = nuevo;
  }

  /**
   * @brief Operación Pop: Elimina el nodo en la cima de la pila y libera su
   * memoria en O(1).
   */
  void desapilar() {
    if (cabeza != NULL) {
      NodoPila *temp = cabeza;
      cabeza = cabeza->sig;
      delete temp;
    }
  }
};

// ============================================================================
// 4. COLA CON PRIORIDAD PARA GESTIÓN DE CASOS
// ============================================================================

/**
 * @class NodoCaso
 * @brief Representa un expediente criminal dentro de la Cola con Prioridad.
 */
class NodoCaso {
public:
  int idCaso;         // Identificador único del caso
  string descripcion; // Delito reportado
  int prioridad;      // Nivel de urgencia (1: Alta, 2: Media, 3: Baja)
  Sospechoso
      sospechosos[5]; // Subconjunto de 5 sospechosos sorteados sin repetición
  int indiceCulpable; // Posición (0 a 4) del verdadero culpable
  int evidenciasMostradas; // Cantidad de pistas recolectadas (máximo 4)
  int idDetectiveAsignado; // Índice del detective a cargo (-1 si no tiene)
  PilaEvidencias historialEvidencias; // Pila propia del expediente
  NodoCaso *sig;                      // Puntero al siguiente caso en la cola

  /**
   * @brief Constructor que sortea 5 sospechosos únicos, define al culpable y
   * rota 5 coartadas.
   */
  NodoCaso(int _id, string _desc, int _prio) {
    idCaso = _id;
    descripcion = _desc;
    prioridad = _prio;
    evidenciasMostradas = 0;
    idDetectiveAsignado = -1;
    sig = NULL;
    indiceCulpable = rand() % 5;

    // Muestreo aleatorio sin reemplazo mediante arreglo booleano de marcas
    bool yaElegido[15] = {false};
    for (int i = 0; i < 5; i++) {
      int r;
      do {
        r = rand() % 15;
      } while (yaElegido[r]);
      yaElegido[r] = true;

      sospechosos[i] = CatalogoSospechosos[r];
      sospechosos[i].coartada = CoartadasCirculares.desencolarYRotar();
      sospechosos[i].esCulpable = (i == indiceCulpable);
    }
  }

  /**
   * @brief Extrae el siguiente rasgo del culpable y lo apila en el historial
   * del caso.
   * @return true si se apiló una nueva pista; false si ya se revelaron las 4
   * evidencias.
   */
  bool solicitarNuevaEvidencia() {
    if (evidenciasMostradas >= 4)
      return false;
    string nuevaPista = "El culpable " +
                        sospechosos[indiceCulpable].rasgos[evidenciasMostradas];
    historialEvidencias.apilar(nuevaPista);
    evidenciasMostradas++;
    return true;
  }
};

/**
 * @class ColaPrioridadCasos
 * @brief Lista ordenada por urgencia donde la cabeza es siempre el
 * caso más prioritario.
 */
class ColaPrioridadCasos {
public:
  NodoCaso *cabeza;

  ColaPrioridadCasos() { cabeza = NULL; }

  /**
   * @brief Destructor que libera todos los casos pendientes al finalizar el
   * servidor.
   */
  ~ColaPrioridadCasos() {
    while (cabeza != NULL) {
      eliminarPrimero();
    }
  }

  /**
   * @brief Inserta un caso según su prioridad (1 antes que 2, 2 antes que 3)
   * con desempate FIFO.
   */
  void insertarConPrioridad(int id, string desc, int prio) {
    NodoCaso *nuevo = new NodoCaso(id, desc, prio);
    if (cabeza == NULL || prio < cabeza->prioridad) {
      nuevo->sig = cabeza;
      cabeza = nuevo;
    } else {
      NodoCaso *actual = cabeza;
      while (actual->sig != NULL && actual->sig->prioridad <= prio) {
        actual = actual->sig;
      }
      nuevo->sig = actual->sig;
      actual->sig = nuevo;
    }
  }

  /**
   * @brief Desencola el caso en la cabeza tras emitirse el veredicto y libera
   * su memoria en O(1).
   */
  void eliminarPrimero() {
    if (cabeza != NULL) {
      NodoCaso *aux = cabeza;
      cabeza = cabeza->sig;
      delete aux; // Dispara automáticamente ~PilaEvidencias() del caso cerrado
    }
  }
};

#endif