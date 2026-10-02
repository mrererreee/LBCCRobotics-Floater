#define RXD2 15
#define TXD2 16

HardwareSerial blueSerial(2);

void setup()
{
  Serial.begin(115200);
  delay(1000);

  blueSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Serial.println("ESP32-P4 HC-05 test");
}

void loop()
{
  if (Serial.available())
  {
    char c = Serial.read();
    blueSerial.write(c);
  }

  if (blueSerial.available())
  {
    char c = blueSerial.read();
    Serial.write(c);
  }
}