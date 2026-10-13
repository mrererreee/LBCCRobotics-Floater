#include <HardwareSerial.h>

#define RXD2 15
#define TXD2 16
#define STATE_PIN 17

HardwareSerial blueSerial(2); //OBJECT CREATED AND SET TO UART2 | UART2 IS SAME AS UART1 WITH DEFAULT PINS 15 & 16.
char inputs [100];

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(STATE_PIN, INPUT);

  blueSerial.begin(9600,   //BAUD RATE
              SERIAL_8N1,  //8 BIT, NO PARITY, 1 STOP BIT.
                    RXD2,  //PIN RECEIVING DATA.
                    TXD2); //PIN TRANSMITING DATA.

  Serial.println("ESP32-P4 HC-05 test");
}

void loop()
{
  if (digitalRead(STATE_PIN) && Serial.available())
  {
    char c = Serial.read();
    blueSerial.write(c);
  }

  if (digitalRead(STATE_PIN) && blueSerial.available())
  {
    char c = blueSerial.read();
    Serial.write(c);
  }
}