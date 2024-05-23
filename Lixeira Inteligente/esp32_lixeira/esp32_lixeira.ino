#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESPAsyncWebServer.h>
#include <string.h>
#include "index.h"
const int PINO_TRIG = 4; // Pino D4 conectado ao TRIG do HC-SR04
const int PINO_ECHO = 2; // Pino D2 conectado ao ECHO do HC-SR04

const char *ssid = "iPhone de Lucas";
const char *senha = "lpsilva123";
String html = "";
float altura = 33.00;
AsyncWebServer server(80);
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

  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
    request->send(200,"text/html",getHTML(html));
  });

  server.begin();
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
  html = String(capacidade);
  delay(2000); // Aguarda 1 segundo antes de fazer a próxima leitura
}
//long -> A variável "long" é utilizada para armazenar números inteiros longos, ou seja, números inteiros maiores do que os que podem ser armazenados em uma variável "int"
//delayMicroseconds -> A função delayMicroseconds() lida com microssegundos (10 elevado a -6 segundos), enquanto a função delay() lida com milissegundos (10 elevado a -3 segundos).