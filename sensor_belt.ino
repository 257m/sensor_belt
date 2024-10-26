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

//defining pin numbers 
const int trigPin = 9;  
const int echoPin = 11;

//defining our variables 
long duration;//in long because the number is big, and not within the range of an integer
int distance; //distace will be in cm as an integer no decimals 

int final_distance() {
   digitalWrite(9,LOW); //setting the trigger pin to low 
  delayMicroseconds(3); //delay for 3 microseconds
  digitalWrite(9, HIGH); //setting the trigger pin to high 
  delayMicroseconds(100);//delay for 10 microseconds 
  duration = pulseIn(11,HIGH);//our duration is equal to the time our pin is HIGH (pulseIn)
  distance = duration*.034/2; //this calculates the distance 
  Serial.print("distance ="); //here the distance is printed based on the calculations above 
  Serial.println(distance); // what is the difference between print and println
  return distance;
}

// Setup everything to make the webserver work.
void setup(void) {
  delay(3000);  // wait for serial monitor to start completely.

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
    <link rel="stylesheet" href="style.css">
</head>
  <body>
)==") +
String(final_distance()) +
String(R"==(</body>
</html>
)=="));
  });
  // serve a built-in htm page
  server.on("/style.css", []() {
    server.send(200, "text/html", FPSTR(styleCss));
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

