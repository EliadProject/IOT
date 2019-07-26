

#include <OneWire.h>//for temperature sensor


#define DEVICE  "18fe349d-dfa"  
#define POWERBUTTON "powerbutton"  
#define TEMPERATUREWANTED "temperaturewanted"
#define STARTTIME "start-time-1"
#define ENDTIME "endtime1"
#define TOKEN  "A1E-ocxptRXO0T4YcvClY8n2LaOkF9XkP6" 
//#define WIFISSID "AndroidAP" 
//#define PASSWORD "-0p-0p-0p"
#define WIFISSID "ADIN" 
#define PASSWORD "0523247841"

#define RELAY  0
OneWire  ds(2);   
float power=0;
float temperatureWanted=0;
int boilerOn=0;
float startTime=0;
float endTime=0;
float currentTime=0;
int syncTimeCount=0;
float temperatureFromSensor;



void setup(void) {
  Serial.begin(9600);
  delay(10);
  pinMode(RELAY, OUTPUT);
   digitalWrite(RELAY,LOW);
}

void loop(void) {
  
  getTemperatureFromSensor(); 
   
}
void getTemperatureFromSensor(){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44);        
  delay(1000);       
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         
  for ( i = 0; i < 9; i++) {           
    data[i] = ds.read();
  }
 OneWire::crc8(data, 8);

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; 
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  
    else if (cfg == 0x20) raw = raw & ~3; 
    else if (cfg == 0x40) raw = raw & ~1; 
  }
  temperatureFromSensor = (float)raw / 16.0;
  Serial.println(temperatureFromSensor);
}
