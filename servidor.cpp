/**
 * @file servidor.cpp
 * @authors Sebastian Chisaba Cruz
 *          Ariadna Valentina Ramirez Santeliz
 *          Jeronimo Pascuas Barroso
 * @brief Servidor HTTP local en C++ que conecta las estructuras de datos con la
 * interfaz web HTML.
 * @details Escucha peticiones en el puerto 8080, ejecuta operaciones sobre la
 * Cola con Prioridad, la Pila de Evidencias, la Cola Circular y los Registros,
 * y responde en formato JSON.
 */

#include "Estructuras.h"
#include <cstring>
#include <ctime>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// Plantilla fija de 4 detectives almacenada en un arreglo estático de Registros
// (struct)
Detective detectives[4] = {{"Sebastian", true, 3},
                           {"Jeronimo", true, 5},
                           {"Daniel", true, 2},
                           {"Valentina", true, 4}};

// Cola con Prioridad global de expedientes activos
ColaPrioridadCasos colaCasos;
int contadorCasos = 101;
string ultimoMensaje = "Sistema policial iniciado correctamente.";

/**
 * @brief Serializa el estado actual de todas las estructuras de datos en C++ a
 * formato JSON.
 * @return string Cadena JSON enviada al navegador web.
 */
string obtenerEstadoJSON() {
  stringstream ss;
  ss << "{";
  ss << "\"mensaje\":\"" << ultimoMensaje << "\",";

  // 1. Exportar arreglo de Registros de Detectives
  ss << "\"detectives\":[";
  for (int i = 0; i < 4; i++) {
    ss << "{\"id\":" << (i + 1) << ",\"nombre\":\"" << detectives[i].nombre
       << "\","
       << "\"disponible\":" << (detectives[i].disponible ? "true" : "false")
       << ","
       << "\"resueltos\":" << detectives[i].casosResueltos << "}"
       << (i < 3 ? "," : "");
  }
  ss << "],";

  // 2. Exportar todos los nodos de la Cola con Prioridad de Casos
  ss << "\"casos\":[";
  NodoCaso *act = colaCasos.cabeza;
  while (act != NULL) {
    string detNom = (act->idDetectiveAsignado == -1)
                        ? "Sin asignar"
                        : detectives[act->idDetectiveAsignado].nombre;
    ss << "{\"id\":" << act->idCaso << ",\"descripcion\":\"" << act->descripcion
       << "\","
       << "\"prioridad\":" << act->prioridad
       << ",\"evidencias\":" << act->evidenciasMostradas << ","
       << "\"detective\":\"" << detNom << "\"}";
    if (act->sig != NULL)
      ss << ",";
    act = act->sig;
  }
  ss << "],";

  // 3. Exportar el Caso Activo (Cabeza de la Cola), su Pila y sus 5 Sospechosos
  if (colaCasos.cabeza != NULL) {
    NodoCaso *c = colaCasos.cabeza;
    ss << "\"casoActivo\":{\"id\":" << c->idCaso << ",\"descripcion\":\""
       << c->descripcion << "\","
       << "\"idDetective\":" << c->idDetectiveAsignado
       << ",\"evidenciasMostradas\":" << c->evidenciasMostradas << ",";

    // Recorrido desde el tope de la Pila de Evidencias
    ss << "\"pilaEvidencias\":[";
    NodoPila *p = c->historialEvidencias.cabeza;
    while (p != NULL) {
      ss << "\"" << p->descripcionEvidencia << "\"";
      if (p->sig != NULL)
        ss << ",";
      p = p->sig;
    }
    ss << "],";

    // Exportar los 5 sospechosos del caso y sus coartadas rotadas por la Cola
    // Circular
    ss << "\"sospechosos\":[";
    for (int i = 0; i < 5; i++) {
      ss << "{\"nombre\":\"" << c->sospechosos[i].nombre << "\","
         << "\"coartada\":\"" << c->sospechosos[i].coartada << "\","
         << "\"rasgos\":[\"" << c->sospechosos[i].rasgos[0] << "\",\""
         << c->sospechosos[i].rasgos[1] << "\",\""
         << c->sospechosos[i].rasgos[2] << "\",\""
         << c->sospechosos[i].rasgos[3] << "\"]}" << (i < 4 ? "," : "");
    }
    ss << "]}";
  } else {
    ss << "\"casoActivo\":null";
  }
  ss << "}";
  return ss.str();
}

/**
 * @brief Función principal que inicializa los casos base y levanta el servidor
 * en el puerto 8080.
 */
int main() {
  srand(time(0));

  // Carga de expedientes iniciales en la Cola con Prioridad
  colaCasos.insertarConPrioridad(contadorCasos++,
                                 "Homicidio en el Club Nocturno", 1);
  colaCasos.insertarConPrioridad(contadorCasos++,
                                 "Muerte sospechosa en la Estacion", 2);

  // Configuración del socket TCP en el puerto 8080
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  listen(server_fd, 10);

  cout << "=========================================================" << endl;
  cout << " MOTOR C++ ACTIVO EN: http://localhost:8080" << endl;
  cout << " Abre el archivo index.html en tu navegador para jugar." << endl;
  cout << "=========================================================" << endl;

  // Bucle de escucha de acciones enviadas desde la interfaz HTML/JS
  while (true) {
    int client = accept(server_fd, NULL, NULL);
    char buffer[2048] = {0};
    read(client, buffer, 2048);
    string req(buffer);

    // Ruta 1: Procesar y apilar nueva evidencia en la Pila del caso prioritario
    if (req.find("GET /evidencia") != string::npos &&
        colaCasos.cabeza != NULL) {
      if (colaCasos.cabeza->solicitarNuevaEvidencia())
        ultimoMensaje =
            "[Pila LIFO] ¡Nueva evidencia apilada en la cima del expediente!";
      else
        ultimoMensaje = "[!] Ya se recolectaron las 4 evidencias de este caso.";
    }
    // Ruta 2: Asignar un detective disponible al caso ubicado en la cabeza de
    // la cola
    else if (req.find("GET /asignar?id=") != string::npos &&
             colaCasos.cabeza != NULL) {
      int pos = req.find("id=") + 3;
      int idDet = req[pos] - '0';
      if (colaCasos.cabeza->idDetectiveAsignado != -1) {
        ultimoMensaje =
            "[!] El caso prioritario ya tiene un detective asignado.";
      } else if (idDet >= 0 && idDet < 4 && detectives[idDet].disponible) {
        colaCasos.cabeza->idDetectiveAsignado = idDet;
        detectives[idDet].disponible = false;
        ultimoMensaje = "[+] Detective " + detectives[idDet].nombre +
                        " asignado al Caso #" +
                        to_string(colaCasos.cabeza->idCaso);
      } else {
        ultimoMensaje = "[!] El detective seleccionado está ocupado.";
      }
    }
    // Ruta 3: Emitir orden de arresto, actualizar estadísticas y desencolar el
    // caso cerrado
    else if (req.find("GET /arrestar?idx=") != string::npos &&
             colaCasos.cabeza != NULL) {
      int pos = req.find("idx=") + 4;
      int idx = req[pos] - '0';
      NodoCaso *c = colaCasos.cabeza;

      if (c->idDetectiveAsignado == -1) {
        ultimoMensaje = "[!] REQUISITO: Asigna un detective antes de emitir la "
                        "orden de arresto.";
      } else if (c->evidenciasMostradas == 0) {
        ultimoMensaje = "[!] REQUISITO: Procesa al menos 1 evidencia en la "
                        "Pila antes de arrestar.";
      } else {
        int detIdx = c->idDetectiveAsignado;
        if (c->sospechosos[idx].esCulpable) {
          detectives[detIdx].casosResueltos++;
          ultimoMensaje = " ¡CASO RESUELTO! " + c->sospechosos[idx].nombre +
                          " era el culpable. (+1 punto para " +
                          detectives[detIdx].nombre + ")";
        } else {
          ultimoMensaje = " ¡ARRESTO ERRÓNEO! " + c->sospechosos[idx].nombre +
                          " era inocente. El culpable era " +
                          c->sospechosos[c->indiceCulpable].nombre + ".";
        }
        detectives[detIdx].disponible = true;
        colaCasos.eliminarPrimero();
      }
    }
    // Ruta 4: Disparar alerta policial e insertar nuevo expediente en la Cola
    // con Prioridad
    else if (req.find("GET /alerta") != string::npos) {
      string desc[] = {"Homicidio de trabajador publico",
                       "Asesinato en Ciudad Bolivar", "Crimen en Suba",
                       "Envenenamiento en Little Caesars"};
      int prios[] = {1, 1, 2, 3};
      int r = rand() % 4;
      colaCasos.insertarConPrioridad(contadorCasos++, desc[r], prios[r]);
      ultimoMensaje =
          " [ALERTA POLICIAL] Nuevo expediente encolado: " + desc[r] +
          " (Prioridad " + to_string(prios[r]) + ")";
    }

    // Envío de respuesta HTTP con cabeceras CORS y estado JSON actualizado
    string json = obtenerEstadoJSON();
    string resp = "HTTP/1.1 200 OK\r\nContent-Type: "
                  "application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n" +
                  json;
    send(client, resp.c_str(), resp.length(), 0);
    close(client);
  }
  return 0;
}