// library for NEOPIXEL RING
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// LIBRARIES FOR MS5837 DEPTH SENSOR
#include <Wire.h>
#include "MS5837.h"
 
// PIN DECLARATIONS
#define NEOPIXEL_LED 27
#define LED_COUNT 24 // number of LEDs on NeoPixel
const int LED_PIN = 23; // for photoresistor testing
const int PHOTORESISTOR_PIN = 22;
// for HC05 Bluetooth module
#define RXD2 15
#define TXD2 16
// linear actuator
#define ACTUATOR_IN1 48
#define ACTUATOR_IN2 47
 
// objects
Adafruit_NeoPixel strip(LED_COUNT, NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);
HardwareSerial blueSerial(2);
MS5837 sensor;
 
// constants for photoresistor calculations to convert to voltage measurements 
const float V_REF = 3.3;
const float R_BITS = 12.0;
const float ADC_STEPS = (1 << int(R_BITS)) - 1;
 
int voltage;
const int threshold = 2000;
 
 
 
 
void setup() {

// setting baud rate to read info from HC05
  Serial.begin(115200);
  Serial.println(ADC_STEPS);
  Serial.println("Starting");
  Wire.begin();
 
 
  // Initialize pressure sensor
  // Returns true if initialization was successful
  // We can't continue with the rest of the program unless we can initialize the sensor
  // will remove later
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
 
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255) ADJUSTABLE
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  blueSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ACTUATOR_IN1, OUTPUT);
  pinMode(ACTUATOR_IN2, OUTPUT);
 
}
 
void loop() {

  // reading raw data from photoresistor pin 
  voltage = analogRead(PHOTORESISTOR_PIN);
  // calculation of voltage coming from photoresistor pin
  float voltageMeasure = (voltage / ADC_STEPS) * V_REF;
  // Update pressure and temperature readings
  sensor.read();

  // if bluetooth terminal speaks to source computer, it will show on arduino ide terminal
  if (Serial.available())
  {
    char c = Serial.read();
    blueSerial.write(c);
  }
 
  // if bluetooth module is detected, all sensor readings will be passed through to display
  // via bluetooth terminal (on PuTTy of secondary computer receiving info)
  else if (blueSerial.available())
  {
    //char c = blueSerial.read(); left for testing
    //Serial.write(c);

    // makes linear actuator move
    digitalWrite(ACTUATOR_IN1, HIGH);
    digitalWrite(ACTUATOR_IN2, LOW);

    // runs photoresistor LED check
    photoresistor_check(voltage, threshold);

    // lights up a pattern
    colorWipe(strip.Color(255,   0,   0), 50);

    // reads all information from depth sensor
    blueSerial.print("Pressure: ");
    blueSerial.print(sensor.pressure());
    blueSerial.print(" mbar");
   
    blueSerial.print(" Temperature: ");
    blueSerial.print(sensor.temperature());
    blueSerial.print(" deg C");
   
    blueSerial.print(" Depth: ");
    blueSerial.print(sensor.depth());
    blueSerial.print(" m");
   
    /*blueSerial.print("Altitude: ");
    blueSerial.print(sensor.altitude());
    blueSerial.println(" m above mean sea level");*/
 
    // prints voltage calculation
    blueSerial.print(" ");
    blueSerial.print(voltageMeasure);
    blueSerial.print(" V");
    blueSerial.println();
  }
 
  // clears NeoPixel ring of pattern
  colorWipe(strip.Color(0,   0,   0), 50);
 
  
  delay(5000);
}
 
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// affects light of LED as photoresistor is being tested between flares of light
void photoresistor_check(int voltage, int bound)
{
  if (voltage < bound)
    digitalWrite(LED_PIN, HIGH);
  digitalWrite(LED_PIN, LOW);
}
