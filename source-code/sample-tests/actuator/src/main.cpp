#include <Arduino.h>
#include <cstring>

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

void oscillate(const char* startDirection, int reps, int time) {
    for (int i = 0; i < reps; i++) {
        if (strcmp(startDirection, "FORWARD") == 0)
        {
            extendActuator();
            delay(time);
    
            retractActuator();
            delay(time);
        } else if (strcmp(startDirection, "REVERSE") == 0) {
            retractActuator();
            delay(time);

            extendActuator();
            delay(time);
        }
    }
}

void pattern1(int timeUnit)
{
    for (int i = 0; i < 3; i++) {
        oscillate("FORWARD", 4, timeUnit);
    
        extendActuator();
        delay(timeUnit * 2);
    
        oscillate("REVERSE", 2, timeUnit);
        
        retractActuator();
        delay(timeUnit * 2);
    }

    oscillate("FORWARD", 1, timeUnit);
    delay(timeUnit);
    oscillate("FORWARD", 1, timeUnit);
    delay(timeUnit);
    oscillate("FORWARD", 1, timeUnit);
    oscillate("FORWARD", 1, timeUnit);
    delay(timeUnit);
    oscillate("FORWARD", 1, timeUnit);
    delay(timeUnit);
    oscillate("FORWARD", 1, timeUnit);
}

void pattern2(int timeUnit)
{
    for (int i = 0; i < 4; i++) {
        oscillate("FORWARD", 4, timeUnit);
    
        extendActuator();
        delay(timeUnit * 2);
    
        oscillate("REVERSE", 2, timeUnit);
        
        retractActuator();
        delay(timeUnit * 2);
    }
}

void pattern3(int timeUnit)
{
    extendActuator();
    delay(timeUnit * 2);
    retractActuator();
    delay(timeUnit * 2);
    extendActuator();
    delay(timeUnit * 2);
    retractActuator();
    delay(timeUnit * 2);

    extendActuator();
    delay(timeUnit * 2);
    retractActuator();
    delay(timeUnit);
    extendActuator();
    delay(timeUnit * 2);

    retractActuator();
    delay(timeUnit);
    extendActuator();
    delay(timeUnit);
    retractActuator();
    delay(timeUnit);
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("[setup] floater firmware starting");

    pinMode(ACTUATOR_IN1, OUTPUT);
    pinMode(ACTUATOR_IN2, OUTPUT);
    Serial.printf("[setup] pins configured: IN1=%d, IN2=%d\n", ACTUATOR_IN1, ACTUATOR_IN2);


    retractActuator();
    delay(4000);

    extendActuator();
    delay(2000);

    // Start safely stopped
    stopActuator();

    delay(500);
}

void loop()
{
    int timeUnit = 100;
    pattern3(timeUnit);
    pattern3(timeUnit);

    timeUnit = 50;
    pattern2(timeUnit);
    pattern2(timeUnit);

    timeUnit = 100;
    pattern1(timeUnit);

    timeUnit = 50;
    pattern2(timeUnit);
    pattern2(timeUnit);
}



