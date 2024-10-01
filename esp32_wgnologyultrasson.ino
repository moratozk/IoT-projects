#include <WiFi.h>
#include <PubSubClient.h>
#include "EmonLib.h"
EnergyMonitor emon1;

const char* ssid =// "endereço rede";
const char* password = //"senha";
const char* mqtt_server = //"ip";

WiFiClient ESP32_nicolas;
PubSubClient client(//nome dispositivo (esp32) ex: ESP32_morato );

String my_payload;
String my_topic;

void setup_wifi() {

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String conc_payload;
  for (int i = 0; i < length; i++) {
    conc_payload += ((char)payload[i]);
  }
  my_payload = conc_payload;
  my_topic = topic;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão com o MQTT");

    if (client.connect("ESP32_nicolas")) {
      Serial.println("Conectado");
      client.subscribe("ESP32_nicolas_recebe");

    } else {
      Serial.print("FALHA COM O BROKER");
      Serial.println("Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  emon1.current(34, 100); 
  pinMode(2, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
}
void conexao() {
  if (WiFi.status() == WL_CONNECTED)
    digitalWrite(2, HIGH);
  else
    digitalWrite(2, LOW);
}
void recebe() {
  if (my_payload == "liga1")
    digitalWrite(19, HIGH);
  if (my_payload == "desliga1")
    digitalWrite(19, LOW);
  if (my_payload == "liga2")
    digitalWrite(18, HIGH);
  if (my_payload == "desliga2")
    digitalWrite(18, LOW);
  if (my_payload == "liga3")
    digitalWrite(17, HIGH);
  if (my_payload == "desliga3")
    digitalWrite(17, LOW);
  if (my_payload == "liga4")
    digitalWrite(16, HIGH);
  if (my_payload == "desliga4")
    digitalWrite(16, LOW);
}
void leitor(){
  float Irms = emon1.calcIrms(1480);
  Serial.println(Irms);
  client.publish("corrente",String(Irms).c_str());
  delay(1000);
}
void loop() {
  reconnect();
  client.loop();
  conexao();
  recebe();
  leitor();
}