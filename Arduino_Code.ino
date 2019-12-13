#include <Wire.h>
#include <SPI.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME680.h"
#include <Adafruit_TSL2561_U.h>


#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SWITCH_PIN 3
#define UV_SENSOR_PIN 4

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

//Formatted Data
String data; 

//Pin of switch
//int switchPin = 3;

//Status of Switch
int switchState = LOW;

void setup() 
{
  Serial.begin(9600);
  //while (!Serial);
  //Serial.println(F("BME680 test"));

  //Initializes the button
  pinMode(SWITCH_PIN, INPUT);

  

  if(!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  if (!bme.begin()) 
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}
 
void loop() 
{
  switchState = digitalRead(SWITCH_PIN);

  if (switchState == HIGH)
  {
    //Serial.println("Data");

    
    if (! bme.performReading()) 
    {
      Serial.println("Failed to perform reading :(");
    }
    //Serial.print("Temperature = ");
    //Serial.print(bme.temperature + ",");
    //Serial.println(" *C");
  
    data += bme.temperature;
    data += ",";
  
    //Serial.print("Pressure = ");
    //Serial.print(bme.pressure / 100.0);
    //Serial.println(" hPa");
  
    data += bme.pressure;
    data += ",";
  
    //Serial.print("Humidity = ");
    //Serial.print(bme.humidity);
    //Serial.println(" %");
  
    data += bme.humidity;
    data += ",";
  
    //Serial.print("Altitude = ");
    //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");
  
    data += bme.readAltitude(SEALEVELPRESSURE_HPA);
    data += ",";
  
    sensors_event_t event;
    tsl.getEvent(&event);
   
    /* Display the results (light is measured in lux) */
    if (event.light)
    {
      //Serial.print("Lux: ");
      //Serial.print(event.light); 
      //Serial.println(" lux");
  
      data += event.light;
    }
    else
    {
      /* If event.light = 0 lux the sensor is probably saturated
         and no reliable data could be generated! */
      Serial.println("Sensor overload");
    }
  
    //Serial.println();
    Serial.println(data);
    //Serial.println();
    delay(2000);
  
    data = "";
  }
  else if (switchState == LOW)
  {
    Serial.println("Sensors currently Deactivated.");
    delay(2000);
  }
}
