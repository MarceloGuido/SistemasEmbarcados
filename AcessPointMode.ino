#include <ESP8266WiFi.h>	// Biblioteca do WiFi

const char *ssid = "ESP8266-X";	// Substituir X pelo numero do grupo
const char *password = "Etec-SEII";	// Senha

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);	// Inicia o access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" iniciado");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());	// Envia o IP do ESP8266 para o celular
}

void loop() { }
