// BLUETOOTH STATE PIN TEST
#include <Wire.h>
#include "MS5837.h"
#include <Preferences.h>

#define RXD1 15
#define TXD1 16
#define STATE_PIN 17
#define STATE_LED 47
const int PHOTORESISTOR_PIN = 22;

// constants for photoresistor calculations to convert to voltage measurements 
const float V_REF = 3.3;
const float R_BITS = 12.0;
const float ADC_STEPS = (1 << int(R_BITS)) - 1;

int voltage;
const int threshold = 2000;

HardwareSerial blueTooth(2); // object set to uart2
Preferences prefs; // for data storage
MS5837 sensor;

//int state = 0;

void setup() {

  Serial.begin(115200); // baud

  // start instance of preferences
  prefs.begin("pin-test", false);

  pinMode(STATE_PIN, INPUT);
  pinMode(STATE_LED, OUTPUT);
  Wire.begin();

  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  blueTooth.begin(9600, SERIAL_8N1, RXD1, TXD1);

  Serial.println("=====ESP RESTART=====");

  uint32_t boots = prefs.getUInt("boots", 0);
  float_t pressure = prefs.getFloat("pressure", 0);
  float_t temperature = prefs.getFloat("temperature", 0);
  float_t depth = prefs.getFloat("depth", 0);
  float_t light = prefs.getFloat("light", 0);
  

  boots++;
  prefs.putUInt("boots", boots);

  Serial.println("\n=== STATE PIN TEST ===");
  Serial.printf("Boots so far: %lu\n", (unsigned long)boots);
  Serial.printf("Last saved pressure reading: %f\n", pressure);
  Serial.printf("Last saved temperature reading: %f\n", temperature);
  Serial.printf("Last saved depth reading: %f\n", depth);
  Serial.printf("Last saved light reading: %f\n", light);

  delay(3000);

  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  
}

void loop() {

  // Update pressure and temperature readings
  sensor.read();
  // reading raw data from photoresistor pin 
  voltage = analogRead(PHOTORESISTOR_PIN);
  // calculation of voltage coming from photoresistor pin
  float voltageMeasure = (voltage / ADC_STEPS) * V_REF;

  int state = digitalRead(STATE_PIN);

  static uint32_t t0 = 0; // timer

  // state pin reads 1, connected AND char is sent to bluetooth terminal
  /*if (state && blueTooth.available()) 
  {
    LED_blink_pattern();*/

    // variable instantiation (will be saved to flash memory)
    float pressure = sensor.pressure();
    float temperature = sensor.temperature();
    float depthMBAR = sensor.depth();
    // reads all information from depth sensor
    blueTooth.print("Pressure: ");
    blueTooth.print(pressure);
    blueTooth.print(" mbar");
   
    blueTooth.print(" Temperature: ");
    blueTooth.print(temperature);
    blueTooth.print(" deg C");
   
    blueTooth.print(" Depth: ");
    blueTooth.print(depthMBAR);
    blueTooth.print(" m");
 
    // prints voltage calculation
    blueTooth.print(" ");
    blueTooth.print(voltageMeasure);
    blueTooth.print(" V");
    blueTooth.println();

    // update and save every second
    if (millis() - t0 > 1000)
    {
      t0 = millis();
      prefs.putFloat("pressure", pressure);
      prefs.putFloat("temperature", temperature);
      prefs.putFloat("depth", depthMBAR);
      prefs.putFloat("light", voltageMeasure);

      Serial.printf("Saved samples = %f %f %f %f\n", pressure, temperature, depthMBAR, voltageMeasure);
    }

  //}
  /*else // state reads 0, disconnected
  {
    digitalWrite(STATE_LED, state);
    Serial.println("Waiting for signal");
    delay(3000);
  }*/


}

void LED_blink_pattern()
{
  digitalWrite(STATE_LED, HIGH);
  delay(1000);
  digitalWrite(STATE_LED, LOW);
  delay(500);
  digitalWrite(STATE_LED, HIGH);
  delay(500);
  digitalWrite(STATE_LED, HIGH);
  delay(1000);
  digitalWrite(STATE_LED, LOW);
}