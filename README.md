# Classroom Automation System

A smart classroom automation solution using **NodeMCU**, **Firebase Realtime Database**, and **MIT App Inventor** for remote and automated control of lights and fans.  
The system is designed to improve energy efficiency, enhance convenience, and provide secure remote access.

---

## 📌 Features

- **Remote Control**: Switch lights and fans ON/OFF from a mobile app.
- **Login Authentication**: Secure access via authenticated app login.
- **Real-time Sync**: Uses **Firebase Realtime Database** for instant updates between the app and NodeMCU.
- **Automation Mode**:
  - Controls appliances based on **room occupancy**.
  - Adjusts fan operation according to **ambient temperature**.
- **Energy Savings**: Estimated **25–30% reduction** in energy usage.

---

## 🛠️ Tech Stack

### Hardware
- NodeMCU ESP8266
- DHT11 Temperature & Humidity Sensor
- PIR Motion Sensor (for occupancy detection)
- Relay Modules (for controlling lights and fans)
- Power Supply

### Software
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Firebase Realtime Database](https://firebase.google.com/products/realtime-database)
- [MIT App Inventor](https://appinventor.mit.edu/)

---

## 📂 Project Structure

```
Classroom-Automation/
├── firmware/            # NodeMCU Arduino code
├── app/                 # MIT App Inventor .aia file
├── firebase-setup/      # Database rules and structure
├── docs/                # Circuit diagrams, images, documentation
└── README.md
```

---

## 🔌 Circuit Diagram

> The circuit connects NodeMCU with a DHT11 sensor, PIR sensor, and relay modules for controlling classroom appliances.

![Circuit Diagram](docs/circuit-diagram.png)

---

## 📊 How It Works

1. The **PIR sensor** detects movement to identify occupancy.
2. The **DHT11 sensor** measures temperature.
3. The NodeMCU updates Firebase with sensor readings.
4. The mobile app displays real-time data and provides control switches.
5. In automation mode:
   - If no occupancy is detected, lights and fans turn OFF automatically.
   - Fans adjust speed or switch ON/OFF based on the temperature threshold.

---

## 📈 Future Improvements
- Integration with voice assistants (Google Assistant / Alexa)
- Cloud-based analytics for usage patterns
- Solar power integration for sustainability

---

## 📜 License
This project is licensed under the [MIT License](LICENSE).

---

## 👨‍💻 Author
**Sumesh Sarkar**  
[GitHub](https://github.com/Sumesh-skr) | [LinkedIn](https://linkedin.com/in/sumesh-skr)
