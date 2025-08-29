# 🌿 Aeroponics Farming using IoT

![GitHub repo size](https://img.shields.io/github/repo-size/BrijModi/Aeroponics-Farming-using-IoT)
![GitHub last commit](https://img.shields.io/github/last-commit/BrijModi/Aeroponics-Farming-using-IoT)
![GitHub license](https://img.shields.io/github/license/BrijModi/Aeroponics-Farming-using-IoT)

An **automated, IoT-integrated aeroponics system** designed to provide a **sustainable and efficient solution for urban and indoor agriculture**.  
By leveraging sensors, smart controls, and IoT connectivity, the system creates an **optimal growth environment for plants**, significantly reducing **water consumption** and **manual labor** compared to traditional farming methods.

---

## ✨ Features

- 🌱 **Automated Nutrient Misting** – Delivers a nutrient-rich mist directly to plant roots at controlled intervals.  
- 📊 **Real-time Environmental Monitoring** – Tracks **temperature, humidity, gas levels, and light intensity**.  
- 💧 **Nutrient & Water Level Management** – Monitors **TDS concentration** and **water levels** to maintain a healthy solution.  
- 📲 **Remote Control & Alerts** – Integrates with **Blynk App** for remote monitoring, real-time alerts, and manual overrides.  
- ❄️ **Automated Climate Control** – Cooling fan activates automatically to regulate temperature and humidity.  

---

## ⚙️ Technical Stack

### 🔌 Hardware
- **Microcontroller:** ESP32  
- **Sensors:**  
  - DHT22 (Temperature & Humidity)  
  - BH1750 (Light Intensity)  
  - MQ135 (Gas Sensor)  
  - Analog TDS Sensor (Nutrients)  
  - Soil Moisture Sensor (Water Level Detection)  
- **Actuators:** Relay Module, Water Misting Pump, Cooling Fan  

### 💻 Software & Platform
- **Programming Language:** C++ (Arduino / ESP32)  
- **IoT Platform:** Blynk App  

---

## 🔁 Working Principle

The IoT-based aeroponics system functions as a **closed-loop control mechanism**, dynamically adjusting its operations based on real-time sensor data.

1. **Data Acquisition** – Sensors continuously measure environment (DHT22 → temp/humidity, BH1750 → light, MQ135 → gas, TDS → nutrients, Soil Sensor → water levels).  
2. **Central Processing** – ESP32 processes data and compares against thresholds (e.g., activates fan when temperature rises).  
3. **Automated Control** – Relay switches control **pump** and **fan**, triggered by sensor data.  
4. **IoT Feedback Loop** – Blynk App enables **remote monitoring, alerts, and manual overrides**.  

---

## 🚧 Challenges Faced

- ⚡ **Integration Complexity** – Managing communication between multiple sensors, actuators, and the ESP32 while ensuring stable power delivery.  
- 🔋 **Power Management** – Different voltage requirements (3.3V, 5V, 9V) required a **custom PSU** with step-down transformers and buck converters for stable operation.  

---

## 🚀 Future Scope

- 📈 **Advanced Data Analytics** – Use ML models for predictive plant growth and automated optimization.  
- ☀️ **Sustainable Power** – Integrate renewable energy sources (e.g., solar panels).  
- 🌇 **Vertical & Multi-Crop Scalability** – Adapt design for vertical farming and support multiple crop types.  
- 🌐 **Enhanced Connectivity** – Global IoT access for collaboration and research.  

---

