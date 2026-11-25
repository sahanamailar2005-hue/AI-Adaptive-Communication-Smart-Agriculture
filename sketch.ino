
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define SOIL_PIN 34

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

String apiKey = "RASG6G5NB9VJBQGD";

float previousSoil = -1;
float previousTemp = -1;
float previousHumidity = -1;

int adaptiveCount = 0;
int fixedCount = 0;
int stepCount = 0;

void setup() {
  Serial.begin(115200);

  dht.begin();
  lcd.init();
  lcd.backlight();

  WiFi.begin("Wokwi-GUEST", "", 6);

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1500);
}

void loop() {
  float soilRaw = analogRead(SOIL_PIN);
  float soil = map(soilRaw, 0, 4095, 100, 0);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT sensor reading error");
    delay(3000);
    return;
  }

  stepCount++;

  // Fixed reporting: send every 3 readings
  if (stepCount % 3 == 0) {
    fixedCount++;
  }

  // Adaptive reporting: send when readings change
  // significantly, or after 5 readings
  bool sendData = false;

  if (previousSoil < 0 ||
      abs(soil - previousSoil) >= 3 ||
      abs(temperature - previousTemp) >= 2 ||
      abs(humidity - previousHumidity) >= 3 ||
      soil <= 20 ||
      stepCount % 5 == 0) {
    sendData = true;
  }

  if (sendData) {
    adaptiveCount++;

    previousSoil = soil;
    previousTemp = temperature;
    previousHumidity = humidity;

    sendToThingSpeak(soil, temperature, humidity);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Data Transmitted");
    lcd.setCursor(0, 1);
    lcd.print("Adaptive: ");
    lcd.print(adaptiveCount);

    Serial.println("Adaptive: DATA SENT");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stable Readings");
    lcd.setCursor(0, 1);
    lcd.print("Waiting...");

    Serial.println("Adaptive: WAITING");
  }

  Serial.print("Soil: ");
  Serial.print(soil);
  Serial.print("% | Temp: ");
  Serial.print(temperature);
  Serial.print(" C | Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(16000);
}

void sendToThingSpeak(float soil, float temp, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=";
    url += apiKey;
    url += "&field1=" + String(soil);
    url += "&field2=" + String(temp);
    url += "&field3=" + String(humidity);
    url += "&field4=" + String(adaptiveCount);
    url += "&field5=" + String(fixedCount);

    http.begin(url);
    int response = http.GET();

    Serial.print("ThingSpeak response: ");
    Serial.println(response);

    http.end();
  }
}
