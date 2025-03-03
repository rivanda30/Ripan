// Blynk Template and Auth Token
#define BLYNK_TEMPLATE_ID "TMPL6PNL28vDo"        // Ganti dengan Template ID Anda
#define BLYNK_TEMPLATE_NAME "Tugas Modul IoT"    // Ganti dengan Template Name Anda
#define BLYNK_AUTH_TOKEN "ShnRYagNWp1-j1EFZsjklbUKI9b1cgIs" // Token Blynk Anda

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi Credentials
char ssid[] = "J7 prime pro max";
char pass[] = "baubanget1";

// Sensor & Buzzer Pins
#define ONE_WIRE_BUS 4       // DS18B20 on GPIO 4
#define TOUCH_SENSOR_PIN 25  // KY-036 DO on GPIO 25
#define BUZZER_PIN 26        // Buzzer on GPIO 26
#define I2C_ADDR 0x27        // LCD I2C Address (0x27 or 0x3F)

// Initialize Sensors
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

BlynkTimer timer; // Timer for sending data

void sendDataToBlynk() {
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    int touchDetected = analogRead(TOUCH_SENSOR_PIN);
    String touchStatus = touchDetected ? "Touch Detected" : "No Touched";

    // Send data to Blynk
    Blynk.virtualWrite(V0, temperatureC); // Temperature to V0
    Blynk.virtualWrite(V1, touchDetected); // Touch status to V1

    // Print to LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperatureC);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print(touchStatus);
}

void setup() {
        Serial.begin(115200);
        Serial.println("Memulai...");
    
        sensors.begin();
        pinMode(TOUCH_SENSOR_PIN, INPUT);
        pinMode(BUZZER_PIN, OUTPUT);
        digitalWrite(BUZZER_PIN, LOW);
    
        lcd.init();
        lcd.backlight();
    
        Serial.println("Menghubungkan ke WiFi...");
        Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
    
        Serial.println("Terhubung ke WiFi dengan IP:");
        Serial.println(WiFi.localIP());
    
        // Send data every 2 seconds
        timer.setInterval(2000L, sendDataToBlynk);
    }

void loop() {
    Blynk.run();
    timer.run();

    int touchDetected = digitalRead(TOUCH_SENSOR_PIN);
    if (touchDetected) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
    }
}