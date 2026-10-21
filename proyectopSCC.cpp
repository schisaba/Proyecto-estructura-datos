/**
 * @file SistemaPolicial.cpp
 * @authors Sebastian Chisaba Cruz
 *          Ariadna Valentina Ramirez Santeliz
 *          Jeronimo Pascuas Barroso
 * @brief Sistema de gestión de casos criminales, interrogatorios y asignación
 * de detectives.
 * @details Implementa Cola con Prioridad, Pila Dinámica, Cola Circular y
 * Registros (struct).
 * @note Declaración de Uso de IA: Se empleó un 45% de asistencia de
 * Inteligencia Artificial para la estructuración de la documentación en línea y
 * el pulido/refactorización de clases.
 */

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

// ============================================================================
// 1. REGISTROS (STRUCT) PARA SOSPECHOSOS Y DETECTIVES
// ============================================================================

/**
 * @struct Sospechoso
 * @brief Ficha de datos que modela a un sospechoso en el catálogo y dentro de
 * cada caso.
 */
struct Sospechoso {
  string nombre;
  string rasgos[4];
  string coartada;
  bool esCulpable;
};

/**
 * @struct Detective
 * @brief Registro que agrupa los datos de cada detective de la central.
 */
struct Detective {
  string nombre;
  bool disponible;
  int casosResueltos;
};

// Catálogo global estático de 15 sospechosos
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

/**
 * @brief Función auxiliar que muestra la tabla completa de detectives con su
 * disponibilidad.
 */
void mostrarDetectives(Detective detectives[], int cantidad) {
  printf("\n%-6s %-15s %-15s %s\n", "ID", "Nombre", "Estado",
         "Casos Resueltos");
  printf("--------------------------------------------------\n");
  for (int i = 0; i < cantidad; i++) {
    printf("%-6d %-15s %-15s %d\n", i + 1, detectives[i].nombre.c_str(),
           detectives[i].disponible ? "Disponible" : "Ocupado",
           detectives[i].casosResueltos);
  }
}

// ============================================================================
// 2. COLA CIRCULAR ESTÁTICA PARA COARTADAS
// ============================================================================

/**
 * @class ColaCircularCoartadas
 * @brief Estructura FIFO circular de 10 posiciones que rota coartadas en O(1).
 */
class ColaCircularCoartadas {
public:
  string elementos[10];
  int cabeza;
  int ultimo;
  int tamano;

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

  bool estaLlena() { return tamano == 10; }
  bool estaVacia() { return tamano == 0; }

  void encolar(string coartada) {
    if (estaLlena())
      return;
    ultimo = (ultimo + 1) % 10;
    elementos[ultimo] = coartada;
    tamano++;
  }

  string desencolarYRotar() {
    if (estaVacia())
      return "Sin coartada declarada.";

    string dato = elementos[cabeza];
    cabeza = (cabeza + 1) % 10;
    ultimo = (ultimo + 1) % 10;
    elementos[ultimo] = dato;

    return dato;
  }
};

ColaCircularCoartadas CoartadasCirculares;

// ============================================================================
// 3. PILA DINÁMICA PARA EL HISTORIAL DE EVIDENCIAS
// ============================================================================

struct NodoPila {
  string descripcionEvidencia;
  NodoPila *sig;
  NodoPila(string d) : descripcionEvidencia(d), sig(NULL) {}
};

/**
 * @class PilaEvidencias
 * @brief Estructura LIFO enlazada donde la evidencia más reciente queda en la
 * cima.
 */
class PilaEvidencias {
public:
  NodoPila *cabeza;

  PilaEvidencias() { cabeza = NULL; }

  ~PilaEvidencias() {
    while (!estaVacia()) {
      desapilar();
    }
  }

  bool estaVacia() { return cabeza == NULL; }

  void apilar(string evidencia) {
    NodoPila *nuevo = new NodoPila(evidencia);
    nuevo->sig = cabeza;
    cabeza = nuevo;
  }

  void desapilar() {
    if (!estaVacia()) {
      NodoPila *temp = cabeza;
      cabeza = cabeza->sig;
      delete temp;
    }
  }

  void mostrarHistorial() {
    if (estaVacia()) {
      cout << "\n[!] No hay evidencias apiladas hasta el momento." << endl;
      return;
    }
    cout << "\n--- HISTORIAL DE EVIDENCIAS RECOPILADAS ---" << endl;
    NodoPila *actual = cabeza;
    int contador = 1;
    while (actual != NULL) {
      cout << "Pista [" << contador++ << "]: " << actual->descripcionEvidencia
           << endl;
      actual = actual->sig;
    }
  }
};

// ============================================================================
// 4. COLA CON PRIORIDAD DINÁMICA PARA GESTIÓN DE CASOS
// ============================================================================

/**
 * @class NodoCaso
 * @brief Expediente criminal individual dentro de la cola de prioridad.
 */
class NodoCaso {
public:
  int idCaso;
  string descripcion;
  int prioridad; // 1: Alta, 2: Media, 3: Baja

  Sospechoso sospechosos[5];
  int indiceCulpable;
  int evidenciasMostradas;
  int idDetectiveAsignado; // Guarda la posición (0 a 3) del detective, o -1 si
                           // no tiene

  PilaEvidencias historialEvidencias;
  NodoCaso *sig;

  NodoCaso(int _id, string _desc, int _prio) {
    idCaso = _id;
    descripcion = _desc;
    prioridad = _prio;
    evidenciasMostradas = 0;
    idDetectiveAsignado = -1;
    sig = NULL;

    indiceCulpable = rand() % 5;

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

  bool solicitarNuevaEvidencia() {
    if (evidenciasMostradas >= 4)
      return false;

    string nuevaPista = "El culpable: " +
                        sospechosos[indiceCulpable].rasgos[evidenciasMostradas];
    historialEvidencias.apilar(nuevaPista);
    evidenciasMostradas++;

    return true;
  }
};

/**
 * @class ColaPrioridadCasos
 * @brief Cola con prioridad donde el frente es siempre el caso de mayor
 * urgencia.
 */
class ColaPrioridadCasos {
public:
  NodoCaso *cabeza;

  ColaPrioridadCasos() { cabeza = NULL; }

  ~ColaPrioridadCasos() {
    while (!esVacia()) {
      eliminarPrimero();
    }
  }

  bool esVacia() { return cabeza == NULL; }

  void insertarConPrioridad(int id, string desc, int prio) {
    NodoCaso *nuevo = new NodoCaso(id, desc, prio);

    if (esVacia() || prio < cabeza->prioridad) {
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

  void eliminarPrimero() {
    if (!esVacia()) {
      NodoCaso *aux = cabeza;
      cabeza = cabeza->sig;
      delete aux;
    }
  }

  void mostrarCasos(Detective detectives[]) {
    if (esVacia()) {
      cout << "\n[!] No hay casos pendientes en la central." << endl;
      return;
    }
    cout << "\n--- CASOS PENDIENTES ---" << endl;
    printf("%-6s %-12s %-16s %-20s %s\n", "ID", "Prioridad", "Evidencias",
           "Detective Asignado", "Descripcion");
    printf("-------------------------------------------------------------------"
           "-------------\n");

    NodoCaso *actual = cabeza;
    while (actual != NULL) {
      string prioTexto = (actual->prioridad == 1)   ? "Alta (1)"
                         : (actual->prioridad == 2) ? "Media (2)"
                                                    : "Baja (3)";
      string estadoEv =
          (actual->evidenciasMostradas > 0)
              ? to_string(actual->evidenciasMostradas) + "/4 Revisadas"
              : "Sin revisar";
      string detNom = (actual->idDetectiveAsignado == -1)
                          ? "Sin asignar"
                          : detectives[actual->idDetectiveAsignado].nombre;

      printf("%-6d %-12s %-16s %-20s %s\n", actual->idCaso, prioTexto.c_str(),
             estadoEv.c_str(), detNom.c_str(), actual->descripcion.c_str());

      actual = actual->sig;
    }
  }
};

// ============================================================================
// 5. EVENTOS EN TIEMPO REAL Y PROGRAMA PRINCIPAL
// ============================================================================

void verificarNuevosCasos(ColaPrioridadCasos &colaCasos,
                          chrono::steady_clock::time_point &ultimoTiempo,
                          int &contadorCasos) {
  auto ahora = chrono::steady_clock::now();
  auto transcurrido =
      chrono::duration_cast<chrono::seconds>(ahora - ultimoTiempo).count();

  if (transcurrido >= 60) {
    string descripciones[] = {"Homicidio de trabajador publico",
                              "Asesinato en Ciudad Bolivar", "Crimen en Suba",
                              "Envenenamiento en Little Caesars"};
    int prioridades[] = {1, 1, 2, 2};

    int i = rand() % 4;
    colaCasos.insertarConPrioridad(contadorCasos++, descripciones[i],
                                   prioridades[i]);

    cout
        << "\n\n [!] ALERTA POLICIAL: Ha entrado un nuevo expediente criminal: "
        << descripciones[i] << " (Prioridad " << prioridades[i] << ")\n"
        << endl;

    ultimoTiempo = ahora;
  }
}

int main() {
  srand(time(0));

  int opc = 0, subOpc = 0;

  // Arreglo de estructuras (struct Detective) con los 4 agentes de la central
  const int cantDetectives = 4;
  Detective detectives[cantDetectives] = {{"Sebastian", true, 3},
                                          {"Jeronimo", true, 5},
                                          {"Daniel", true, 2},
                                          {"Valentina", true, 4}};

  ColaPrioridadCasos colaCasos;
  int contadorCasos = 101;

  colaCasos.insertarConPrioridad(contadorCasos++,
                                 "Homicidio en el Club Nocturno", 1);
  colaCasos.insertarConPrioridad(contadorCasos++,
                                 "Muerte sospechosa en la Estacion", 2);

  auto ultimoTiempo = chrono::steady_clock::now();

  while (opc != 5) {
    verificarNuevosCasos(colaCasos, ultimoTiempo, contadorCasos);

    cout << "\n------------------- MENU PRINCIPAL -----------------" << endl;
    cout << "1. Consultar detectives" << endl;
    cout << "2. Sala de Evidencias" << endl;
    cout << "3. Expedientes / Asignar y Resolver Caso" << endl;
    cout << "4. Interrogar y Ver Perfiles de Sospechosos" << endl;
    cout << "5. Salir" << endl;
    cout << "Seleccione una opcion: ";
    cin >> opc;

    switch (opc) {
    case 1:
      mostrarDetectives(detectives, cantDetectives);
      break;

    case 2:
      subOpc = 0;
      while (subOpc != 3) {
        verificarNuevosCasos(colaCasos, ultimoTiempo, contadorCasos);
        if (colaCasos.esVacia()) {
          cout << "\n[!] No hay casos activos para inspeccionar evidencias."
               << endl;
          break;
        }

        NodoCaso *casoActual = colaCasos.cabeza;
        cout << "\n=== SALA DE EVIDENCIAS - CASO #" << casoActual->idCaso
             << " ===" << endl;
        cout << "1. Ver historial de evidencias" << endl;
        cout << "2. Procesar nueva evidencia de la escena" << endl;
        cout << "3. Volver al menu principal" << endl;
        cout << "Opcion: ";
        cin >> subOpc;

        if (subOpc == 1) {
          casoActual->historialEvidencias.mostrarHistorial();
        } else if (subOpc == 2) {
          if (casoActual->solicitarNuevaEvidencia()) {
            cout << "\n[+] ¡NUEVA EVIDENCIA APILADA CON EXITO!" << endl;
          } else {
            cout << "\n[!] Ya se recolectaron las 4 evidencias del caso."
                 << endl;
          }
        }
      }
      break;

    case 3:
      subOpc = 0;
      while (subOpc != 4) {
        verificarNuevosCasos(colaCasos, ultimoTiempo, contadorCasos);
        cout << "\n---------- Expedientes / Casos ----------" << endl;
        cout << "1. Ver cola de casos prioritarios" << endl;
        cout << "2. Asignar Detective al Caso Prioritario" << endl;
        cout << "3. Emitir Veredicto y Cerrar Caso Urgente" << endl;
        cout << "4. Volver al menu principal" << endl;
        cout << "Opcion: ";
        cin >> subOpc;

        if (subOpc == 1) {
          colaCasos.mostrarCasos(detectives);
        } else if (subOpc == 2) {
          if (colaCasos.esVacia()) {
            cout << "\n[!] No hay casos pendientes para asignar." << endl;
          } else {
            NodoCaso *casoActual = colaCasos.cabeza;

            if (casoActual->idDetectiveAsignado != -1) {
              cout << "\n[!] Este caso ya esta asignado al detective: "
                   << detectives[casoActual->idDetectiveAsignado].nombre
                   << endl;
            } else {
              // Mostramos directamente la tabla completa con el estado
              // Disponible / Ocupado
              mostrarDetectives(detectives, cantDetectives);

              int idDet;
              cout << "\nSeleccione el ID del detective (1 a " << cantDetectives
                   << "): ";
              cin >> idDet;

              if (idDet >= 1 && idDet <= cantDetectives &&
                  detectives[idDet - 1].disponible) {
                casoActual->idDetectiveAsignado = idDet - 1;
                detectives[idDet - 1].disponible = false;
                cout << "\n[+] Caso #" << casoActual->idCaso << " asignado a "
                     << detectives[idDet - 1].nombre << "." << endl;
              } else {
                cout << "[!] ID invalido o el detective seleccionado esta "
                        "ocupado."
                     << endl;
              }
            }
          }
        } else if (subOpc == 3) {
          if (colaCasos.esVacia()) {
            cout << "\n[!] No hay casos pendientes." << endl;
          } else {
            NodoCaso *casoActual = colaCasos.cabeza;

            if (casoActual->idDetectiveAsignado == -1) {
              cout << "\n[!] REQUISITO FALTANTE: Asigne un detective al caso "
                      "primero (Opcion 2)."
                   << endl;
            } else if (casoActual->evidenciasMostradas == 0) {
              cout << "\n[!] REQUISITO FALTANTE: Debe solicitar al menos 1 "
                      "evidencia en el Menu 2."
                   << endl;
            } else {
              int detIdx = casoActual->idDetectiveAsignado;

              cout << "\n=== ORDEN DE ARRESTO - CASO #" << casoActual->idCaso
                   << " ===" << endl;
              cout << "Detective a cargo: " << detectives[detIdx].nombre
                   << endl;
              cout << "Seleccione al sospechoso que desea arrestar:\n" << endl;

              for (int i = 0; i < 5; i++) {
                cout << "\n[" << i + 1
                     << "] Nombre: " << casoActual->sospechosos[i].nombre
                     << endl;
                cout << "    Rasgos conocidos:" << endl;
                for (int r = 0; r < 4; r++) {
                  cout << "      - " << casoActual->sospechosos[i].rasgos[r]
                       << endl;
                }
              }

              int eleccion;
              cout << "\nOpcion (1 a 5): ";
              cin >> eleccion;

              if (eleccion >= 1 && eleccion <= 5) {
                int idxElegido = eleccion - 1;

                if (casoActual->sospechosos[idxElegido].esCulpable) {
                  cout << "\n ¡ERA EL CULPABLE! "
                       << casoActual->sospechosos[idxElegido].nombre
                       << " ¿Creia que podia desafiar al Estado? ¡Bienvenido a "
                          "la patria milagro!"
                       << endl;
                  detectives[detIdx].casosResueltos++;
                } else {
                  int cIdx = casoActual->indiceCulpable;
                  cout << "\n ¡ARRESTO ERRONEO! "
                       << casoActual->sospechosos[idxElegido].nombre
                       << " era inocente." << endl;
                  cout << "El VERDADERO ASESINO ERA: "
                       << casoActual->sospechosos[cIdx].nombre << "." << endl;
                }

                detectives[detIdx].disponible = true;
                cout << "[+] El detective " << detectives[detIdx].nombre
                     << " ha sido liberado para nuevos casos." << endl;

                colaCasos.eliminarPrimero();
              } else {
                cout << "Seleccion invalida." << endl;
              }
            }
          }
        }
      }
      break;

    case 4:
      if (colaCasos.esVacia()) {
        cout << "\n[!] No hay casos activos para consultar sospechosos."
             << endl;
      } else {
        NodoCaso *casoActual = colaCasos.cabeza;
        cout << "\n=== PERFILES Y RASGOS DE SOSPECHOSOS DEL CASO #"
             << casoActual->idCaso << " ===" << endl;
        for (int i = 0; i < 5; i++) {
          cout << "\n[" << i + 1
               << "] Nombre: " << casoActual->sospechosos[i].nombre << endl;
          cout << "    Rasgos conocidos:" << endl;
          for (int r = 0; r < 4; r++) {
            cout << "      - " << casoActual->sospechosos[i].rasgos[r] << endl;
          }
          cout << "    Coartada declarada: "
               << casoActual->sospechosos[i].coartada << endl;
        }
      }
      break;

    case 5:
      cout << "Saliendo del sistema policial." << endl;
      break;
    }
  }

  return 0;
}