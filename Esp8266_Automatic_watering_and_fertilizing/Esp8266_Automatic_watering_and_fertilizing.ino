#include <ESP8266WiFi.h>
#include <MicroGear.h>
///  รับค่าจาก nodemcu
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D2, D3); // RX, TX
const char* ssid     = "KUY";
const char* password = "123456789xo0";

#define APPID   "NUTTACIT"
#define KEY     "xv4HY6mKMhDkP2a"
#define SECRET  "Dfgf23P2QIqPy7M4tGUVuf6a7"
#define ALIAS   "esp8266"

WiFiClient client;
String phRead = "";
String humiRead = "";
String timestart1 = "";
String timestart2 = "";
String timestart3 = "";
String timeEnd1 = "";
String timeEnd2 = "";
String timeEnd3 = "";

float PhWrite;
int   humiWrite;


String timephstart1 = "";
String timephstart2 = "";
String timephstart3 = "";
String timephEnd1 = "";
String timephEnd2 = "";
String timephEnd3 = "";
int timer = 0;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  String top = (String)topic;
  msg[msglen] = '\0';
  String message = (String)(char *)msg;
  // ---------------------------------------------------------- //
  Serial.print(top);
  Serial.print(" --> ");
  Serial.println(message);

  if (top == "/NUTTACIT/ionic/phstart") {
    phRead =  message;
    mySerial.println((String)"phstart=" + phRead);


  }else  if (top == "/NUTTACIT/ionic/phend") {
    phRead =  message;
    mySerial.println((String)"phend=" + phRead);


  }else if (top == "/NUTTACIT/ionic/humi") {
    humiRead  = message;
    mySerial.println((String)"humi=" + humiRead);

  } else if (top == "/NUTTACIT/ionic/compost1") {

    timephstart1  = message;
    mySerial.println((String)"timephstart1=" + timephstart1);
  }
  else if (top == "/NUTTACIT/ionic/compost2") {
    timephEnd1  = message;
    mySerial.println((String)"timephEnd1=" + timephEnd1);
  }
  else if (top == "/NUTTACIT/ionic/compost3") {
    timephstart2  = message;
    mySerial.println((String)"timephstart2=" + timephstart2);
  }
  else if (top == "/NUTTACIT/ionic/compost4") {
    timephEnd2  = message;
    mySerial.println((String)"timephEnd2=" + timephEnd2);
  }

  else if (top == "/NUTTACIT/ionic/compost5") {
    timephstart3  = message;
    mySerial.println((String)"timephstart3=" + timephstart3);
  }
  else if (top == "/NUTTACIT/ionic/compost6") {
    timephEnd3  = message;
    mySerial.println((String)"timephEnd3=" + timephEnd3);
  }
  else if (top == "/NUTTACIT/ionic/water1") {
    timestart1  = message;
    mySerial.println((String)"timestart1=" + timestart1);

  }
  else if (top == "/NUTTACIT/ionic/water2") {
    timeEnd1  = message;
    mySerial.println((String)"timeEnd1=" + timeEnd1);
  }
  else if (top == "/NUTTACIT/ionic/water3") {
    timestart2  = message;
    mySerial.println((String)"timestart2=" + timestart2);
  }
  else if (top == "/NUTTACIT/ionic/water4") {
    timeEnd2  = message;
    mySerial.println((String)"timeEnd2=" + timeEnd2);
  }
  else if (top == "/NUTTACIT/ionic/water5") {
    timestart3  = message;
    mySerial.println((String)"timestart3=" + timestart3);
  }
  else if (top == "/NUTTACIT/ionic/water6") {
    timeEnd3  = message;
    mySerial.println((String)"timeEnd3=" + timeEnd3);
  }

}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.subscribe("/ionic/+");
  microgear.setAlias(ALIAS);
}



void setup() {
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);

  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println("Starting...");

  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
}

void loop() {

  if (mySerial.available()) {
    String received = mySerial.readStringUntil('\n');
    //Serial.println(received);

    String received1 = received;
    //Serial.println(received1);
    if (!received1.indexOf("Moisture=")) {

      humiWrite  = received1.substring(9).toInt();
    }
    if (!received1.indexOf("PH=")) {

      PhWrite  = received1.substring(3).toFloat();
    }



  }


  if (microgear.connected()) {
    microgear.loop();
    if (timer >= 3000) {
      microgear.publish("/esp/th", "" + String(PhWrite) + "," + humiWrite);
      timer = 0;
    }
    else timer += 100;
  }
  else {
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  delay(100);
}
