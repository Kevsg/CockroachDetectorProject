#include <Time.h>
#include <TimeLib.h>

/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "33";
const char* password = "33445566";

#define APPID   "KevinNodeMCU"
#define KEY     "gCXI5fhwcoI10Ib"
#define SECRET  "Yzer4RmRJcIxz5d3iV5KUDHXz"
#define ALIAS   "NodeMCU"

WiFiClient client;

int timer = 0;
MicroGear microgear(client);
const char* s = "Testing";
char c;

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  //Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  //Serial.println((char *)msg);
  char on[2] = {'o','n'};
  char off[3] = {'o','f','f'};
  if(strcmp(on,(char *)msg)==0) {
    Serial.write("on1"); 
  } else if (strcmp(off,(char *) msg)){
    Serial.write("off");  
  }
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Found new member --> ");
  for (int i = 0; i < msglen; i++)
  Serial.print((char)msg[i]);
  Serial.println();
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Lost member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  /* Set the alias of this microgear ALIAS */
  microgear.setAlias(ALIAS);
}

void setup() {
  /* Add Event listeners */
  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, onMsghandler);

  /* Call onFoundgear() when new gear appear */
  microgear.on(PRESENT, onFoundgear);

  /* Call onLostgear() when some gear goes offline */
  microgear.on(ABSENT, onLostgear);

  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");

  /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
  /* You may want to use other method that is more complicated, but provide better user experience */
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);

  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
}

void loop() {

  HTTPClient http;
  /* To check if the microgear is still connected */
  if (microgear.connected()) {
    // Serial.println("connected");
    microgear.loop();
    if(Serial.available()) {
      String s = Serial.readString();
      if(s == "1") {
        microgear.chat("Backend","s1");
      } else if (s=="2") {
        microgear.chat("Backend","s2");
      }
    }
  } else {
      microgear.connect(APPID);
      delay(1000);
  }
  delay(500);
}
