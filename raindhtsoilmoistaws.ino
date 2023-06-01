#include <DHT.h>

#define DHTPIN 2           // Pin connected to the DHT22 sensor
#define DHTTYPE DHT22      // DHT sensor type

#define MOISTURE_PIN A0    // Pin connected to the soil moisture sensor
#define RAINDROP_PIN A1    // Pin connected to the raindrop module

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();     // Read temperature from DHT22
  float humidity = dht.readHumidity();           // Read humidity from DHT22

  uint16_t moistureValue = analogRead(MOISTURE_PIN);   // Read moisture value from soil moisture sensor
  uint16_t raindropValue = analogRead(RAINDROP_PIN);   // Read raindrop value from raindrop module
//99&12&675&929&#
  // Print sensor data
 
  Serial.print(temperature);
  Serial.print("&");
  

  Serial.print(humidity);
  Serial.print("&");


  Serial.print(moistureValue);
  Serial.print("&");


  Serial.print(raindropValue);
  Serial.print("&");
  //Serial.print("*#");
  //Serial.println();

  delay(5000);  // Delay for 5 seconds before collecting data again
}
