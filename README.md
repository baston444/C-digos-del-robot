## Instrucciones de Instalación y Uso

### Clonar el repositorio

Clonar el repositorio del proyecto utilizando el siguiente comando:

```bash
git clone https://github.com/YisusTLL/PROY-2026-GRUPO-12.git
```

Ingresar a la carpeta del proyecto:

```bash
cd PROY-2026-GRUPO-12
```

### Dependencias

Antes de ejecutar el proyecto, instalar **Arduino IDE 2.x** y la tarjeta **ESP32 by Espressif Systems** desde el Gestor de tarjetas.

Además, instalar las siguientes librerías desde el Administrador de Bibliotecas de Arduino IDE:

* WiFi
* WebServer
* Wire
* Adafruit GFX Library
* Adafruit SSD1306

### Ejecución

1. Configurar las credenciales de la red WiFi en el archivo `ULTRA_ESP32.ino`.
2. Cargar `ULTRA_ESP32.ino` en la placa ESP32.
3. Cargar `ULTRA_ARDU.ino` en la placa Arduino Mega 2560.
4. Verificar que ambos dispositivos estén correctamente conectados.
5. Al iniciar el sistema, el Arduino enviará automáticamente las mediciones del sensor ultrasónico al ESP32 mediante la red WiFi.
