#include <WiFi.h>
#include "LiquidCrystal.h"

#define DOOR_PIN 8

WiFiClient client;
char SSID[] = "Boun_Wlan";
char PASSWORD[] = "";

const char* HOST = "13.75.95.218";

const char* DEVICE_REGISTERED_API_ENDPOINT = "/wowdoor/is-device-registered";
const char* DOOR_REQUEST_API_ENDPOINT = "/wowdoor/is-door-request-sent";

const char* static_anim[] = {
  "-               ",
  " -              ",
  "  -             ",
  "   -            ",
  "    -           ",
  "     -          ",
  "      -         ",
  "       -        ",
  "        -       ",
  "         -      ",
  "          -     ",
  "           -    ",
  "            -   ",
  "             -  ",
  "              - ",
  "               -"
};

char* uniqId = "168264";

int deviceRegistered = 0;
int doorOpened = 0;

int clientConnected = 0;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void reconnect() {
  clientConnected = client.connect(HOST, 80); 
}

void parseJSON(String line) {
  line.trim();
  
  int currIndex = 1;
  int lastIndex = line.length() - 1;
  if(line.startsWith("{")) {
    Serial.println("test");
    
    boolean finishFlag = false;
    while(!finishFlag) {
      int elSepIndex = line.indexOf(':');
      int nextIndex = line.indexOf(',');
      if(nextIndex < 0) {nextIndex = lastIndex; finishFlag = true;};
      String EKey = line.substring(currIndex, elSepIndex);
      EKey.trim();
      EKey = EKey.substring(1, EKey.length() - 1);
      
      String EValue = line.substring(elSepIndex + 1, nextIndex);
      EValue.trim();
      EValue = EValue.substring(1, EValue.length() - 1);
      line = line.substring(nextIndex + 1);
      
      Serial.print(EKey);
      Serial.print(":");
      Serial.println(EValue);
    }
    
  }
}

void setup()
{
  pinMode(DOOR_PIN, OUTPUT);
  pinMode(9, INPUT);
  
  digitalWrite(DOOR_PIN, HIGH);
  lcd.begin(16, 2);
  Serial.begin(115200);
  WiFi.begin(SSID);
  
  Serial.println("Connecting.");
  
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  Serial.println("Waiting for an IP address.");
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }
  
}

void checkRegisterDevice() {
  String request = String("GET ") + 
    (String)DEVICE_REGISTERED_API_ENDPOINT + "/" + (String)uniqId + " HTTP/1.1\r\n" + 
    "Host: " + HOST + "\r\n\r\n";
  client.print(request);
  delay(1000);
  Serial.println(request);
  
  while(client.available() > 0) {
    String result = client.readStringUntil('\n');
    Serial.println(result);

    if(result.indexOf("true") >= 0) {
      deviceRegistered = 1;
      break;
    } else if(result.indexOf("false") >= 0) {
      deviceRegistered = 0;
      //clientConnected = client.connect(HOST, 80);
      break;
    }
  }
  
}

void checkDoorOpened() {
  String request = String("GET ") + 
    (String)DOOR_REQUEST_API_ENDPOINT + "/" + (String)uniqId + " HTTP/1.1\r\n" + 
      "Host: " + HOST + "\r\n\r\n";
  client.print(request);
  delay(1000);
    Serial.println(request);
  
  while(client.available() > 0) {
    String result = client.readStringUntil('\n');
        Serial.println(result);
    if(result.indexOf("true") >= 0) {
      doorOpened = 1;
      break;
    } else if(result.indexOf("false") >= 0) {
      doorOpened = 0;
      break;
    }
  }
  
  delay(1000);
  
  request = String("GET ") + 
    (String)DEVICE_REGISTERED_API_ENDPOINT + "/" + (String)uniqId + " HTTP/1.1\r\n" + 
      "Host: " + HOST + "\r\n\r\n";
  client.print(request);
  delay(1000);
  while(client.available() > 0) {
    String result = client.readString();
    if(result.indexOf("true") >= 0) {
      deviceRegistered = 1;
      break;
    } else if(result.indexOf("false") >= 0) {
      deviceRegistered = 0;
      break;
    }
  }
}

void startAnimation() {
   
}

void openDoor() {
  digitalWrite(DOOR_PIN, LOW);
  delay(3000);
  digitalWrite(DOOR_PIN, HIGH);
}

void loop()
{
  if(!deviceRegistered) {
    reconnect();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your PIN is:");
    lcd.setCursor(0, 1);
    lcd.print(uniqId);
    checkRegisterDevice();
    delay(4000);
  }
  else if(!doorOpened) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WOW Door hazir!");
    
    for(int i = -15; i < 16; i++) {
          reconnect();
      lcd.setCursor(0, 1);
      lcd.print(static_anim[abs(i)]);
      checkDoorOpened();
      if(doorOpened) {
        break;
      }
      if(!deviceRegistered) {
        break;
      }
      delay(300);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WOW Door acik!");
    openDoor();
    delay(1000);
    doorOpened = 0;
  }
  
}

