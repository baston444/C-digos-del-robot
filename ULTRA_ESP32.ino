/*
  ESP32 LILYGO LORA32 T3 V1.6.1
  Mediador entre página web y Arduino Mega

  Flujo:
  Página web -> WiFi -> LilyGO -> Serial GPIO13 -> Mega RX1 pin 19 -> Relés

  Conexión serial:
  LilyGO GPIO13 TX -> Arduino Mega RX1 pin 19
  LilyGO GND       -> Arduino Mega GND

  OLED:
  SDA -> GPIO21
  SCL -> GPIO22

  Rutas principales:
  /adelante
  /atras
  /izquierda
  /derecha
  /stop
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =======================
// WIFI
// =======================

const char* WIFI_SSID = "Redmi Note 13 5G";
const char* WIFI_PASS = "baston444";

// =======================
// OLED LILYGO LORA32
// =======================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// =======================
// SERIAL HACIA ARDUINO MEGA
// =======================

#define MEGA_RX 14   // No es necesario conectarlo para esta prueba
#define MEGA_TX 13   // GPIO13 TX hacia RX1 pin 19 del Mega

HardwareSerial MegaSerial(2);

// =======================
// SERVIDOR WEB
// =======================

WebServer server(80);

String ipActual = "Sin IP";
int contador = 0;
String ultimaAccion = "Esperando";

// =======================
// SETUP
// =======================

void setup() {
  Serial.begin(115200);

  // Serial2 hacia Arduino Mega
  MegaSerial.begin(9600, SERIAL_8N1, MEGA_RX, MEGA_TX);

  iniciarPantalla();
  mostrarPantalla("LILYGO MEDIADOR", "Iniciando...", "");

  conectarWiFi();
  configurarServidor();

  mostrarEstado("ONLINE", "Esperando web");
}

// =======================
// LOOP
// =======================

void loop() {
  server.handleClient();
}

// =======================
// PANTALLA
// =======================

void iniciarPantalla() {
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("No se encontro pantalla OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void mostrarPantalla(String linea1, String linea2, String linea3) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println(linea1);

  display.setCursor(0, 18);
  display.println(linea2);

  display.setCursor(0, 36);
  display.println(linea3);

  display.display();
}

void mostrarEstado(String accion, String detalle) {
  ultimaAccion = accion;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("ESP32 -> MEGA");

  display.setCursor(0, 12);
  display.print("IP:");
  display.println(ipActual);

  display.setCursor(0, 28);
  display.print("Accion:");
  display.println(accion);

  display.setCursor(0, 42);
  display.println(detalle);

  display.setCursor(0, 56);
  display.print("Total:");
  display.println(contador);

  display.display();
}

// =======================
// WIFI
// =======================

void conectarWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Conectando a WiFi...");
  Serial.println(WIFI_SSID);

  int intentos = 0;

  while (WiFi.status() != WL_CONNECTED && intentos < 40) {
    delay(500);
    Serial.print(".");
    intentos++;

    mostrarPantalla("Conectando WiFi", "Intento:", String(intentos));
  }

  if (WiFi.status() == WL_CONNECTED) {
    ipActual = WiFi.localIP().toString();

    Serial.println();
    Serial.print("IP del LilyGO: ");
    Serial.println(ipActual);

    mostrarPantalla("WiFi conectado", "IP:", ipActual);
    delay(1500);
  } else {
    ipActual = "Sin WiFi";

    Serial.println();
    Serial.println("No se pudo conectar a WiFi");

    mostrarPantalla("ERROR WIFI", "No conectado", "");
  }
}

// =======================
// SERVIDOR WEB
// =======================

void configurarServidor() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);

  // Comandos principales en español
  server.on("/adelante", HTTP_GET, []() {
    ejecutarAccion("ADELANTE");
  });

  server.on("/atras", HTTP_GET, []() {
    ejecutarAccion("ATRAS");
  });

  server.on("/izquierda", HTTP_GET, []() {
    ejecutarAccion("IZQUIERDA");
  });

  server.on("/derecha", HTTP_GET, []() {
    ejecutarAccion("DERECHA");
  });

  server.on("/stop", HTTP_GET, []() {
    ejecutarAccion("STOP");
  });

  server.on("/detener", HTTP_GET, []() {
    ejecutarAccion("STOP");
  });

  server.on("/parar", HTTP_GET, []() {
    ejecutarAccion("STOP");
  });

  // Compatibilidad con nombres en inglés
  server.on("/forward", HTTP_GET, []() {
    ejecutarAccion("ADELANTE");
  });

  server.on("/backward", HTTP_GET, []() {
    ejecutarAccion("ATRAS");
  });

  server.on("/back", HTTP_GET, []() {
    ejecutarAccion("ATRAS");
  });

  server.on("/left", HTTP_GET, []() {
    ejecutarAccion("IZQUIERDA");
  });

  server.on("/right", HTTP_GET, []() {
    ejecutarAccion("DERECHA");
  });

  // Compatibilidad con letras
  server.on("/F", HTTP_GET, []() {
    ejecutarAccion("ADELANTE");
  });

  server.on("/B", HTTP_GET, []() {
    ejecutarAccion("ATRAS");
  });

  server.on("/L", HTTP_GET, []() {
    ejecutarAccion("IZQUIERDA");
  });

  server.on("/R", HTTP_GET, []() {
    ejecutarAccion("DERECHA");
  });

  server.on("/S", HTTP_GET, []() {
    ejecutarAccion("STOP");
  });

  // Rutas tipo API
  server.on("/api/adelante", HTTP_GET, []() {
    ejecutarAccion("ADELANTE");
  });

  server.on("/api/atras", HTTP_GET, []() {
    ejecutarAccion("ATRAS");
  });

  server.on("/api/izquierda", HTTP_GET, []() {
    ejecutarAccion("IZQUIERDA");
  });

  server.on("/api/derecha", HTTP_GET, []() {
    ejecutarAccion("DERECHA");
  });

  server.on("/api/stop", HTTP_GET, []() {
    ejecutarAccion("STOP");
  });

  // Si llega algo desconocido, no devuelve 404.
  server.onNotFound(handleAnyRoute);

  server.begin();

  Serial.println("Servidor web iniciado en ESP32 LilyGO");
}

// =======================
// ACCIONES
// =======================

void ejecutarAccion(String accion) {
  contador++;

  Serial.print("Web recibida: ");
  Serial.println(accion);

  Serial.print("Enviando al Mega: ");
  Serial.println(accion);

  MegaSerial.println(accion);

  mostrarEstado(accion, "Mega:" + accion);

  responder("OK: " + accion + " enviado al Mega");
}

void responder(String mensaje) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(200, "text/plain", mensaje);
}

void handleRoot() {
  contador++;
  mostrarEstado("ROOT", "Conexion web");
  responder("ESP32 LilyGO mediador conectado. IP: " + ipActual);
}

void handleStatus() {
  contador++;
  mostrarEstado("STATUS", "ESP32 online");
  responder("OK ESP32 mediador. IP: " + ipActual);
}

void handleAnyRoute() {
  String ruta = server.uri();
  contador++;

  Serial.print("Ruta no asignada recibida: ");
  Serial.println(ruta);

  mostrarEstado("RUTA WEB", ruta);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

  if (server.method() == HTTP_OPTIONS) {
    server.send(200, "text/plain", "OK OPTIONS");
    return;
  }

  server.send(200, "text/plain", "Ruta recibida por ESP32, pero no asignada al Mega: " + ruta);
}