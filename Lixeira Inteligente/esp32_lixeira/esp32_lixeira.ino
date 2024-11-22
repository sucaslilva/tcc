#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include "index.h"

const int PINO_TRIG = 4; // Pino D4 conectado ao TRIG do HC-SR04
const int PINO_ECHO = 2; // Pino D2 conectado ao ECHO do HC-SR04

const char *ssid = "HOME_LUCAS";
const char *senha = "lpsilva123";

const char* serverUrl = "http://lsconnectiondb.ddns.net:8180/status";

String html = "";
float altura = 19.00;
float cont = 0.00;
AsyncWebServer server(80);

void enviaLogSensor(int sensor_id, float porc_vol) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Cria o JSON para enviar
    String jsonData = "{\"sensor_id\": " + String(sensor_id) + ", \"porc_vol\": " + String(porc_vol) + "}";

    // Envia o pedido POST
    int httpResponseCode = http.POST(jsonData);

    // Verifica a resposta do servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Erro ao enviar POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Erro na conexão Wi-Fi");
  }
}

void connectWiFi() {
  Serial.println("Conectando a...");
  Serial.println(ssid);

  WiFi.begin(ssid, senha);
  unsigned long startAttemptTime = millis();

  // Verifica conexão com timeout
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi Conectado");
    Serial.println("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Falha na conexão WiFi");
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(PINO_TRIG, OUTPUT); // Configura o pino TRIG como saída
  pinMode(PINO_ECHO, INPUT); // Configura o pino ECHO como entrada

  connectWiFi();

  /*server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
    request->send(200,"text/html",getHTML(html));
  });

  server.begin();*/
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2); // Ajuste do tempo de delay
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);
  
  long duracao = pulseIn(PINO_ECHO, HIGH);  
  float distancia = (duracao * 0.0343) / 2;
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Calcula capacidade
  float capacidade = ((distancia * 100) / altura);
  // Arredonda para duas casas
  capacidade = round(capacidade * 100.0) / 100.0;
  // Valida capacidade > 0
  if (capacidade >= 0) {
    Serial.print("Capacidade: ");
    Serial.print(capacidade);
    Serial.println(" %");
    enviaLogSensor(1, capacidade);
  }
  
  delay(900000); 
}
