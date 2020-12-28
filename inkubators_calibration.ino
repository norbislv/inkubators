
#include <DHT.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

int PWM=0; //current pwm
uint8_t DHTPin = D4; // DHT Sensor
const int heaterPin = 16; //D0
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;
 
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  dht.begin();  
//  analogWrite(heaterPin, 512);  // heater          
 }

 
void loop() {
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  Serial.println(Temperature); 
  delay(3000);
}

bool calibration(){ 
// heats incubator in steps of 5% of pwm and checks when tempreature stops increasing. stores data on SPIFFS in calibration.txt file. checksum is added to file when calibration is complete.
// outputs serial data in format: "temperature, pwm" for plotting or just watching it go brrrrrr. returns true when finished.
  
}

bool checkSPIFF(){
//checks if version.txt is present, if not offers to format SPIFF. returns true if version.txt is present or format is complete.
  
}

bool checkCalibrtion(){
//checks if calibration.txt if present, if not offers to calibrate. returns true if calibrated.
}


int checkWatchDog() {
//checks if finished.txt contains time. if second line is "0" it means that cycle had finished. returns elpased time from the file or 0 if cycle had finished
  
}

bool updateWatchDog (int elpased) {
//stores elapsed time in finished.txt. returns ture on success
}


void startHeatCycle(int temperature, int timeToGo){
// starts heat cycle
// resets watchdog file.  
}

int checkTemperature (int targetTemperature, int previousTemperature){
  //checks if current temprature is at target. looks at temperature difference between previous temperature and adjusts PWM.  
}
