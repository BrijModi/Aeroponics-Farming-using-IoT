#define BLYNK_TEMPLATE_ID "TEMPLATE ID"
#define BLYNK_TEMPLATE_NAME "TEMPLATE NAME"
#define BLYNK_AUTH_TOKEN "AUTH TOKEN"

// Libraries installed
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WIFI_ID";
char pass[] = "WIFI_PASS";

// Virtual Pins for Blynk
#define VIRTUAL_PIN_TEMP_C V0
#define VIRTUAL_PIN_HUMIDITY V1
#define VIRTUAL_PIN_CO2 V2
#define VIRTUAL_PIN_FAN_LED V3
#define VIRTUAL_PIN_WATER_LEVEL V4
#define VIRTUAL_PIN_TDS V5
#define VIRTUAL_PIN_MOTOR_LED V6
#define VIRTUAL_PIN_LIGHT V7  // New Virtual Pin for BH1750

// Sensor & Relay Pins
#define MQ135PIN 35
#define DHTPIN 27
#define FAN_RELAY_PIN 26
#define MOTOR_RELAY_PIN 25
#define WATER_LEVEL_PIN 33
#define TDS_PIN 32

// Thresholds
#define TEMP_THRESHOLD 30.00
#define HUMIDITY_THRESHOLD 90.00
#define WATER_LEVEL_THRESHOLD 40
#define NUM_READINGS 10
#define MOTOR_ON_DURATION 30000   // 70 sec
#define MOTOR_OFF_DURATION 60000  // 35 sec

// Sensor Instances
DHT dht(DHTPIN, DHT22);
BH1750 lightMeter;
BlynkTimer timer;

// State Variables
bool fanState = false;
bool motorState = false;
unsigned long motorLastRunTime = 0;

void setup() {
    Serial.begin(9600);
    dht.begin();
    Wire.begin(21, 22);  // I2C Setup for BH1750 (SDA=21, SCL=22)
    
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("BH1750 Sensor Initialized.");
    } else {
        Serial.println("Error initializing BH1750!");
    }

    pinMode(FAN_RELAY_PIN, OUTPUT);
    pinMode(MOTOR_RELAY_PIN, OUTPUT);
    digitalWrite(FAN_RELAY_PIN, HIGH);
    digitalWrite(MOTOR_RELAY_PIN, HIGH);

    Blynk.begin(auth, ssid, pass);

    timer.setInterval(2500L, sendSensorData);
    timer.setInterval(1000L, controlMotor);
}

void loop() {
    Blynk.run();
    timer.run();
}

void sendSensorData() {
    sendCO2Data();
    sendDHT22Data();
    sendWaterLevelData();
    sendTDSData();
    sendLightData();  // New Function for BH1750
}

void sendCO2Data() {
    int co2Level = map(analogRead(MQ135PIN), 0, 4095, 0, 5000);
    Blynk.virtualWrite(VIRTUAL_PIN_CO2, co2Level);
    Serial.printf("CO2: %d ppm\n", co2Level);
}

void sendDHT22Data() {
    float tc = dht.readTemperature();
    float hu = dht.readHumidity();
    if (!isnan(tc) && !isnan(hu)) {
        Blynk.virtualWrite(VIRTUAL_PIN_TEMP_C, tc);
        Blynk.virtualWrite(VIRTUAL_PIN_HUMIDITY, hu);
        controlFan(tc, hu);
        Serial.printf("Temp: %.2f°C, Humidity: %.2f%%\n", tc, hu);
    }
}

void controlFan(float temp, float humidity) {
    bool shouldFanBeOn = temp > TEMP_THRESHOLD;
    digitalWrite(FAN_RELAY_PIN, shouldFanBeOn ? LOW : HIGH);
    
    if (fanState != shouldFanBeOn) {
        fanState = shouldFanBeOn;
        Blynk.virtualWrite(VIRTUAL_PIN_FAN_LED, shouldFanBeOn ? 255 : 0);
        Serial.println(shouldFanBeOn ? "Fan ON" : "Fan OFF");

        // Notify event
        if (shouldFanBeOn) {
            Blynk.logEvent("fan_status", "Fan turned ON due to high temperature");
        } else {
            Blynk.logEvent("fan_status", "Fan turned OFF");
        }
    }
}

void sendWaterLevelData() {
    int avgWaterLevel = 0;
    for (int i = 0; i < NUM_READINGS; i++) avgWaterLevel += analogRead(WATER_LEVEL_PIN);
    int waterLevelPercentage = map(avgWaterLevel / NUM_READINGS, 50, 4000, 100, 0);
    Blynk.virtualWrite(VIRTUAL_PIN_WATER_LEVEL, waterLevelPercentage);

    if (waterLevelPercentage < WATER_LEVEL_THRESHOLD) {
        Blynk.logEvent("water_level", "Warning: Low water level detected!");
    }
}

void controlMotor() {
    unsigned long currentMillis = millis();
    if (!motorState && (currentMillis - motorLastRunTime >= MOTOR_OFF_DURATION)) {
        digitalWrite(MOTOR_RELAY_PIN, LOW);
        motorState = true;
        motorLastRunTime = currentMillis;
        Blynk.virtualWrite(VIRTUAL_PIN_MOTOR_LED, 255);
        Serial.println("Motor ON");

        // Notify event
        Blynk.logEvent("motor_status", "Motor turned ON");
    }
    if (motorState && (currentMillis - motorLastRunTime >= MOTOR_ON_DURATION)) {
        digitalWrite(MOTOR_RELAY_PIN, HIGH);
        motorState = false;
        motorLastRunTime = currentMillis;
        Blynk.virtualWrite(VIRTUAL_PIN_MOTOR_LED, 0);
        Serial.println("Motor OFF");

        // Notify event
        Blynk.logEvent("motor_status", "Motor turned OFF");
    }
}

void sendTDSData() {
    float temp = dht.readTemperature();
    if (isnan(temp)) return;

    long sum = 0;
    for (int i = 0; i < NUM_READINGS; i++) sum += analogRead(TDS_PIN);
    float voltage = (sum / NUM_READINGS * 5.0) / 4095.0;
    float tdsValue = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * (1 + 0.02 * (temp - 25.0));
    Blynk.virtualWrite(VIRTUAL_PIN_TDS, tdsValue);
    Serial.printf("TDS: %.2f ppm, Temp: %.2f°C\n", tdsValue, temp);
}

// New function to send BH1750 light sensor data to Blynk
void sendLightData() {
    float lux = lightMeter.readLightLevel();
    if (lux >= 0) {
        Blynk.virtualWrite(VIRTUAL_PIN_LIGHT, lux);
        Serial.printf("Light Intensity: %.2f lx\n", lux);
    } else {
        Serial.println("Error reading BH1750 sensor!");
    }
}