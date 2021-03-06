// Science Research Club
// 2016.12.02

// full documentation: https://github.com/kungpaogao/Lux-Meter

// NOTE: needs modified Print.h for printf()
// http://playground.arduino.cc/Main/Printf
#include <Wire.h> // communicate with I2C devices (light sensor SDA, SCL)
#include <BH1750.h> // the light sensor code
#include <SD.h> // access SD card
#include <SPI.h> // for SPI devices (SD)
#include <RTClib.h> // the clock on logging shield for getting time of data

#define LOG_INTERVAL 1000 // milliseconds between logs
#define SYNC_INTERVAL 3000 // milliseconds between writings
uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL 1 // echo data to serial port (USB)
#define WAIT_TO_START 0 // wait for serial input in setup()

BH1750 lightMeter; // define BH1750 object
RTC_DS1307 RTC; // define Real Time Clock (RTC) object

const int chipSelect = 10; // define data logging shield digital pin (10 in our case)

File logfile; // logging file

// error function for printing errors in console
void error(char *str) {
  Serial.print("error: ");
  Serial.println(str);
  while(1);
}

void setup(void) {
  Serial.begin(9600);
  Serial.println("Starting...");

  #if WAIT_TO_START // only runs if WAIT_TO_START is set to 1 (true)
    Serial.println("Type character to start");
    while (!Serial.available());
  #endif 

  Serial.print("Initializing SD card..."); // start SD card
  pinMode(10, OUTPUT); // set pin again ??
  if(!SD.begin(chipSelect)) {
    error("Card failed, or not present!"); // error message if SD card not working
  }
  Serial.println("card initialized.");

  // create logging file
  char filename[] = "LOG000.CSV";
  for(uint16_t i = 0; i < 1000; i++) {
    filename[3] = i/100 + '0'; 
    filename[4] = i/10 + '0';
    filename[5] = i%10 + '0';
    if(!SD.exists(filename)) {
      logfile = SD.open(filename, FILE_WRITE); // make new if none exists
      break;
    }
  }
  if(!logfile) {
    error("Couldn't create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);

  // connect to RTC
  Wire.begin();
  if(!RTC.begin()) {
    logfile.println("RTC failed"); // shows error in logfile
    #if ECHO_TO_SERIAL
      Serial.println("RTC failed"); // also shows error in console if connected to USB
    #endif
  }

  logfile.println("millis,stamp,datetime,light");
  #if ECHO_TO_SERIAL
    Serial.println("millis,stamp,datetime,light");
  #endif

  pinMode(LED_BUILTIN, OUTPUT); // set LED to be an output
  lightMeter.begin(); // start light sensor
}

void loop(void) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW); //flash LED to indicate code is running
  
  DateTime now; 

  // delay for the amount of time between readings
  delay((LOG_INTERVAL - 1) - (millis() % LOG_INTERVAL));

  // log milliseconds since starting
  uint32_t m = millis();
  logfile.print(m);          
  logfile.print(", ");    
  #if ECHO_TO_SERIAL
    Serial.print(m);   
    Serial.print(", ");  
  #endif

  now = RTC.now(); // fetch time
  // log time  
  logfile.print(now.unixtime()); // seconds since 1/1/1970
  logfile.print(", ");
  logfile.printf("%02u/%02u/%4u %02u:%02u:%02u", now.month(), now.day(), now.year(),
    now.hour(), now.minute(), now.second());
  #if ECHO_TO_SERIAL
    Serial.print(now.unixtime()); // seconds since 1/1/1970
    Serial.print(", ");
    Serial.printf("%02u/%02u/%4u %02u:%02u:%02u", now.month(), now.day(), now.year(),
      now.hour(), now.minute(), now.second());
  #endif

  lightMeter.readLightLevel();
  delay(10);
  int lux = lightMeter.readLightLevel();

  logfile.print(", ");
  logfile.print(lux);
  #if ECHO_TO_SERIAL
    Serial.print(", ");
    Serial.print(lux);
  #endif

  logfile.println();
  #if ECHO_TO_SERIAL
    Serial.println();
  #endif

  // write data to disk
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();

  logfile.flush();
} 
