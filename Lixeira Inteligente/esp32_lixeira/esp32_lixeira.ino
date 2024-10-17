#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESPAsyncWebServer.h>
#include <string.h>
#include <HTTPClient.h>
#include "index.h"
const int PINO_TRIG = 4; // Pino D4 conectado ao TRIG do HC-SR04
const int PINO_ECHO = 2; // Pino D2 conectado ao ECHO do HC-SR04

const char *ssid = "LUCAS";
const char *senha = "Lpsilva123";

const char* serverUrl = "http://lsconnectiondb.ddns.net:8180/status";


String html = "";
float altura = 33.00;
AsyncWebServer server(80);
void sendSensorData(int sensor_id, float porc_vol) {
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
void setup() {
  // Inicio código sensor
  Serial.begin(9600); // Inicializa a comunicação serial
  pinMode(PINO_TRIG, OUTPUT); // Configura o pino TRIG como saída
  pinMode(PINO_ECHO, INPUT); // Configura o pino ECHO como entrada
  //Fim Código Sensor

  //Início conexão com o WIFI
  Serial.println("Conectando a...");
  Serial.println(ssid);

  WiFi.begin(ssid,senha);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

 /*server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
    request->send(200,"text/html",getHTML(html));
  });

  server.begin();*/
}
void loop() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);
  
  long duracao = pulseIn(PINO_ECHO, HIGH); // Mede o tempo de resposta do ECHO  
  float distancia = (duracao * 0.0343) / 2;// Calcula a distância usando a velocidade do som (aproximadamente 343 m/s)
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  float capacidade = 100 - ((distancia * 100) / altura);
  sendSensorData(1, capacidade); 
  //html = String(capacidade);
  delay(20000);
}