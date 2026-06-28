## Instrucciones de Instalación y Uso

### Clonar el repositorio

Para obtener los códigos del robot, clonar el siguiente repositorio:

```bash
git clone https://github.com/baston444/C-digos-del-robot.git
```

Luego ingresar a la carpeta del proyecto:

```bash
cd C-digos-del-robot
```

### Dependencias

Antes de ejecutar el proyecto es necesario instalar:

* Arduino IDE 2.x.
* ESP32 by Espressif Systems (desde el Gestor de tarjetas de Arduino IDE).

Además, instalar las siguientes librerías:

* WiFi
* WebServer
* Wire
* Adafruit GFX Library
* Adafruit SSD1306

### Ejecución

1. Abrir `ULTRA_ESP32.ino` y `ULTRA_ARDU.ino` en Arduino IDE.
2. Configurar las credenciales de la red WiFi en `ULTRA_ESP32.ino`.
3. Cargar `ULTRA_ESP32.ino` en la placa ESP32.
4. Cargar `ULTRA_ARDU.ino` en la placa Arduino Mega 2560.
5. Una vez cargados ambos programas, el sistema comenzará a transmitir las mediciones del sensor ultrasónico al ESP32 mediante la red WiFi.
