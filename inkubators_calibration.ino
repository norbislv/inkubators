#include <DHT_U.h>
#include <DHT.h>



#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
uint8_t DHTPin = D4; 
const int ledPin = 16; //D0
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  dht.begin();  
  analogWrite(ledPin, 512);            
 }

 
void loop() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  Serial.println(Temperature); 
  delay(3000);
}
