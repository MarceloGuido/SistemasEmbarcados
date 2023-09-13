#include <ESP8266WiFi.h>	// Biblioteca do WiFi

const char* ssid     = "SSID";	// Nome da rede WiFi
const char* password = "PASSWORD";	// Senha 

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);	// Conexão ao Wi-Fi
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Aguarda conexão do Wi-Fi
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Conexão estabelecida!");  
  Serial.print("Endereço IP:\t");
  Serial.println(WiFi.localIP());	// Endereço IP do ESP8266
}

void loop() { }
