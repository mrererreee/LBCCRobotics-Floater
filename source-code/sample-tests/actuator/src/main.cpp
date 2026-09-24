#include <Arduino.h>

const int ACTUATOR_IN1 = 32;
const int ACTUATOR_IN2 = 33;

void stopActuator()
{
    digitalWrite(ACTUATOR_IN1, LOW);
    digitalWrite(ACTUATOR_IN2, LOW);
    Serial.println("[actuator] stop");
}

void extendActuator()
{
    digitalWrite(ACTUATOR_IN1, HIGH);
    digitalWrite(ACTUATOR_IN2, LOW);
    Serial.println("[actuator] extend");
}

void retractActuator()
{
    digitalWrite(ACTUATOR_IN1, LOW);
    digitalWrite(ACTUATOR_IN2, HIGH);
    Serial.println("[actuator] retract");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("[setup] floater firmware starting");

    pinMode(ACTUATOR_IN1, OUTPUT);
    pinMode(ACTUATOR_IN2, OUTPUT);
    Serial.printf("[setup] pins configured: IN1=%d, IN2=%d\n", ACTUATOR_IN1, ACTUATOR_IN2);

    // Start safely stopped
    stopActuator();

    delay(2000);
}

void loop()
{
        extendActuator();
        delay(2000);
    
        retractActuator();
        delay(2000);
}
