// @file WebServer.ino
// @brief Example WebServer implementation using the ESP32 WebServer
// and most common use cases related to web servers.
//
// * Setup a web server
// * redirect when accessing the url with servername only
// * get real time by using builtin NTP functionality
// * send HTML responses from Sketch (see builtinfiles.h)
// * use a LittleFS file system on the data partition for static files
// * use http ETag Header for client side caching of static files
// * use custom ETag calculation for static files
// * extended FileServerHandler for uploading and deleting static files
// * extended FileServerHandler for uploading and deleting static files
// * serve APIs using REST services (/api/list, /api/sysinfo)
// * define HTML response when no file/api/handler was found
//
// See also README.md for instructions and hints.
//
// Please use the following Arduino IDE configuration
//
// * Board: "ESP32 Dev Module" or other board with ESP32
// * Partition Scheme: "Default 4MB with spiffs" or any other scheme with spiffs or FAT
//     but LittleFS will be used in the partition (not SPIFFS)
// * other setting as applicable
//
// Changelog:
// 21.07.2021 creation, first version
// 08.01.2023 ESP32 version with ETag
// 02.08.2024 support LitteFS and FAT file systems (on large Flash chips)

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

#include "secrets.h"  // add WLAN Credentials in here.

#include "esp_partition.h"  // to check existing data partitions in Flash memory

#include <FS.h>        // File System for Web Server Files
#include <LittleFS.h>  // Use LittleFSThis file system is used.
#include <FFat.h>      // or.. FAT

// mark parameters not used in example
#define UNUSED __attribute__((unused))

// TRACE output simplified, can be deactivated here
#define TRACE(...) Serial.printf(__VA_ARGS__)

// name of the server. You reach it using http://webserver
#define HOSTNAME "webserver"

// need a WebServer for http access on port 80.
WebServer server(80);

// The text of builtin files are in this header file
#include "builtinfiles.h"

const int buzzer = 6;
const int trig_pin = 9;
const int echo_pin = 11;
float timing = 0.0;
float distance = 0.0;

#include "components.h"

int distance_buzz(int distance) {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  timing = pulseIn(echo_pin, HIGH);

  distance = timing * (0.034 / 2);

  if (distance <= 50) {
    tone(buzzer,500 + distance, distance*50);
    delay(distance*5);
    noTone(buzzer);
    delay(distance*5);

  }
  else if (distance <= 200) {
    tone(buzzer,500 + distance,disatnce*50);
    delay(distance*5);
    tone(buzzer, 600);
    delay(distance*5);
  }
  else {
    noTone(buzzer);
  }
}

// Setup everything to make the webserver work.
void setup(void) {
  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(trig_pin, LOW);
  digitalWrite(buzzer, LOW);

  // Use Serial port for some trace information from the example
  Serial.begin(9600);
  
  // allow to address the device by the given name e.g. http://webserver
  WiFi.setHostname(HOSTNAME);

  // start WiFI
  Serial.print("Setting AP (Access Point)… ");
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(ssid, passPhrase);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address of the router of this network: ");
  Serial.println(IP);
  Serial.print("ESP32 IP as soft AP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());

  server.begin();

  // serve a built-in htm page
  server.on("/", []() {
    server.send(200, "text/html", 
  String(R"==(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>First Website</title>
    <style>
    )==") + dashboardCSS + speedometerCSS + String("</style>") + speedometerJS +
String(R"==(</head>
  <body>)==") + dashboard + speedometer() +
String(R"==(
</body>
</head>
</html>
)=="));
  });

server.on("/settings.html", []() {
    server.send(200, "text/html", 
  String(R"==(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>First Website</title>
    <style>)==") + dashboardCSS + settingsJS +
    String(R"==(</style>
</head>
<body>)==") + dashboard + settings + String(R"==(</body>
</html>
)=="));
});
   

  server.onNotFound([]() {
    // standard not found in browser.
    server.send(404, "text/html", FPSTR(notFoundContent));
  });

  server.begin();

  TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());
}  // setup

void loop(void) {
  server.handleClient();
}
