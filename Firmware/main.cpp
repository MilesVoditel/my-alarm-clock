#include <Adafruit_GFX.h>      // core graphics library
#include <Adafruit_ST7789.h>   // driver for the ST7789 screen
#include <WiFi.h>
#include <time.h>
#include <SPI.h>

int alarmActive = 0; // variable to track if the alarm is active

// NOTE EDIT LATER ON WHEN USING AT HOME
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// NOTE EDIT LATER ON WHEN USING AT HOME
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000; // NOTE CHANGE LATER ON TO MATCH OWN TIMEZONE
const int   daylightOffset_sec = 3600; // NOTE CHANGE LATER ON TO MATCH WHETHER OR NOT DAYLIGHT SAVINGS IS USED, SET TO 0 IF NO DAYLIGHT SAVINGS

// --- Pin assignments (change these to match how YOU wired it) ---
#define TFT_CS   7
#define TFT_RST  8
#define TFT_DC   10
#define TFT_MOSI 20
#define TFT_SCLK 21
#define TFT_keyPin 1
#define TFT_keyPin 2
#define TFT_keyPin 3
#define TFT_keyPin 4
#define TFT_keyPin 5
#define TFT_Buzzer 6



// --- Objects live out here, outside any function ---
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// setup() runs ONCE when the board powers on
void setup() {
  Serial.begin(115200); // lets the board talk to your computer

  pinMode(keyPin, INPUT_PULLUP);
  tft.init(76, 284); // screen dimensions
  tft.setRotation(2); // orientation, could be different, try 0-3
  Serial.println("TFT Initialized!");

  tft.fillScreen(ST77XX_BLACK);  // clear the screen
  tft.setCursor(0, 0);           // top-left corner
  tft.setTextSize(2);            // a readable size
  tft.println("Connecting to Wi-Fi...");
  Serial.println("Connecting to Wi-Fi...");
  
  WiFi.begin(ssid, password); // starts the process to connect to wi-fi
  while (WiFi.status() != WL_CONNECTED) { // repeats until connected to wifi
    delay(500); // waits 500 milliseconds
    Serial.print(".");
  }

  tft.fillScreen(ST77XX_BLACK); // clears screen
  tft.setCursor(0, 0); // top-left corner
  tft.println("Connected to Wi-Fi");
  Serial.println("Connected to Wi-Fi");

  //syncs time with server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  tft.fillScreen(ST77XX_BLACK); // clears screen
  tft.setCursor(0, 0); // top-left corner
  tft.println("Syncing time...");
  Serial.println("Syncing time...");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) { // repeats until time is synced with server
    delay(500);
    Serial.print(".");
  }

  tft.fillScreen(ST77XX_BLACK); // clears screen
  tft.setCursor(0, 0); // top-left corner
  tft.println("Time synced");
  Serial.println("Time synced");
}

// loop() runs OVER and OVER, forever
void loop() {
  struct tm timeinfo; // declares local variable to store all stuff needed for time

  if (getLocalTime(&timeinfo)) { // gets current time from server
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 35);
    tft.printf("%02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
  if(alarmActive == 1) {
    delay(10);
  } else {
  delay(100); // wait a second before looping
  }
}

void alarm() {
  //trigger buzzer
  //generates random string of numbers between 1 and 5
  if (alarmActive == 1) {
    activateBuzzer();
  }

}

void disarmAlarm() {
  random(1, 5);
}

void activateBuzzer() {
  digitalWrite(TFT_Buzzer, HIGH); // triggers buzzer
  delay(100); // wait a second
  digitalWrite(TFT_Buzzer, LOW); // turns off buzzer
  delay(100); // wait a second
}