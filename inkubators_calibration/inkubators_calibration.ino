#include "FS.h"
#include <DHT.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

//global variables
const uint8_t vers= 0; //version number
uint8_t DHTPin = D4; // DHT Sensor
const uint8_t heaterPin = 16; //D0
uint8_t operation=0; //what operation controller is currently doing. 0 - idle, 1 - heating cycle, 2 - calibration
int cycleStartTime=0; //time when cycle was started
int PWM=0; //output pwm value 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);  

// function prototypes to make compiler happy
void readSensors();
bool checkSPIFFS();
bool checkSetup();
bool checkCalibration();
int checkWatchDog();
bool formatSPIFFS();
bool doSetup();
bool calibration();
bool updateWatchDog (int elpased);
void startHeatCycle(int temperature, int timeToGo);
int checkTemperature (int targetTemperature, int previousTemperature);
void initController();

 
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  dht.begin();  
  SPIFFS.begin();  
  Serial.println("");
  initController();    //check if necesary config is present 
 }
 
void loop() {
  delay(3000);
  readSensors();
}

void readSensors(){
//just prints output from sensor
  float temperature;
  float humidity;

  temperature = dht.readTemperature(); // Gets the values of the temperature
  humidity = dht.readHumidity(); // Gets the values of the humidity 
  Serial.print (temperature); 
  Serial.print (", ");
  Serial.print (humidity);
  Serial.println("");
}
 
bool checkSPIFFS(){
//checks if version.txt is present.
  File f = SPIFFS.open("/version.txt", "r");
  if (!f) return false;
  return true; 
  
}

bool checkSetup(){
//checks if setup.txt is present.
  File f = SPIFFS.open("/setup.txt", "r");
  if (!f)return false;
  f.close();
  return true;
}

bool checkCalibration(){
//checks if calibration.txt if present
  File f = SPIFFS.open("/calibration.txt", "r");
  if (!f)return false;
  f.close();
  return true;
}


int checkWatchDog() {
//checks if finished.txt contains time. if second line is "0" it means that cycle had finished. returns elpased time from the file or 0 if cycle had finished
}

bool formatSPIFFS(){
//formats SPIFFS and places version.txt
  SPIFFS.format();
  File f = SPIFFS.open("/version.txt", "w");
  if (!f){
    Serial.println("file open failed");
    return false;
  }
  f.println("0");
  Serial.println("done");
  f.close();
  return true;
}

bool doSetup(){
//makes setup.txt file
  File f = SPIFFS.open("/setup.txt", "w");
  if (!f){
    Serial.println("file open failed");
    return false;
  }
  f.println("40"); //maximum temperature. ask user for this in future
  f.println("256"); //starting pwm. one quarter of maximum
  Serial.println("done");
  f.close();
  return true;
}

bool calibration(){ 
// heats incubator in steps of 5% of pwm and checks when tempreature stops increasing. stores data on SPIFFSS in calibration.txt file. checksum is added to file when calibration is complete.
// outputs serial data in format: "temperature, pwm" for plotting or just watching it go brrrrrr. returns true when finished.
  File f = SPIFFS.open("/setup.txt", "r");
  if (!f) {
    Serial.println("file open failed");
    return false;
  }
   operation = 2;
   int pwmStep=102;//will increase in steps of ~10%
   int temperature = 0; 
   int steadyCounter=0;
   
   String s=f.readStringUntil('\n'); 
   int maxTemp = s.toInt();
   s=f.readStringUntil('\n');
   PWM = s.toInt();//starting PWM
   f.close();
   f = SPIFFS.open("/calibration.txt", "w");
   if (!f) {
     Serial.println("file open failed");
     return false;
   }
   
  
   int oldTemp = dht.readTemperature(); //we start from current temperature, not 0
   analogWrite(heaterPin, PWM);  // start the heater 
   
   while (temperature<maxTemp){//run until we reaxh max temp. 
     delay(3000); //sensor needs about 2 seconds between reads
     temperature = dht.readTemperature();
     Serial.print (temperature); 
     Serial.print (", ");
     Serial.print (PWM);
     if ((temperature-oldTemp)<1) steadyCounter++;
     if (steadyCounter>20){//temperature hasn't changed much for one minute
      if (PWM==2023) { //calibration complete
        f.print(PWM);
        f.print(" ");
        f.print(temperature);
        PWM=0;
        analogWrite(heaterPin, PWM);
        break;
        
      }
      if (PWM<(1023-pwmStep)){
        f.print(PWM);
        f.print(" ");
        f.print(temperature);
        PWM+=pwmStep;
        analogWrite(heaterPin, PWM);

      }
      else{
        f.print(PWM);
        f.print(" ");
        f.print(temperature);
        PWM=2023;
        analogWrite(heaterPin, PWM);
        
      }
     }
   }
}


bool updateWatchDog (int elpased) {
//stores elapsed time in finished.txt. returns ture on success
}


void startHeatCycle(int temperature, int timeToGo){
// starts heat cycle
// resets watchdog file.  
}

void initController(){
  if (!checkSPIFFS()){
    Serial.println("SPIFFS must be formated. Stand by");
    formatSPIFFS();
  }
  if (!checkSetup()){
    Serial.println("Setup must be done");
    doSetup();
  }
  if (!checkCalibration()){
    Serial.println("Calibration should be done. This will take long time. proceed? [Y/n]");
    int incomingByte= 0;
    while (Serial.available() == 0) {//wait for user input
  }
    incomingByte = Serial.read();
    if (char(incomingByte)=='Y' || char(incomingByte)=='y') {
    calibration();
    }
  else {
    Serial.println("ok, will just read sensors in main loop for now");
   }
  }   
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
