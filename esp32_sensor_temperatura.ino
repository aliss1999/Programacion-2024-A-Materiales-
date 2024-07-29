#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// Definiciones del DHT11
#define DHTPIN 4      // Pin al que está conectado el DHT11
#define DHTTYPE DHT11 // Tipo de sensor (DHT11)

// Definiciones de Wi-Fi
const char* ssid = "CAMPUS_EPN";
const char* password = "politecnica**";

// Definición del servidor
const char* serverName = "http://alissvaca.com/temperatura";

// Inicialización del DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Conexión a Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a Wi-Fi.");

  // Inicialización del DHT11
  dht.begin();
}

void loop() {
  // Espera a que el Wi-Fi esté conectado
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    float temp = dht.readTemperature();

    // Comprobar si la lectura es correcta
    if (isnan(temp)) {
      Serial.println("Fallo al leer del DHT11.");
    } else {
      Serial.print("Temperatura: ");
      Serial.print(temp);
      Serial.println(" °C");
      
      // Iniciar conexión al servidor
      http.begin(serverName);

      // Añadir los datos de la temperatura como parámetros de la URL
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "temperature=" + String(temp);

      // Enviar la solicitud HTTP POST
      int httpResponseCode = http.POST(httpRequestData);
      
      // Verificar la respuesta del servidor
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode); // Código de respuesta
        Serial.println(response); // Respuesta del servidor
      } else {
        Serial.print("Error en la solicitud HTTP: ");
        Serial.println(httpResponseCode);
      }
      
      // Finalizar la conexión
      http.end();
    }
  } else {
    Serial.println("Desconectado del Wi-Fi.");
  }
  
  // Esperar 10 segundos antes de la siguiente lectura
  delay(10000);
}
