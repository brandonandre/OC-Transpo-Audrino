// Created by Brandon Andre
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

// Settings

// WiFi Hostname
const char* ssid     = "wifi_hostname";
// WiFi Password
const char* password = "wifi_password";

/*
  Here is the API call that needs to be made. Change all of the following options to your preferences;
  - API App Id Code
  - API Key Code
  - Route Number (Example 118)
  - Stop Number (Example 2954)
*/
String url = "/v1.2/GetNextTripsForStop?appID=API_APP_ID_HERE0&apiKey=API_KEY_HERE&routeNo=118&stopNo=2954";


// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
 
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
 
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

const char* host = "api.octranspo1.com";

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done

  display.display();

  delay(2000);

  // Clear the buffer.
  
  pinMode(BUILTIN_LED, OUTPUT);
  
  Serial.begin(155200);

  //WiFi.mode(WIFI_OFF);
  
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(WHITE);
  display.setCursor(40, 10);
  display.println("WiFi");
  display.setCursor(40, 20);
  display.println("Setup...");

  display.fillRect(0, 0, 33, 17, WHITE);
  
  display.display();

  WiFi.begin(ssid, password);

  int retry = 0;

  while (WiFi.status() != WL_CONNECTED) {
    retry++;
    delay(1000);
    Serial.print(".");

    if (retry > 15){
        display.clearDisplay();
        display.setTextSize(0.5);
        display.setTextColor(WHITE);
        display.setCursor(40, 10);
        display.println("Timeout");
        display.setCursor(40, 20);
        display.println(retry);
        display.display();
    }
    
  }

  Serial.println("");

  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(WHITE);
  display.setCursor(40, 10);
  display.println("Connected");
  display.setCursor(40, 20);
  display.println("");
  display.display();
  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(1000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond:");
  Serial.println(client.available());
  String whole;
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    whole += line;
  }

  Serial.println(whole);

  String response = xmlTakeParam(whole, "AdjustedScheduleTime");


  int howFar = response.toInt();
  Serial.println("Bus is currently: ");
  Serial.print(howFar);
  
  Serial.println();
  
  bool close = false;

    uint8_t color = 1;
    
    display.clearDisplay();

    display.fillRect(0, 0, 130, 17, color%2);
    display.setTextSize(0.5);
    display.setTextColor(BLACK);
    display.setCursor(44, 9);
    display.println("KANATA");
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(40, 18);
    display.println(howFar);

    //display.fillRect(0, 0, 130, 17, color%2);

    display.setTextSize(1);
    display.setCursor(70, 18);
    display.println("mins");
    
    display.display();
  

  if(howFar < 7 and howFar > 3){

    Serial.print("Under 7 minutes away.");
    
    digitalWrite(BUILTIN_LED, LOW);
  } else if (howFar > 7){



    Serial.print("Over 7 minutes away.");
    
    digitalWrite(BUILTIN_LED, HIGH);
  } else if (howFar < 4) {

    Serial.print("Very close!");


    
    close = true;
    for (int i=0; i<30; i++){
      digitalWrite(BUILTIN_LED, HIGH);
      delay(1000);
      digitalWrite(BUILTIN_LED, LOW);
      delay(1000);
    }
  }

  Serial.println();
  Serial.println("closing connection");

  if (!close) {
    delay(1000 * 59);
  }
    
}

String xmlTakeParam(String inStr,String needParam)
{
  if(inStr.indexOf("<"+needParam+">")>0){
     int CountChar=needParam.length();
     int indexStart=inStr.indexOf("<"+needParam+">");
     int indexStop= inStr.indexOf("</"+needParam+">");  
     return inStr.substring(indexStart+CountChar+2, indexStop);
  }
  return "not found";
}



