
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL3HL8CZ2I"
#define BLYNK_DEVICE_NAME "BME688Monitor"

#define BLYNK_FIRMWARE_VERSION        "0.1.3"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#include "bsec.h"
#include <XNODE.h>
Adafruit_BME680 bme;
// Create an object of the class Bsec
Bsec iaqSensor;
// Create an object of the class XNODE
XNODE xnode(&Serial2);
// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);

#define LED_BUILTIN 12
#define APP_DEBUG


#include "BlynkEdgent.h"
int period = 1000;
unsigned long time_now = 0;
String output;
union Data {
  uint16_t now;
} data;
void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17); //using RX2 and TX2 from ESP
  delay(100);
  BlynkEdgent.begin();
  delay(1000);

  Serial2.write("AT+IPR=115200\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CFUN=0 \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CGDCONT=1,\"IP\",\"internet.itelcel.com\" \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CFUN=1 \r"); //instead of a sending message, a variable must be sent
  delay(5000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CFUN=1,1 \r"); //instead of a sending message, a variable must be sent
  delay(10000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CPIN? \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CSQ \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CGATT?\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+COPS?\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CGNAPN\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CNACT=1\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+CNACT?\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //________________blynk HTTPS_____________________________________________________________________________
  Serial2.write("AT+CSSLCFG=\"sslversion\",1,3 \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+SHSSL=1,\"\"\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+SHCONF=\"URL\",\"https://blynk.cloud:443\"\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+SHCONF=\"BODYLEN\",1024 \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+SHCONF=\"HEADERLEN\",350   \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+SHCONN \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+SHSTATE? \r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  //_____________________________________________________________________________________________
  Serial2.write("AT+SHCHEAD\r"); //instead of a sending message, a variable must be sent
  delay(2000);
  while (Serial2.available()) {
    int g = Serial2.read();
    Serial.write(g);
  }
  Wire.begin();
  delay(5000);
  iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();
  bme.begin();
  bme.setGasHeater(320, 150);


}

void loop() {
  BlynkEdgent.run();

  if ((unsigned long)(millis() - time_now) > period) {
    time_now = millis();
    readBME688();
  }
}

void readBME688()
{
  if (iaqSensor.run() && bme.performReading()) {
    //    Blynk.virtualWrite(V0,String(iaqSensor.rawTemperature));
    //    Blynk.virtualWrite(V1,String(iaqSensor.pressure));
    //    Blynk.virtualWrite(V2,String(iaqSensor.rawHumidity));
    //    Blynk.virtualWrite(V3,String(bme.gas_resistance));
    //    Blynk.virtualWrite(V4,String(iaqSensor.iaq));
    //    Blynk.virtualWrite(V5,String(iaqSensor.iaqAccuracy));
    //    Blynk.virtualWrite(V6,String(iaqSensor.co2Equivalent));
    //    Blynk.virtualWrite(V7,String(iaqSensor.breathVocEquivalent));
  float IAQ = 52;
  if ( IAQ >= 0 && IAQ <= 50)
  {
    xnode.SendCommandWithRange("007A", "1", 0, 255, 0);
  }
  if ( IAQ >= 50.1 && IAQ <= 100)
  {
    xnode.SendCommandWithRange("007A", "1", 255, 255, 0);
  }
  if ( IAQ >= 100.1 && IAQ <= 150)
  {
    xnode.SendCommandWithRange("007A", "1", 255, 128, 0);
  }

  if ( IAQ >= 150.1 && IAQ <= 200)
  {
    xnode.SendCommandWithRange("007A", "1", 255, 0, 0);
  }

  if ( IAQ >= 200.1 && IAQ <= 300)
  {
    xnode.SendCommandWithRange("007A", "1", 87, 35, 100);
  }

  if ( IAQ >= 300.1 && IAQ <= 500)
  {
    xnode.SendCommandWithRange("007A", "1", 0, 0, 0);
  }
    char buffer[20];
    int i = iaqSensor.rawTemperature;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v0=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }

    i = iaqSensor.pressure;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v1=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = iaqSensor.rawHumidity;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v2=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = bme.gas_resistance;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v3=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = iaqSensor.iaq;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v4=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = iaqSensor.iaqAccuracy;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v5=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = iaqSensor.co2Equivalent;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v6=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = iaqSensor.breathVocEquivalent;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v7=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = iaqSensor.temperature;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v8=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    i = iaqSensor.humidity;
    sprintf(buffer, "%i", i);
    Serial2.write("AT+SHREQ=\"/external/api/update?token=h_e4Kk_78q08ObTbY-Dj6FOyor56eJf3&v9=");
    Serial2.write(buffer);
    Serial2.write("\",1\r");
    delay(2000);
    while (Serial2.available()) {
      int g = Serial2.read();
      Serial.write(g);
    }
    delay(60000);
  }
}

// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }

  }
}

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
