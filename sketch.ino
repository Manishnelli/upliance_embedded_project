#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define ONE_WIRE_BUS 2


#define LOW_TEMP_LED     4   // Blue
#define TARGET_TEMP_LED  16  // Yellow
#define OVERHEAT_LED     17  // Red
#define HEATER_LED        5  // Orange


const float TARGET_TEMP = 50.0;
const float OVERHEAT_TEMP = 70.0;
const float TOLERANCE = 1.0; // ±1°C


bool heaterLedState = false;
bool overheatLedState = false;
unsigned long previousHeaterMillis = 0;
unsigned long previousOverheatMillis = 0;
const long blinkInterval = 500;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


LiquidCrystal_I2C lcd(0x27, 16, 2);

float lastTemp = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(21, 22);  
  sensors.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(LOW_TEMP_LED, OUTPUT);
  pinMode(TARGET_TEMP_LED, OUTPUT);
  pinMode(OVERHEAT_LED, OUTPUT);
  pinMode(HEATER_LED, OUTPUT);

  digitalWrite(LOW_TEMP_LED, LOW);
  digitalWrite(TARGET_TEMP_LED, LOW);
  digitalWrite(OVERHEAT_LED, LOW);
  digitalWrite(HEATER_LED, LOW);
}

void loop() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");


  digitalWrite(LOW_TEMP_LED, LOW);
  digitalWrite(TARGET_TEMP_LED, LOW);


  String statusMsg = "";
  String trendMsg = "";

  
  if (temperatureC > lastTemp + 0.2) {
    trendMsg = "Temp Rising";
  } else if (temperatureC < lastTemp - 0.2) {
    trendMsg = "Temp Falling";
  } else {
    trendMsg = "Stabilizing";
  }

  // Handle temperature status
  if (temperatureC > OVERHEAT_TEMP) {
    digitalWrite(OVERHEAT_LED, HIGH);
    digitalWrite(HEATER_LED, LOW);
    statusMsg = "Overheat";
  } 
  else if (abs(temperatureC - TARGET_TEMP) <= TOLERANCE) {
    digitalWrite(TARGET_TEMP_LED, HIGH);
    digitalWrite(HEATER_LED, LOW);
    statusMsg = "Idle";
  } 
  else if (temperatureC < TARGET_TEMP) {
    digitalWrite(LOW_TEMP_LED, HIGH);
    digitalWrite(OVERHEAT_LED, LOW);

    // Blink heater LED
    unsigned long currentMillis = millis();
    if (currentMillis - previousHeaterMillis >= blinkInterval) {
      previousHeaterMillis = currentMillis;
      heaterLedState = !heaterLedState;
      digitalWrite(HEATER_LED, heaterLedState);
    }
    statusMsg = "Heating";
  }
  else if (temperatureC > TARGET_TEMP && temperatureC <= OVERHEAT_TEMP) {
    // Blink red LED
    unsigned long currentMillis = millis();
    if (currentMillis - previousOverheatMillis >= blinkInterval) {
      previousOverheatMillis = currentMillis;
      overheatLedState = !overheatLedState;
      digitalWrite(OVERHEAT_LED, overheatLedState);
    }
    digitalWrite(HEATER_LED, LOW);
    statusMsg = "Caution!";
  }

  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatureC, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print(statusMsg);

 
  Serial.println("Trend: " + trendMsg);
  lcd.setCursor(10, 1);
  lcd.print(trendMsg.substring(0, 6)); 

  lastTemp = temperatureC;
  delay(200);
}
