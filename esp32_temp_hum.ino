#include <DHT.h>

// Definir el pin donde está conectado el DHT11
#define DHTPIN 4

// Definir el tipo de sensor DHT
#define DHTTYPE DHT11

// Inicializar el sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando sensor DHT11...");
  dht.begin();
}

void loop() {
  // Esperar 2 segundos entre lecturas
  delay(2000);

  // Leer la humedad y la temperatura
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Verificar si alguna lectura ha fallado y mostrar un mensaje
  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println(F("Error al leer del sensor DHT!"));
    return;
  } 
  
  
    // Mostrar los resultados en el monitor serial
    Serial.print(F("Humedad: "));
    Serial.print(humedad);
    Serial.print(F("%  Temperatura: "));
    Serial.print(temperatura);
    Serial.println(F("°C"));
  
}