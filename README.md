# Wi-Fi ESP - 8266 
Para configurar la placa se debe utilizar el siguiente código: 
```c++
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
SoftwareSerial espSerial(0, 1);

void setup() {
  Serial.begin(9600);     // Comunicación con el monitor serial
  espSerial.begin(9600);
  WiFi.begin("moto", "gatotactico"); 
  // Esperar hasta que esté conectado
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Una vez conectado, mostrar la dirección IP
  Serial.println();
  Serial.println("Conectado a la red Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  if (espSerial.available()) {
    while (espSerial.available()) {
      char c = espSerial.read();
      Serial.write(c);  
    }
    delay(500);
    // Verificar la respuesta
    String respuesta = "";
    while (espSerial.available()) {
      respuesta += char(espSerial.read());
    }
    if (respuesta.indexOf("No AP") != -1) {
      Serial.println("No se ha encontrado ninguna red Wi-Fi conectada.");
    } else if (respuesta.indexOf("WIFI CONNECTED") != -1 || respuesta.indexOf("+CWJAP:") != -1) {
      Serial.println("Conexión Wi-Fi establecida.");
    }
  }
}
```
