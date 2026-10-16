// BLUETOOTH STATE PIN TEST
#include <Wire.h>
#include "MS5837.h"

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
MS5837 sensor;

//int state = 0;

void setup() {

  Serial.begin(115200); // baud

  pinMode(STATE_PIN, INPUT);
  pinMode(STATE_LED, OUTPUT);
  Wire.begin();

  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  blueTooth.begin(9600, SERIAL_8N1, RXD1, TXD1);

  Serial.println("test");

}

void loop() {

  // reading raw data from photoresistor pin 
  voltage = analogRead(PHOTORESISTOR_PIN);
  // calculation of voltage coming from photoresistor pin
  float voltageMeasure = (voltage / ADC_STEPS) * V_REF;
  // Update pressure and temperature readings
  sensor.read();

  int state = digitalRead(STATE_PIN);

  // state pin reads 1, connected AND char is sent to bluetooth terminal
  if (state && blueTooth.available()) 
  {
    LED_blink_pattern();

    //photoresistor_check(voltage, threshold);

    // reads all information from depth sensor
    blueTooth.print("Pressure: ");
    blueTooth.print(sensor.pressure());
    blueTooth.print(" mbar");
   
    blueTooth.print(" Temperature: ");
    blueTooth.print(sensor.temperature());
    blueTooth.print(" deg C");
   
    blueTooth.print(" Depth: ");
    blueTooth.print(sensor.depth());
    blueTooth.print(" m");
 
    // prints voltage calculation
    blueTooth.print(" ");
    blueTooth.print(voltageMeasure);
    blueTooth.print(" V");
    blueTooth.println();

  }
  else // state reads 0, disconnected
  {
    digitalWrite(STATE_LED, state);
    Serial.println("Waiting for signal");
  }


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
