/*
Code adapted from: https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "CHANGE-THIS";
const char* password = "CHANGE-THIS";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "CHANGE-THIS";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
bool ledState = false;



// LED Pin
const int ledPin = 6;

void setup() {
  Serial.begin(115200);


  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic esp32/output, it lights the led depending on its state. 
  // Changes the output state according to the message
  if (String(topic) == "esp32/led") {
    if (!ledState){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
      ledState = true;
    }
    else{
      Serial.println("off");
      digitalWrite(ledPin, LOW);
      ledState = false;
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  
  if (!client.connected()) {
    
    reconnect();
  }
  client.loop();
}