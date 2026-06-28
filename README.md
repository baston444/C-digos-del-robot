
# Códigos del Robot

Este repositorio contiene los programas utilizados para el funcionamiento del robot, desarrollados para un Arduino Mega 2560 y un ESP32.

## Archivos

- `ULTRA_ARDU.ino`: Control del sensor ultrasónico y comunicación con el ESP32.
- `ULTRA_ESP32.ino`: Servidor web y comunicación WiFi.

## Instalación

1. Clonar el repositorio:

```bash
git clone https://github.com/baston444/C-digos-del-robot.git
```

2. Abrir los archivos `.ino` en Arduino IDE.

3. Instalar las librerías necesarias:

- WiFi
- WebServer
- Wire
- Adafruit GFX
- Adafruit SSD1306

4. Configurar las credenciales WiFi antes de cargar el programa al ESP32.

## Hardware utilizado

- Arduino Mega 2560
- ESP32
- Sensor ultrasónico HC-SR04
