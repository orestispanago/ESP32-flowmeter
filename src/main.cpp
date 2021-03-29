#include <Arduino.h>
#define FLOWMETER_PIN 13 // As marked on board

long previousMillis;
int interval = 1000; // milliseconds
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec;
float flowRate;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup()
{
  Serial.begin(115200);
  pinMode(FLOWMETER_PIN, INPUT_PULLUP); // make sure that your pin has internal pullup
  attachInterrupt(digitalPinToInterrupt(FLOWMETER_PIN), pulseCounter, FALLING);
}

void loop()
{
  if (millis() - previousMillis > interval)
  {
    Serial.print("pulseCount: \t");
    Serial.println(pulseCount);
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    Serial.print("Flow rate: ");
    Serial.print(flowRate);
    Serial.println(" L/min");
  }
}