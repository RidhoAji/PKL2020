#include <WiFi.h>
#include <PubSubClient.h>
#define LED 5
const char* ssid = "Hilman Wifi";
const char* password = "12345abcde";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(100);
  Serial.println();
  Serial.print("Menghubungkan ke ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Terhubung");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("pesan masuk [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Kontrol LED
  if ((char)payload[0] == '0') {
    digitalWrite(LED, LOW); }

  else {
    digitalWrite(LED, HIGH); 
  }

}

void reconnect() {
  
  while (!client.connected()) {
    Serial.println("Menghubungkan ke MQTT...");
   
   //ID klien
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
   
   //menghubungkan
    if (client.connect(clientId.c_str())) {
      Serial.println("Terhubung");
     
    //SUBSCRIBE
      client.subscribe("led");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() { 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED, OUTPUT);
 
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  else if (!client.connect() {
   Disconnect();
  }
  client.loop();
}

void Disconnect() {
   digitalWrite(LED, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
    delay(4000);
}
