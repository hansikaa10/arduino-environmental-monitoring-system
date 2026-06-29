#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int gasSensorPin = A0;
const int buzzerPin = 6;
const int gasThreshold = 700;

int gasValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Gas + Temperature + Humidity Monitor Starting...");
  
  dht.begin();
  
  pinMode(gasSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read gas sensor
  gasValue = analogRead(gasSensorPin);
  
  // Read DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Handle DHT read failure 
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Warning: DHT sensor read failed. Retrying...");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    delay(2000);
    return;
  }
  
  // Gas alert 
  if (gasValue > gasThreshold) {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("!! GAS ALERT: High levels detected !!");
  } else {
    digitalWrite(buzzerPin, LOW);
  }
  
  // Serial output
  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  // LCD output — Line 1: Gas reading + alert indicator
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(gasValue);
  if (gasValue > gasThreshold) {
    lcd.print(" ALERT");
  }
  
  // LCD output — Line 2: Temperature + Humidity
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature, 1);  // 1 decimal place
  lcd.print("C H:");
  lcd.print(humidity, 0);     // 0 decimal places to fit on 16-char display
  lcd.print("%");
  
  delay(2000);
}
