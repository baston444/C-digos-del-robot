const char* ssid = "Red robot";
const char* password = "Blacky2701";

// Cambia esta IP por la IP real que muestra el ESP32 LILYGO
const char* host = "10.90.75.183";
const int port = 80;

// Pines del sensor ultrasonico en el Mega
const int trigPin = 7;
const int echoPin = 8;

unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 1000; // enviar cada 1 segundo

bool enviarComandoEsperando(String comando, String esperado, unsigned long tiempoMaximo) {
  Serial.print("Enviando: ");
  Serial.println(comando);

  Serial3.println(comando);

  String respuesta = "";
  unsigned long inicio = millis();

  while (millis() - inicio < tiempoMaximo) {
    while (Serial3.available()) {
      char c = Serial3.read();
      respuesta += c;
      Serial.write(c);

      if (respuesta.indexOf(esperado) >= 0) {
        Serial.println();
        Serial.print("Encontrado: ");
        Serial.println(esperado);
        return true;
      }

      if (respuesta.indexOf("ERROR") >= 0) {
        Serial.println();
        Serial.println("Respuesta ERROR detectada");
        return false;
      }

      if (respuesta.indexOf("FAIL") >= 0) {
        Serial.println();
        Serial.println("Respuesta FAIL detectada");
        return false;
      }
    }
  }

  Serial.println();
  Serial.print("No encontre: ");
  Serial.println(esperado);
  return false;
}

void leerRespuesta(unsigned long tiempoMaximo) {
  unsigned long inicio = millis();

  while (millis() - inicio < tiempoMaximo) {
    while (Serial3.available()) {
      Serial.write(Serial3.read());
    }
  }
}

float medirDistanciaCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH, 30000);

  if (duracion == 0) {
    return -1; // sin lectura
  }

  float distancia = duracion * 0.0343 / 2.0;
  return distancia;
}

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  delay(500);

  Serial.println("Iniciando ESP8266...");

  enviarComandoEsperando("AT", "OK", 2000);
  enviarComandoEsperando("ATE0", "OK", 2000);
  enviarComandoEsperando("AT+CWMODE=1", "OK", 2000);
  enviarComandoEsperando("AT+CIPMUX=0", "OK", 2000);

  String conectarWiFi = "AT+CWJAP=\"";
  conectarWiFi += ssid;
  conectarWiFi += "\",\"";
  conectarWiFi += password;
  conectarWiFi += "\"";

  Serial.println("Conectando al WiFi...");
  bool conectado = enviarComandoEsperando(conectarWiFi, "OK", 15000);

  if (conectado) {
    Serial.println("WiFi conectado.");
  } else {
    Serial.println("No se pudo conectar al WiFi.");
  }

  enviarComandoEsperando("AT+CIFSR", "STAIP", 3000);

  Serial.println("Listo para enviar distancia.");
}

void loop() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= intervalo) {
    tiempoAnterior = tiempoActual;

    float distancia = medirDistanciaCM();

    Serial.print("Distancia medida: ");
    Serial.print(distancia);
    Serial.println(" cm");

    if (distancia > 0) {
      enviarDistancia(distancia);
    } else {
      Serial.println("Lectura invalida del sensor.");
    }
  }
}

void enviarDistancia(float distancia) {
  Serial.println();
  Serial.println("Enviando distancia al ESP32 LILYGO...");

  String iniciarConexion = "AT+CIPSTART=\"TCP\",\"";
  iniciarConexion += host;
  iniciarConexion += "\",";
  iniciarConexion += port;

  bool conexionOK = enviarComandoEsperando(iniciarConexion, "CONNECT", 3000);

  if (!conexionOK) {
    Serial.println("No se pudo abrir conexion TCP.");
    return;
  }

  String peticion = "";
  peticion += "GET /distancia?cm=";
  peticion += String(distancia, 1);
  peticion += " HTTP/1.0\r\n";
  peticion += "Host: ";
  peticion += host;
  peticion += "\r\n";
  peticion += "\r\n";

  String comandoEnviar = "AT+CIPSEND=";
  comandoEnviar += peticion.length();

  bool listoParaEnviar = enviarComandoEsperando(comandoEnviar, ">", 2000);

  if (!listoParaEnviar) {
    Serial.println("No aparecio el simbolo > para enviar datos.");
    return;
  }

  Serial.println("Enviando peticion HTTP...");
  Serial3.print(peticion);

  leerRespuesta(1000);
}