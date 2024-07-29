#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Configuración de la red WiFi
const char* ssid = "Aliss";
const char* password = "lillium28";

// Crear un objeto de servidor web
WebServer server(80);

// Crear un objeto para controlar el servo
Servo myServo;

// Pin al que está conectado el servo
const int servoPin = 18;

// Variable para almacenar la posición actual del servo
int pos = 0;

void setup() {
  // Iniciar el puerto serial para depuración
  Serial.begin(115200);
  
  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");

  // Iniciar el servidor web
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.begin();
  Serial.println("Servidor web iniciado");

  // Asignar el pin al servo
  myServo.attach(servoPin);
  myServo.write(pos);
}

void loop() {
  // Manejar las solicitudes del servidor web
  server.handleClient();
}

void handleRoot() {
  // Página web para controlar el servo
  String html = "<html><body>";
  html += "<h1>Control del Servo</h1>";
  html += "<form action=\"/move\" method=\"POST\">";
  html += "Posición (0-180): <input type=\"number\" name=\"pos\" min=\"0\" max=\"180\" value=\"" + String(pos) + "\">";
  html += "<input type=\"submit\" value=\"Mover\">";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleMove() {
  if (server.method() == HTTP_POST) {
    if (server.hasArg("pos")) {
      pos = server.arg("pos").toInt();
      if (pos < 0) pos = 0;
      if (pos > 180) pos = 180;

      for (int currentPos = myServo.read(); currentPos != pos;) {
        if (currentPos < pos) {
          currentPos++;
        } else {
          currentPos--;
        }
        myServo.write(currentPos);
        delay(1000); // Esperar 1 segundo entre cada paso
      }

      String response = "<html><body>";
      response += "<h1>Servo Movido a " + String(pos) + " Grados</h1>";
      response += "<a href=\"/\">Volver</a>";
      response += "</body></html>";

      server.send(200, "text/html", response);
    } else {
      server.send(400, "text/html", "Bad Request");
    }
  } else {
    server.send(405, "text/html", "Method Not Allowed");
  }
}