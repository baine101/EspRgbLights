#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#import "index.h"


char ssid[] = "MadHatters";          //  your network SSID (name) 
char pass[] = "Blink182";   // your network password
int red = 5;
int green = 4;
int blue = 0;



ESP8266WebServer server(80);

void fade(){

 String message = ""; 

    if(server.arg("r")== ""){
      message += "r Argument not found";
    }
    
    if(server.arg("g")== ""){
      message += "g Argument not found";
    }
     
    if(server.arg("b")== ""){
      message += "b Argument not found";
    }

    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();
    
    analogWrite(red, 0);
    analogWrite(green, 0);
    analogWrite(blue, 0);  


  while(r <= 1020){
    analogWrite(red, r);
    delay(100);
    r = r + 20;
  }

   while(r >= 0){
    analogWrite(red, r);
    delay(100);
    r = r - 20;
  }
  
  server.send(200, "text/html", "fade");
}




void root(){
   String message = ""; 
   String s = MAIN_page;

    if(server.arg("r")== ""){
      message += "r Argument not found";
    }
    
    if(server.arg("g")== ""){
      message += "g Argument not found";
    }
     
    if(server.arg("b")== ""){
      message += "b Argument not found";
    }

    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();

    analogWrite(red, r);
    analogWrite(green, g);
    analogWrite(blue, b);  

    message = "r=";
    message += r ;
    message += "&";
    message += "g=";
    message += g;
    message += "&";
    message += "b=";
    message += b;

   server.send(200, "text/html", s);
}

void initWifi(){
  Serial.printf("Connecting to %s ,%s", ssid , pass);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
}

void setup() {
  Serial.begin(115200);
  Serial.println();

 ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());



  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
  initWifi();
  
  server.begin();
  server.on("/", root); 
  server.on("/fade", fade); 

}



void loop() {
   ArduinoOTA.handle();
    server.handleClient();   
}
