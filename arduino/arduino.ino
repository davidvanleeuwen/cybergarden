#include <TheThingsNetwork.h>
#include <TheThingsMessage.h>
#include "./config.h"
#include <CayenneLPP.h>

// Sensor includes
#include <SHT1x.h>
#include <BH1750.h>
#include <Wire.h>

#define dataPin 10
#define clockPin 13
#define loraSerial Serial1
#define debugSerial Serial

SHT1x sht1x(dataPin, clockPin);
BH1750 LightSensor(0x23);

TheThingsNetwork ttn(loraSerial, debugSerial, TTN_FP_EU868);
CayenneLPP lpp(51);

uint16_t temperature;
uint16_t humidity;
uint16_t lux;

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.personalize(devAddr, nwkSKey, appSKey);

  Serial.begin(115200);
  Serial.begin(9600);

  Wire.begin();
  LightSensor.begin(BH1750::CONTINUOUS_LOW_RES_MODE);
}

void loop()
{
  debugSerial.println("-- LOOP");

  temperature = sht1x.readTemperatureC();
  humidity = sht1x.readHumidity();
  lux = LightSensor.readLightLevel();

  lpp.reset();
  lpp.addTemperature(1, temperature);
  lpp.addRelativeHumidity(2, humidity);
  lpp.addLuminosity(3, lux);
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());

  delay(60000);
}
