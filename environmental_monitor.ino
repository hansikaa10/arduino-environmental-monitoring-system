#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- DHT ----------------
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---------------- Pins ----------------
const int GAS_SENSOR_PIN = A0;
const int BUZZER_PIN = 6;

// ---------------- Thresholds ----------------
const int SAFE_LIMIT = 400;
const int WARNING_LIMIT = 700;

// ---------------- Variables ----------------
int gasValue;
float temperature;
float humidity;


void setup() {

  Serial.begin(9600);

  dht.begin();

  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Environmental");
  lcd.setCursor(0,1);
  lcd.print("Monitor Ready");

  delay(2000);
  lcd.clear();
}


void loop() {

  readSensors();

  if (isnan(temperature) || isnan(humidity)) {

    lcd.clear();
    lcd.print("Sensor Error");

    Serial.println("Failed to read DHT sensor.");

    delay(2000);
    return;
  }

  displayReadings();

  checkAirQuality();

  printSerialOutput();

  delay(2000);
}


void readSensors() {

  gasValue = analogRead(GAS_SENSOR_PIN);

  temperature = dht.readTemperature();

  humidity = dht.readHumidity();
}

void displayReadings() {

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("G:");
  lcd.print(gasValue);

  lcd.print(" T:");
  lcd.print(temperature,1);

  lcd.setCursor(0,1);

  lcd.print("H:");
  lcd.print(humidity,0);
  lcd.print("%");
}


void checkAirQuality() {

  if (gasValue < SAFE_LIMIT) {

    digitalWrite(BUZZER_PIN, LOW);

    lcd.setCursor(10,1);
    lcd.print("SAFE");

  }

  else if (gasValue < WARNING_LIMIT) {

    lcd.setCursor(10,1);
    lcd.print("WARN");

    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);

  }

  else {

    lcd.setCursor(10,1);
    lcd.print("DANGER");

    digitalWrite(BUZZER_PIN, HIGH);

  }
}


void printSerialOutput() {

  Serial.println("--------------------------------");

  Serial.print("Gas Level    : ");
  Serial.println(gasValue);

  Serial.print("Temperature  : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity     : ");
  Serial.print(humidity);
  Serial.println(" %");

  if (gasValue < SAFE_LIMIT)
    Serial.println("Air Quality  : SAFE");

  else if (gasValue < WARNING_LIMIT)
    Serial.println("Air Quality  : WARNING");

  else
    Serial.println("Air Quality  : DANGER");

  Serial.println("--------------------------------");
}
