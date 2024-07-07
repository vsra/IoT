document.getElementById('wifiForm').addEventListener('submit', function(event) {
  event.preventDefault();

  let username = document.getElementById('username').value;
  let password = document.getElementById('password').value;

  conectarRedWiFi(username, password);
});



// Función para conectar a la red WiFi
async function conectarRedWiFi(user, password) {
  try {
      // URL a la que se enviarán los datos
      let url = '/solicitudEnviada';

      // Datos a enviar en la solicitud
      let datos = {
          usernamee: user,
          password: password
      };

      // Opciones para la solicitud HTTP POST
      let opciones = {
          method: 'POST',
          headers: {
              'Content-Type': 'application/json' // Indicamos que el cuerpo es JSON
          },
          body: JSON.stringify(datos) // Convertimos los datos a formato JSON
      };
      console.log(opciones.body);

      // Realizamos la solicitud HTTP
      let respuesta = await fetch(url, opciones);
      let resultado = await respuesta.json();

      return resultado;
  } catch (error) {
      throw new Error('Error al conectar a la red WiFi: ' + error.message);
  }
}


// Función para manejar la acción del reproductor de sonido
async function accionReproductor(accion) {
    try {
        let url = '/controlSonido';

        let acciones = {
            accionSeleccionada: accion
        };

        switch (accion) {
            case 'play':
                console.log('Reproduciendo sonido...');
                //acciones.accionSeleccionada = accion;

                break;
            case 'pausa':
                console.log('Pausando sonido...');
                accionSeleccionada = "pausa";

                break;
            case 'siguiente':
                console.log('Cambiando al siguiente sonido...');
                // Aquí podrías poner la lógica para ir al siguiente sonido
                break;
            case 'anterior':
                console.log('Cambiando al sonido anterior...');
                // Aquí podrías poner la lógica para ir al sonido anterior
                break;
            case 'subir':
                console.log('Subiendo volumen...');
                // Aquí podrías poner la lógica para subir el volumen
                break;
            case 'bajar':
                console.log('Bajando volumen...');
                // Aquí podrías poner la lógica para bajar el volumen
                break;
            case 'detener':
                console.log('Deteniendo sonido...');
                // Aquí podrías poner la lógica para detener el sonido
                break;
            default:
                console.log('Acción no reconocida.');
        }

        let opcionSeleccionada = {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json' 
            },
            body: JSON.stringify(acciones)
        };
        console.log(opcionSeleccionada.body);

        let respuesta = await fetch(url, opcionSeleccionada);
        let resultado = await respuesta.json();

        return resultado;
    } catch (error) {
        throw new Error('Error al conectar a la red WiFi: ' + error.message);
    }
}




document.getElementById('mqttForm').addEventListener('submit', function(event) {
    event.preventDefault();

    // Obtener valores del formulario MQTT
    let nombre = document.getElementById('name2').value;
    let telefono = document.getElementById('phone').value;

    // Aquí puedes llamar a la función para enviar datos a MQTT
    enviarDatosMQTT(nombre, telefono);
});

// Función para conectar a la red WiFi
function conectarRedWiFi(username, password) {
    console.log('Conectando a red WiFi...');
    // Aquí iría la lógica real para conectar a una red WiFi
}

// Función para enviar datos a MQTT
function enviarDatosMQTT(nombre, telefono) {
    console.log('Enviando datos a MQTT...');
    // Aquí iría la lógica real para enviar datos a un servidor MQTT
}
