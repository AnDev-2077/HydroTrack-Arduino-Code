# Wi-Fi ESP - 32 Wroom 
Para configurar la placa se debe utilizar el siguiente código: 
```c++
#include <WiFi.h>

#include <FirebaseESP32.h>

#include <addons/TokenHelper.h>

#include <addons/RTDBHelper.h>

  

// Creamos el objeto servidor en el puerto 80

WiFiServer server(80);

  

// Variables para almacenar las credenciales ingresadas

String ssidInput = "";

String passwordInput = "";

  

// Página web HTML con un formulario para ingresar SSID y contraseña

String pagina = "<!DOCTYPE html>"

"<html>"

"<head>"

"<meta charset='utf-8' />"

"<title>Configuración Wi-Fi ESP32</title>"

"</head>"

"<body>"

"<center>"

"<h1>Conectar a una Red Wi-Fi</h1>"

"<form action='/connect' method='GET'>"

"SSID: <input type='text' name='ssid'><br>"

"Contraseña: <input type='password' name='password'><br><br>"

"<input type='submit' value='Conectar'>"

"</form>"

"</center>"

"</body>"

"</html>";

  

#define API_KEY  "AIzaSyBbLWtfazsb1OgUoeYszJ1l7fh2cfdWL_M"

#define DATABASE_URL "tank-1568c-default-rtdb.firebaseio.com"

String userEmail = "jhamgod@gmail.com";

String userPassword = "12345678";

FirebaseData fbdo;

FirebaseAuth auth;

FirebaseConfig config;

const long timeZone = -5;

const char* ntpServer = "pool.ntp.org";

unsigned long sendDataPrevMillis = 0;

  
  

String porcentaje = "-1";

  

void setup() {

  Serial.begin(115200);

  Serial.println();

  //pines

  pinMode(23, INPUT);

  pinMode(22, INPUT);

  pinMode(21, INPUT);

  pinMode(19, INPUT);

  pinMode(18, INPUT);

  pinMode(17, INPUT);

  pinMode(16, INPUT);

  pinMode(15, INPUT);

  // Configuramos el ESP32 como punto de acceso

  WiFi.softAP("ESP32_AP", "12345678"); // Nombre y contraseña del punto de acceso

  Serial.println("Punto de acceso iniciado.");

  

  // Iniciamos el servidor

  server.begin();

  configTime(timeZone * 3600, 0, ntpServer);

  Serial.println("Servidor web iniciado.");

}

  

void loop() {

  // Esperamos a que haya un cliente conectado

  WiFiClient client = server.available();

  

  if (client) {

    Serial.println("Cliente conectado.");

    String header = "";

    String currentLine = "";

  

    while (client.connected()) {

      if (client.available()) {

        char c = client.read();

        header += c;

  

        // Si se encuentra un salto de línea significa que la solicitud HTTP está completa

        if (c == '\n') {

          // Si la línea está vacía, significa que es el fin del request HTTP del cliente

          if (currentLine.length() == 0) {

            // Enviamos la respuesta HTTP

            client.println("HTTP/1.1 200 OK");

            client.println("Content-type:text/html");

            client.println("Connection: close");

            client.println();

  

            // Verificamos si hay una solicitud para conectarse

            if (header.indexOf("GET /connect?") >= 0) {

              // Extraemos los valores de SSID y password desde la cabecera

              int ssidIndex = header.indexOf("ssid=") + 5;

              int passwordIndex = header.indexOf("password=") + 9;

              ssidInput = header.substring(ssidIndex, header.indexOf("&", ssidIndex));

              passwordInput = header.substring(passwordIndex, header.indexOf(" ", passwordIndex));

  

              // Reemplazamos caracteres de espacio en blanco (+)

              ssidInput.replace("+", " ");

              passwordInput.replace("+", " ");

  

              // Intentamos conectar a la red Wi-Fi con los valores ingresados

              Serial.println("Conectando a la red Wi-Fi:");

              Serial.println("SSID: " + ssidInput);

              Serial.println("Password: " + passwordInput);

              WiFi.begin(ssidInput.c_str(), passwordInput.c_str());

  

              // Esperamos a que se conecte

              int attempts = 0;

              while (WiFi.status() != WL_CONNECTED && attempts < 20) {

                delay(500);

                Serial.print(".");

                attempts++;

              }

  

              if (WiFi.status() == WL_CONNECTED) {

                // Conexión exitosa

                Serial.println("\nConectado a " + ssidInput);

                Serial.println("IP: " + WiFi.localIP().toString());

                client.println("<h2>Conexión exitosa</h2>");

                client.println("<p>Conectado a " + ssidInput + "</p>");

                client.println("<p>IP: " + WiFi.localIP().toString() + "</p>");

  

                config.api_key = API_KEY;

                auth.user.email = userEmail;

                auth.user.password = userPassword;

                config.database_url = DATABASE_URL;

                config.token_status_callback = tokenStatusCallback;

  

                Firebase.reconnectNetwork(true);

                Firebase.begin(&config, &auth);

              } else {

                // Error en la conexión

                Serial.println("\nNo se pudo conectar.");

                client.println("<h2>Error en la conexión</h2>");

                client.println("<p>No se pudo conectar a " + ssidInput + ". Intente de nuevo.</p>");

              }

            } else {

              // Si no se envió una solicitud de conexión, mostramos la página de configuración

              client.println(pagina);

            }

  

            // Fin de la respuesta HTTP

            client.println();

            break;

          } else {

            currentLine = "";

          }

        } else if (c != '\r') {

          currentLine += c;

        }

      }

    }

  

    // Cerramos la conexión con el cliente

    client.stop();

    Serial.println("Cliente desconectado.");

  }

  calcularPorcentaje();

  Serial.println(porcentaje);

  // Enviamos datos a Firebase si está conectado

  if (WiFi.status() == WL_CONNECTED) {

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {

  

      sendDataPrevMillis = millis();

      Serial.println("-1");

      if(porcentaje != "-1"){

        FirebaseJson json;

        json.set("porcentaje", porcentaje);

        json.set("fecha", getCurrentDate());

  

        String moduleId = "-O9AOGhgVPLt464eEULY";

        String path = "/ModulesWifi/" + moduleId;

  

        Firebase.pushJSON(fbdo, path.c_str(), json);

      }

    }

  }

  delay(300);

}

void calcularPorcentaje(){

    if (digitalRead(23) == HIGH) {

        porcentaje = "100";

    }

    else if (digitalRead(22) == HIGH) {

        porcentaje = "85";

    }

    else if (digitalRead(21) == HIGH) {

        porcentaje = "71";

    }

    else if (digitalRead(19) == HIGH) {

        porcentaje = "57";

    }

    else if (digitalRead(18) == HIGH) {

        porcentaje = "42";

    }

    else if (digitalRead(17) == HIGH) {

        porcentaje = "28";

    }

    else if (digitalRead(16) == HIGH) {

        porcentaje = "14";

    }

    else if (digitalRead(15) == HIGH) {

        porcentaje = "0";

    }else{

       porcentaje = "-1";

    }

}

String getCurrentDate() {

  time_t now = time(nullptr);

  struct tm *timeInfo = localtime(&now);

  char fecha[11];

  strftime(fecha, sizeof(fecha), "%d/%m/%Y", timeInfo);

  return String(fecha);

}
```
