#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SSD1306.h"
#include "OLEDDisplayUi.h"
#include "images.h"
#include "font.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Image Header.h"

const char* ssid = "xxxxx";
const char* password = "xxxxx";

const String endPoint = "http://api.openweathermap.org/data/2.5/weather?q=Hong Kong Special Administrative Region,HK&units=metric&APPID=";
const String key = "your key";

//address -> 0x3c
//SDA -> 19
//SDC -> 18
//SSD1306Wire display(ADDRESS, SDA, SDC);
//SSD1306 display(0x3c, 5, 4);
SSD1306 display(0x3c, 21, 22);
OLEDDisplayUi ui (&display);

/* 
 *  ///default font ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
  //http://oleddisplay.squix.ch/#/home//
 Lato_Bold_10
 Lato_Regular_10
 Lato_Regular_12
 Lato_Bold_12
 Lato_Bold_14
 Lato_Regular_14 
 Lato_Bold_20
 */

//string variable to store JSON varaible
String cityStr;
String temperatureStr, temperature_minStr, temperature_maxStr;
String pressureStr;
String riseStr, setStr;
String windSpeedStr, windDegStr;
String dtStr;
String humidityStr;
String weatherDescriptionStr, weatherIconStr;

//screen size
int screenW = 128;
int screenH = 64;


//define an NTP Client to request date and time from an NTP server.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//String variables to save the date and time.
String formattedDate;
String dayStamp;
String timeStamp;

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
//  display->setTextAlignment(TEXT_ALIGN_CENTER); 
//  display->setFont(ArialMT_Plain_16);
//  display->drawString(128,0,"APPLE");
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
 // display->drawXbm(50 + x, 0 + y, bubble_width, bubble_height, bubble_bits);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(Lato_Bold_10);
 
  //int textWidth = display->getStringWidth(dayStamp);
  display->drawString(64 + x, 0 + y, dayStamp);
  display->setFont(Lato_Bold_20);
  timeStamp = timeClient.getFormattedTime();
  //textWidth = display->getStringWidth(timeStamp);
  display->drawString(64 + x, 25 + y, timeStamp);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
 
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
   //weather icon , location, weatherDescription
  if(weatherIconStr == "01d" || weatherIconStr == "01n"){
    display->drawXbm(0 + x, 0 + y, Logo_01d_width, Logo_01d_height, Logo_01d_bits);
  }
  if(weatherIconStr == "02d" || weatherIconStr == "02n"){
    display->drawXbm(0 + x, 0 + y, Logo_02d_width, Logo_02d_height, Logo_02d_bits);
  }
  if(weatherIconStr == "03d" || weatherIconStr == "03n"){
    display->drawXbm(0 + x, 0 + y, Logo_03d_width, Logo_03d_height, Logo_03d_bits);
  }
  if(weatherIconStr == "04d" || weatherIconStr == "04n"){
    display->drawXbm(0 + x, 0 + y, Logo_03d_width, Logo_03d_height, Logo_03d_bits);
  }
  if(weatherIconStr == "09d" || weatherIconStr == "09n"){
    display->drawXbm(0 + x, 0 + y, Logo_9d_width, Logo_9d_height, Logo_9d_bits);
  }
  if(weatherIconStr == "10d" || weatherIconStr == "10n"){
    display->drawXbm(0 + x, 0 + y, Logo_10d_width, Logo_10d_height, Logo_10d_bits);
  }
  if(weatherIconStr == "11d" || weatherIconStr == "11n"){
    display->drawXbm(0 + x, 0 + y, Logo_11d_width, Logo_11d_height, Logo_11d_bits);
  }
  if(weatherIconStr == "13d" || weatherIconStr == "13n"){
    display->drawXbm(0 + x, 0 + y, Logo_13d_width, Logo_13d_height, Logo_13d_bits);
  }   
  if(weatherIconStr == "50d" || weatherIconStr == "50n"){
    display->drawXbm(0 + x, 0 + y, Logo_50d_width, Logo_50d_height, Logo_50d_bits);
  }       
  display->setTextAlignment(TEXT_ALIGN_LEFT); 
  display->setFont(Lato_Bold_14);
  display->drawString(55 + x,10 + y , cityStr);
  display->setFont(Lato_Bold_12);
  display->drawString(55 + x,30 + y, weatherDescriptionStr);
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  //weather icon , location, wether

  // The coordinates define the left starting point of the text
 // display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawXbm(0 + x, 0 + y, Logo_temperature_width, Logo_temperature_height, Logo_temperature_bits);
  display->setFont(Lato_Bold_14);
  display->drawString(55 + x, 0 + y, "Temp");
  display->setFont(Lato_Bold_14);
  display->drawString(55 + x,20 + y, temperatureStr + "°C");
  display->setFont(Lato_Regular_10);
  display->drawString(55+ x, 35 + y, temperature_minStr + "°C / " + temperature_maxStr + "°C");

}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
 
 // display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(Lato_Bold_10);
  display->drawString(5 + x, 0 + y, "Humidity");
  display->drawString(70 + x, 0 + y, "Pressure");
  display->drawString(5 + x, 28 + y, "Wind speed");

  display->setFont(Lato_Regular_10);
  display->drawString(5 + x, 12 + y, humidityStr + " %");
  display->drawString(70 + x, 12 + y, pressureStr + " hPa");
  display->drawString(5 + x, 40 + y, windSpeedStr + " m/s");
}

FrameCallback frames[] = {drawFrame1, drawFrame2, drawFrame3, drawFrame4};

int frameCount = 4;

OverlayCallback overlays[] = {msOverlay};
int overlaysCount = 1;

unsigned long previousMillis = 0;
const long interval = 10000;




void setupUI(){
  ui.setTargetFPS(60);
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui.setIndicatorPosition(BOTTOM);
  ui.setIndicatorDirection(LEFT_RIGHT);
//  ui.setTimePerTransition(0.1);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, frameCount);
  ui.setOverlays(overlays, overlaysCount);
  ui.init();
  display.flipScreenVertically();
}

void setup() {
  int i;
//  Serial.begin(921600);
  Serial.begin(115200);
  setupUI();
  
  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(Lato_Regular_12);
  display.drawString(64, 14, "connecting to Wifi");
  display.display();
  
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
   delay(500);
  Serial.println("Connecting to WiFi..");
  display.drawString(20 + i, 30, ".");
  i += 5;
  display.display();
  }
  //initialize the NTP client to get date and time from an NTP server.
  timeClient.begin();
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  // Set offset time in seconds to adjust for your timezone, HK is +8.
  timeClient.setTimeOffset(28800);
  
  Serial.println("Connected to the WiFi network");
  delay(2000);
  
}

void getData(){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    //specify URL
    http.begin(endPoint + key);
    //make request
    int httpCode = http.GET();

    if(httpCode > 0){
      //check for returning data
      String payload = http.getString();
      //Serial.println(httpCode);
      Serial.println(payload);
      //parse JSON
      //https://arduinojson.org/v5/assistant/
     const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 976;
     DynamicJsonBuffer jsonBuffer(bufferSize);
     JsonObject& root = jsonBuffer.parseObject(payload);
  //   Serial.println(root);
    

    const char* city = root["sys"]["country"];
    const char* temperature = root["main"]["temp"];
    const char* temperature_min = root["main"]["temp_min"];
    const char* temperature_max = root["main"]["temp_max"];
    const char* pressure = root["main"]["pressure"];
    const char* windSpeed = root["wind"]["speed"];
    const char* humidity = root["main"]["humidity"];
    const char* weatherDescription = root["weather"][0]["description"];
    const char* weatherIcon = root["weather"][0]["icon"];

    cityStr = city;  
    
    String tempT = temperature;
    int splitDot = tempT.indexOf(".");
    temperatureStr = tempT.substring(0,splitDot);

    String tempMin = temperature_min;
    int splitDot_Min = tempMin.indexOf(".");
    temperature_minStr = tempMin.substring(0,splitDot_Min);

    String tempMax = temperature_max;
    int splitDot_Max = tempMax.indexOf(".");
    temperature_maxStr = tempMax.substring(0,splitDot_Max);

    String tempHum = humidity;
    int splitDot_Hum = tempHum.indexOf(".");
    humidityStr = tempHum.substring(0,splitDot_Hum);
    
    pressureStr = pressure;
      Serial.println(pressureStr);
   
    windSpeedStr = windSpeed;
      Serial.println(windSpeedStr);
    weatherDescriptionStr = weatherDescription;
      Serial.println(weatherDescriptionStr);
    weatherIconStr = weatherIcon;
      Serial.println(weatherIconStr);
     
    }else{
      Serial.println("Error");
    }
    //free resource
    http.end();
  }
  //delay(6000);
}


void loop() {
   
   unsigned long currentMillis = millis();
   
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
     getData();
   // Serial.print();
  }
  //get a valid date and time:
  while(!timeClient.update()){
    timeClient.forceUpdate();
  }
  //convert the date and time to a readable format with the getFormattedDate()
  formattedDate = timeClient.getFormattedDate(); //the result : 2018-12-31T16:47:24Z
  
  //extract date and time...trim T and Z
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0,splitT);
  Serial.print("Date: ");
  Serial.println(dayStamp);

  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("Hour:");
  Serial.println(timeStamp); 
  //delay(200); 

  
  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    delay(remainingTimeBudget);
  }

}
