const float V_REF = 3.3;
const float R_BITS = 12.0;
const float ADC_STEPS = (1 << int(R_BITS)) - 1;
 
// both can be reassigned to different pins as long as it doesn't disrupt other functionalities of the esp
const int LED_PIN = 32;
const int PHOTORESISTOR_PIN = 16;
int voltage;
const int threshold = 2000; // changing the threshold affects the sensitivity of the light. the higher, the less it flickers
 
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println(ADC_STEPS);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  // notes
  /*
    - analogRead returns raw integer between 0-4095 (0 - 3.3V)
    - for actual voltage, use V = raw * 3.3 / 4095
    - for base calculations with a voltage flow of 5V
  */
  voltage = analogRead(PHOTORESISTOR_PIN);
 
  float voltageMeasure = (rawValue / ADC_STEPS) *V_REF;
  Serial.print(voltageMeasure);
  Serial.println(" V");
 
  if (voltage < threshold)
  {
    digitalWrite(LED_PIN, HIGH);
 
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}