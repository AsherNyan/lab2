#include "pitches.h"
#include "buzzer.h"
#include "button.h"

#define BUZZER_PIN 3
#define BUTTON_PIN 4
const float voltsPerMeasurement = 5.0/1024.0;
const int sensorPin=A0;

enum AlarmState{
  ALARM_ON,
  STAND_BY  
};

int notes[] = {NOTE_FS3, NOTE_C3};
double durations[] = {6,6};
int melodyLength = 2;
float sensor_value;

Buzzer buzzer(BUZZER_PIN);
Button button(BUTTON_PIN);

AlarmState alarm = STAND_BY;

void setup()
{
  buzzer.setMelody(notes, durations, melodyLength);
}
void loop()
{
  switch (alarm)
  {
    case STAND_BY:
    sensor_value = readDist();
    if (sensor_value<10)
    {
      buzzer.turnSoundOn();
      alarm = ALARM_ON;
    }
      break;
    
    case ALARM_ON:
    buzzer.playSound();
    if (button.wasPressed())
    {
      buzzer.turnSoundOff();
      alarm = STAND_BY;
    }
      break;
  }
}

float readDist(){
  float volts = readAnalog() * voltsPerMeasurement;
  return pow(14.7737/volts, 1.2134); // for small IR sensor (SHARP 2Y0A21)
}

float readAnalog()
{
  int sum = 0;
  int maxV = -5000;
  int minV = 5000;
  int n = 15;
  for (int i = 0; i < n; i++)
  {
    int cur = analogRead(sensorPin);
    if (cur > maxV)
    {
      maxV = cur;
    }
    if (cur < minV)
    {
      minV = cur;
    }
    sum += cur;
    delay(6);
  }
  return (sum - maxV - minV) / (float)(n - 2);
}
