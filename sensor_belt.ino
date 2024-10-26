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

// local time zone definition (Berlin)
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

// need a WebServer for http access on port 80.
WebServer server(80);

// The file system in use...
fs::FS *fsys = nullptr;

// The text of builtin files are in this header file
#include "builtinfiles.h"

// enable the CUSTOM_ETAG_CALC to enable calculation of ETags by a custom function
#define CUSTOM_ETAG_CALC

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
    server.send(200, "text/html", FPSTR(homePage));
  });


  TRACE("Register file system handlers...\n");

  // UPLOAD and DELETE of files in the file system using a request handler.
  server.addHandler(new FileServerHandler());

  // enable CORS header in webserver results
  server.enableCORS(true);

  // enable ETAG header in webserver results (used by serveStatic handler)
#if defined(CUSTOM_ETAG_CALC)
  // This is a fast custom eTag generator. It returns a value based on the time the file was updated like
  // ETag: 63bbceb5
  server.enableETag(true, [](FS &fs, const String &path) -> String {
    File f = fs.open(path, "r");
    String eTag = String(f.getLastWrite(), 16);  // use file modification timestamp to create ETag
    f.close();
    return (eTag);
  });

#else
  // enable standard ETAG calculation using md5 checksum of file content.
  server.enableETag(true);
#endif

  // serve all static files
  server.serveStatic("/", *fsys, "/");

  TRACE("Register default (not found) answer...\n");

  // handle cases when file is not found
  server.onNotFound([]() {
    // standard not found in browser.
    server.send(404, "text/html", FPSTR(notFoundContent));
  });

  server.begin();

  TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());
}  // setup

// run the server...
void loop(void) {
  server.handleClient();
}  // loop()

// end.
