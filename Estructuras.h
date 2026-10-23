#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// 1. REGISTROS (STRUCT) PARA SOSPECHOSOS Y DETECTIVES
struct Sospechoso {
  string nombre;
  string rasgos[4];
  string coartada;
  bool esCulpable;
};

struct Detective {
  string nombre;
  bool disponible;
  int casosResueltos;
};

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

// 2. COLA CIRCULAR ESTÁTICA PARA COARTADAS
class ColaCircularCoartadas {
public:
  string elementos[10];
  int cabeza, ultimo, tamano;

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

  void encolar(string coartada) {
    if (tamano == 10)
      return;
    ultimo = (ultimo + 1) % 10;
    elementos[ultimo] = coartada;
    tamano++;
  }

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

ColaCircularCoartadas CoartadasCirculares;

// 3. PILA DINÁMICA PARA EL HISTORIAL DE EVIDENCIAS
struct NodoPila {
  string descripcionEvidencia;
  NodoPila *sig;
  NodoPila(string d) : descripcionEvidencia(d), sig(NULL) {}
};

class PilaEvidencias {
public:
  NodoPila *cabeza;
  PilaEvidencias() { cabeza = NULL; }
  ~PilaEvidencias() {
    while (cabeza != NULL)
      desapilar();
  }

  void apilar(string evidencia) {
    NodoPila *nuevo = new NodoPila(evidencia);
    nuevo->sig = cabeza;
    cabeza = nuevo;
  }

  void desapilar() {
    if (cabeza != NULL) {
      NodoPila *temp = cabeza;
      cabeza = cabeza->sig;
      delete temp;
    }
  }
};

// 4. COLA CON PRIORIDAD DINÁMICA PARA GESTIÓN DE CASOS
class NodoCaso {
public:
  int idCaso;
  string descripcion;
  int prioridad;
  Sospechoso sospechosos[5];
  int indiceCulpable;
  int evidenciasMostradas;
  int idDetectiveAsignado;
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
    string nuevaPista = "El culpable " +
                        sospechosos[indiceCulpable].rasgos[evidenciasMostradas];
    historialEvidencias.apilar(nuevaPista);
    evidenciasMostradas++;
    return true;
  }
};

class ColaPrioridadCasos {
public:
  NodoCaso *cabeza;
  ColaPrioridadCasos() { cabeza = NULL; }
  ~ColaPrioridadCasos() {
    while (cabeza != NULL)
      eliminarPrimero();
  }

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

  void eliminarPrimero() {
    if (cabeza != NULL) {
      NodoCaso *aux = cabeza;
      cabeza = cabeza->sig;
      delete aux;
    }
  }
};

#endif