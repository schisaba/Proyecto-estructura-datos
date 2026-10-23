const API = "http://localhost:8080";
let segundos = 60;

async function llamarCPlusPlus(ruta = "/") {
    try {
        const res = await fetch(API + ruta);
        const data = await res.json();
        renderizarInterfaz(data);
    } catch (e) {
        document.getElementById("banner-mensaje").innerText =
            " Ejecuta primero './servidor' en tu terminal de C++ para conectar la interfaz.";
    }
}

function renderizarInterfaz(data) {
    document.getElementById("banner-mensaje").innerText = data.mensaje;

    // 1. Renderizar Cola con Prioridad
    const divCasos = document.getElementById("lista-casos");
    divCasos.innerHTML = data.casos.length === 0 ? "<p>No hay casos pendientes.</p>" : "";
    data.casos.forEach((c, idx) => {
        const textoPrio = c.prioridad === 1 ? "Alta (1)" : c.prioridad === 2 ? "Media (2)" : "Baja (3)";
        divCasos.innerHTML += `
      <div class="card-caso ${idx === 0 ? 'activo' : ''}">
        <div style="display:flex; justify-content:space-between;">
          <strong>#${c.id} - ${c.descripcion}</strong>
          <span class="badge prio-${c.prioridad}">${textoPrio}</span>
        </div>
        <div style="font-size:0.8rem; color:#cbd5e1; margin-top:6px;">
           Detective: <b>${c.detective}</b> |  Pistas: <b>${c.evidencias}/4</b>
          ${idx === 0 ? ' <span style="color:#f87171;">(EN ATENCIÓN)</span>' : ''}
        </div>
      </div>`;
    });

    // 2. Renderizar Detectives
    const divDet = document.getElementById("lista-detectives");
    divDet.innerHTML = "";
    data.detectives.forEach((d, idx) => {
        divDet.innerHTML += `
      <div class="card-det" onclick="llamarCPlusPlus('/asignar?id=${idx}')">
        <strong>#${d.id} ${d.nombre}</strong>
        <div style="font-size:0.8rem; color:${d.disponible ? '#4ade80' : '#f87171'};">
          ● ${d.disponible ? 'Disponible (Clic para asignar)' : 'Ocupado'}
        </div>
        <div style="font-size:0.78rem; color:#94a3b8; margin-top:4px;">Casos resueltos: ${d.resueltos}</div>
      </div>`;
    });

    // 3. Renderizar Pila de Evidencias y Sospechosos del Caso Prioritario
    const divPila = document.getElementById("pila-evidencias");
    const divSosp = document.getElementById("lista-sospechosos");

    if (!data.casoActivo) {
        divPila.innerHTML = "<p>Sin caso activo.</p>";
        divSosp.innerHTML = "<p>Sin sospechosos por interrogar.</p>";
        return;
    }

    divPila.innerHTML = data.casoActivo.pilaEvidencias.length === 0
        ? "<p style='color:#94a3b8;'>Pila vacía. Presiona el botón para recolectar la primera pista.</p>"
        : "";
    data.casoActivo.pilaEvidencias.forEach((pista, i) => {
        divPila.innerHTML += `
      <div class="nodo-pila">
        <strong>${i === 0 ? ' [TOPE DE LA PILA]' : `Pista #${i + 1}`}</strong>: ${pista}
      </div>`;
    });

    divSosp.innerHTML = "";
    data.casoActivo.sospechosos.forEach((s, i) => {
        const tags = s.rasgos.map(r => `<span class="tag-rasgo">${r}</span>`).join("");
        divSosp.innerHTML += `
      <div class="card-sosp">
        <strong>[${i + 1}] ${s.nombre}</strong>
        <div class="coartada"> "${s.coartada}"</div>
        <div class="rasgos-list">${tags}</div>
        <button class="btn-arrestar" onclick="llamarCPlusPlus('/arrestar?idx=${i}')">
           Emitir Orden de Arresto contra ${s.nombre}
        </button>
      </div>`;
    });
}

function solicitarEvidencia() { llamarCPlusPlus("/evidencia"); }
function dispararAlerta() { segundos = 60; llamarCPlusPlus("/alerta"); }

// Reloj automático de 60 segundos conectado al servidor C++
setInterval(() => {
    segundos--;
    if (segundos <= 0) {
        dispararAlerta();
    }
    document.getElementById("timer").innerText = segundos;
}, 1000);

llamarCPlusPlus();