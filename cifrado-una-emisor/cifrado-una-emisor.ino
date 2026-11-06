
#include <WiFi.h>

// Configuracion Wi-Fi
const char* nombreRed = "ESP32_XOR";
const char* password = "12345678";

WiFiServer servidor(80);
WiFiClient cliente;

const byte llaveXOR = 0b00000100;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000);

  // Crear red Wi-Fi
  WiFi.softAP(nombreRed, password);

  servidor.begin();

  Serial.println("ESP32 A - EMISOR XOR");
  Serial.println("--------------------------");
  Serial.print("Red Wi-Fi: ");
  Serial.println(nombreRed);
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.softAPIP());

  Serial.println("Esperando conexion del ESP32 B...");
}

void loop() {

  // Esperar conexion del receptor
  if (!cliente || !cliente.connected()) {
    cliente = servidor.available();

    if (cliente) {
      Serial.println("ESP32 B conectado!");
    }
  }

  // Leer mensaje del Monitor Serie
  if (Serial.available() > 0) {

    String original = Serial.readStringUntil('\n');
    original.trim();

    if (original.length() == 0) return;

    String cifrado = "";

    // Aplicar cifrado XOR
    for (int i = 0; i < original.length(); i++) {

      byte letra = (byte)original[i];
      byte resultado = letra ^ llaveXOR;

      cifrado += (char)resultado;
    }

    Serial.print("Mensaje original: ");
    Serial.println(original);

    Serial.print("Mensaje cifrado: ");
    Serial.println(cifrado);

    // Enviar los bytes cifrados codificados en hexadecimal
    if (cliente && cliente.connected()) {

      for (int i = 0; i < cifrado.length(); i++) {

        byte dato = (byte)cifrado[i];

        if (dato < 16) cliente.print("0");

        cliente.print(dato, HEX);
      }

      cliente.println();

      Serial.println("Mensaje enviado correctamente.");
    }
    else {
      Serial.println("No hay ESP32 B conectado.");
    }

    Serial.println("--------------------------");
  }
}
