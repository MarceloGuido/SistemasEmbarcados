/*********
  Baseado em: https://randomnerdtutorials.com/esp8266-web-server/
*********/

// Carregar a biblioteca do WiFi
#include <ESP8266WiFi.h>

// Substituir pelas credenciais da rede
const char* ssid     = "ALTERAR_PELO_SSID_DA_REDE";
const char* password = "SENHA_DA_REDE";

// Iniciar o servidor web na porta 80
WiFiServer server(80);

// Variavel para armazenar a requisição HTTP
String header;

// Variaveis auxiliares para armazenar o estado atual da saída
String EstadoD1 = "off";
String EstadoD2 = "off";

// Caso não esteja usando o NodeMCU 1.0 no Arduino IDE, descomente as linhas 24 e 25
// Atribuir variáveis ​​de saída aos pinos GPIO 
//const int D1 = 5;
//const int D2 = 4;

// Hora atual
unsigned long currentTime = millis();
// Hora anterior
unsigned long previousTime = 0; 
// Define tempo de timeout em milisegundos (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Inicializa as saidas
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  // Desliga as saidas
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);

  // Conecta a rede WiFi
  Serial.print("Conectando a rede ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprime o endereço IP local e inicializa o servidor web
  Serial.println("");
  Serial.println("Conectado ao WiFi.");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Fica ouvindo os clientes que conectam ao servidor

  if (client) {                             // Se um novo cliente conectar 
    Serial.println("Novo cliente.");        // imprime uma menssagem no monitor serial 
    String currentLine = "";                // cria uma string para armazenar os dados recebidos do cliente
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // fica em loop enquanto o cliente estiver conectado
      currentTime = millis();         
      if (client.available()) {             // Se houver bytes para ler do cliente, 
        char c = client.read();             // le um byte, e então
        Serial.write(c);                    // imprime no monitor serial 
        header += c;
        if (c == '\n') {                    // Se o byte for um caractere de nova linha
          // se a linha atual estiver em branco, você terá dois caracteres de nova linha seguidos.
          // esse é o fim da solicitação HTTP do cliente, então envie uma resposta:
          if (currentLine.length() == 0) {
            // Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP/1.1 200 OK)
            // e um tipo de conteúdo para que o cliente saiba o que está por vir, depois uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // lia e desliga as GPIOs
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              EstadoD1 = "on";
              digitalWrite(D1, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              EstadoD1 = "off";
              digitalWrite(D1, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              EstadoD2 = "on";
              digitalWrite(D2, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              EstadoD2 = "off";
              digitalWrite(D2, LOW);
            }
            
            // Exibe a pagina web HTML 
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS para estilizar os botões liga/desliga
            // Sinta-se à vontade para alterar os atributos de cor de fundo e tamanho da fonte para atender às suas preferências
            client.println("<style>html { font-family: Verdana; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #000000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #273336;}</style></head>");
            
            // Título da página da web
            client.println("<body><h1>Servidor Web ESP8266</h1>");
            
            // Exibir o estado atual e botões ON/OFF para GPIO 5  
            client.println("<p>Estado do GPIO 5 (D1): " + EstadoD1 + "</p>");
            // Se o EstadoD1 estiver desligado, exibe o botão ON       
            if (EstadoD1=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Exibir o estado atual e botões ON/OFF para GPIO 4
            client.println("<p>Estado do GPIO 4 (D2): " + EstadoD2 + "</p>");
            // Se o EstadoD2 estiver desligado, exibe o botão ON
            if (EstadoD2=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // A resposta HTTP termina com outra linha em branco
            client.println();
            // Saida do loop
            break;
          } else { // se nao tiver uma nova linha, limpa currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // se tiver qualquer outra coisa além de um caractere de retorno de carro (carriage return),
          currentLine += c;      // adicione-o ao final da currentLine
        }
      }
    }
    // Limpa a variável de cabeçalho
    header = "";
    // Encerra a conexao
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}
