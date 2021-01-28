#include <WiFi.h>
#include <PubSubClient.h>
#define LED_BUILTIN

const char *ssid = "Hilman Wifi";   
const char *password = "12345abcde"; 

const byte LIGHT_PIN = 5;          
const char *ID = "LED";  
const char *TOPIC = "test"; 
const char *STATE_TOPIC = "masuk";  

IPAddress broker(192,168,1,10);
WiFiClient wclient;

PubSubClient client(wclient); 

void callback(char* topic, byte* payload, unsigned int length) {
  String response;

  for (int i = 0; i < length; i  ) {
    response  = (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print(" : ");
  Serial.println(response);
  if(response == "on")  
  {
    digitalWrite(LIGHT_PIN, HIGH);
    client.publish(STATE_TOPIC,"on");
  }
  else if(response == "off")  
  {
    digitalWrite(LIGHT_PIN, LOW);
    client.publish(STATE_TOPIC,"off");
  }
}


void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnect to client
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if(client.connect(ID)) {
      client.subscribe(TOPIC);
      Serial.println("connected");
      Serial.print("Subcribed to: ");
      Serial.println(TOPIC);
      Serial.println('\n');

    } else {
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200); 
  pinMode(LIGHT_PIN, OUTPUT);
  delay(1000);
  setup_wifi(); 
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected())  
  {
    reconnect();
  }
  client.loop();
}
