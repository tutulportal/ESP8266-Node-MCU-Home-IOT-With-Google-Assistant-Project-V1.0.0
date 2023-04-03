#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<ArduinoJson.h>
#include <WiFiClient.h>

WiFiClient wifiClient;

// define led pin
const int ledPin =  5;

// Wifi Credintials
const char* SSID = "SSID";
const char* Password = "Password";

// wifi connection function
void connectWifiNetwork(){
  WiFi.begin(SSID, Password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to Your Wifi ...");
  }
}

// get api response function
void requestAPI(){
  // Declare http client reference
  HTTPClient http;

  // Create HTTP Request
  http.begin(wifiClient,"your api here"); // place get method api response from server here

  // get status code
  int httpCode = http.GET();

  // check if httpCode == 200
  if(httpCode == HTTP_CODE_OK){
    Serial.print("HTTP Status Code: ");
    Serial.print(httpCode);
    Serial.println();

    //print response
    Serial.println(http.getString());
    parseJSON(http.getString());

    
  }

  //close connection
  http.end();
}

// after getting api response parse it into JSON function
void parseJSON(String json){
  
  //declare the buffer
  DynamicJsonDocument buffer(1024);

  //Deserialize the string => object
  deserializeJson(buffer, json);

  //get Message field
  const char* message = buffer["message"];
  int ledStatus = buffer["data"]["status"];

  //Print Response
  Serial.println();
  Serial.print("Response is : ");
  Serial.print(message);
  Serial.println();
  Serial.print("Led 1 Status is: "); // you can change to anything
  Serial.print(ledStatus); // here led status is the api data led status e.g: 0 means off, 1 means on

  // for turn on or off led based on api status response
  if(ledStatus == 0){
    digitalWrite(ledPin, LOW);
  }else{
    digitalWrite(ledPin, HIGH);
  }

}

// initial setup
void setup() {

  // setup pinMode
  pinMode(ledPin, OUTPUT);

  // begin serial at 115200 port
  Serial.begin(115200);

  //setup wifi
  connectWifiNetwork();

}

// start the loop
void loop() {
  
  // check connection and start the loop
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Successfully Connected to Your Wifi Network.");

    requestAPI();
    delay(2000);
  }

}
