
#include "TM1637.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#define ONE_WIRE_BUS_PIN 5
#define DISPLAY_CLOCK_PIN 2
#define DISPLAY_DATA_PIN 3
#define BUTTON_PIN 9

#define MAX_SENSORS 2

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

TM1637 display(DISPLAY_CLOCK_PIN, DISPLAY_DATA_PIN);

int selectedSensor = 0;
int buttonPressedPrevious = 0;

char descriptions[] = { 0x0F, 0x0b };

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  
  display.init();
  display.set(BRIGHT_DARKEST);

  sensors.begin();
}

void loop()
{
  byte buttonPressed = digitalRead(BUTTON_PIN);
  if (buttonPressed && !buttonPressedPrevious) {
    selectedSensor += 1;
    if (selectedSensor >= MAX_SENSORS) {
      selectedSensor = 0;  
    }
  }
  buttonPressedPrevious = buttonPressed;

  sensors.requestTemperatures();
  for (int sensor = 0; sensor < MAX_SENSORS; sensor++) {
      float temp = sensors.getTempCByIndex(sensor);
      if (sensor == selectedSensor) {
        display.display(0, descriptions[sensor]);
        display.display(1, (int)temp / 10);
        display.display(2, (int)temp % 10);
        display.display(3, (int)(temp * 10) % 10);
      }
  }
}
