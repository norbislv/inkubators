
#include <DHT.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

//global variables

uint8_t DHTPin = D4; // DHT Sensor
const int heaterPin = 16; //D0
int operation=0; //what operation controller is currently doing
int cycleStartTime=0; //time when cycle was started
int PWM=0; //output pwm value 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);  

// function prototypes to make compiler happy
void readSensors();
bool checkSPIFF();
bool checkSetup();
bool checkCalibration();
int checkWatchDog();
bool formatSPIFF();
bool doSetup();
bool calibration();
bool updateWatchDog (int elpased);
void startHeatCycle(int temperature, int timeToGo);
int checkTemperature (int targetTemperature, int previousTemperature);

 
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  dht.begin();        
 }
 
void loop() {
  if (!checkSPIFF()){
   Serial.println("SPIFF must be formated. Stand by");
  formatSPIFF();
  }
  if (!checkSetup()){
    Serial.println("Setup must be done");
    doSetup();
  }
  if (!checkCalibration()){
   Serial.println("Calibration should be done. This will take long time. proceed? [Y/n]");
    int incomingByte= 0;
    if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (char(incomingByte)=="Y" || char(incomingByte)=="y") {
      while (!calibration){
    }
    }
    else {
      Serial.println("ok, will just read sensors for now");
      readSensors();
      
    }
   }}
  while (1){// main cycle
   delay(3000);
   checkTemperature(30,30);
  }
}

void readSensors(){
//just prints output from sensor
 float temperature;
 float humidity;
 while(1){
 temperature = dht.readTemperature(); // Gets the values of the temperature
 humidity = dht.readHumidity(); // Gets the values of the humidity 
 Serial.print (temperature); 
 Serial.print (", ");
 Serial.print (humidity);
 }
}
bool checkSPIFF(){
//checks if version.txt is present.
  
}

bool checkSetup(){
  //checks if setup.txt is present.
}
bool checkCalibration(){
//checks if calibration.txt if present
}


int checkWatchDog() {
//checks if finished.txt contains time. if second line is "0" it means that cycle had finished. returns elpased time from the file or 0 if cycle had finished
  
}

bool formatSPIFF(){
//formats SPIFF and places version.txt
}

bool doSetup(){
  //makes setup.txt file
}

bool calibration(){ 
// heats incubator in steps of 5% of pwm and checks when tempreature stops increasing. stores data on SPIFFS in calibration.txt file. checksum is added to file when calibration is complete.
// outputs serial data in format: "temperature, pwm" for plotting or just watching it go brrrrrr. returns true when finished.

}


bool updateWatchDog (int elpased) {
//stores elapsed time in finished.txt. returns ture on success
}


void startHeatCycle(int temperature, int timeToGo){
// starts heat cycle
// resets watchdog file.  
}

int checkTemperature (int targetTemperature, int previousTemperature){
//checks if current temprature is at target. looks at temperature difference between previous temperature and adjusts PWM. returns current temperature
 float temperature;
 float humidity;
 temperature = dht.readTemperature(); // Gets the values of the temperature
 humidity = dht.readHumidity(); // Gets the values of the humidity 
 Serial.println(temperature); 
 //  analogWrite(heaterPin, 512);  // heater 
 return temperature;
}
