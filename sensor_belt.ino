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
#include "components.h"

//defining pin numbers 
const int trigPin = 2;  
const int echoPin = 4;


//defining our variables 
long duration;//in long because the number is big, and not within the range of an integer
int distance; //distace will be in cm as an integer no decimals 

int final_distance() {
   digitalWrite(2,LOW); //setting the trigger pin to low 
  delayMicroseconds(3); //delay for 3 microseconds
  digitalWrite(2, HIGH); //setting the trigger pin to high 
  delayMicroseconds(100);//delay for 10 microseconds 
  duration = pulseIn(4,HIGH);//our duration is equal to the time our pin is HIGH (pulseIn)
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
    <script>
      $(document).ready(function() {
            let speed = 0;
            let maxSpeed = 200;

            setInterval(function() {
                speed += Math.floor(Math.random() * 20) + 1;
                if (speed > maxSpeed) {
                    speed = maxSpeed;
                }

                let rotation = (speed / maxSpeed) * 180;
                $('.needle').css('transform', 'translate(-50%, 0) rotate(' + rotation + 'deg)');
                $('.speed').text(speed);
            }, 1000);
        });
    </script>
    <style>
      #speedometer {
            width: 200px;
            height: 200px;
            border-radius: 50%;
            background-color: #f5f5f5;
            display: flex;
            justify-content: center;
            align-items: center;
            position: relative;
      }
 
      #speedometer .needle {
            width: 2px;
            height: 80px;
            background-color: red;
            position: absolute;
            bottom: 50%;
            left: 50%;
            transform-origin: bottom center;
            transform: translate(-50%, 0) rotate(0deg);
            transition: transform 0.5s;
      }
 
      #speedometer .speed {
            font-size: 24px;
            font-weight: bold;
            margin-top: 20px;
      }
    </style>
</head>
  <body>  
  <!-- Speedometer -->
  <div class="text-center" id="speedometer">
   <div class="needle">
   </div>
   <div class="speed">
    )==") + String(distance) + String(R"==(
   </div>
  </body>
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
    <link rel="stylesheet" href="styles.css">
    <script src="settings.js"></script>
</head>
<body>)==") + dashboard + R"==(</body>
</html>
)=="));
});
   
  // serve a built-in htm page
  server.on("/style.css", []() {
    server.send(200, "text/css", 
  String(R"==(
*{
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    font-family:'Kumbh Sans', sans-serif;
}

.navbar{
    background: #131313;
    height: 80px;
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 1.2rem;
    position: sticky;
    top: 0;
    z-index: 999;
}



.navbar__container{
    display: flex;
    justify-content: space-between;
    height: 80px;
    z-index: 1;
    width: 100%;
    max-width: 1300px;
    margin: 0 auto;
    padding: 0 50px;
}

#navbar__logo{
    background-color: #ff1877;
    background-image: linear-gradient(to top, #ff0844, #ffb199
    100%);
    -webkit-background-clip: text;
    -moz-background-clip: text;
    -webkit-text-fill-color: transparent;
    -moz-text-fill-color: transparent;
    -moz-text-fill-color: transparent;
    display: flex;
    align-items: center;
    cursor: pointer;
    text-decoration: none;
    font-size: 2rem;
}


.fa-gem {
    margin-right: flex;
}

.navbar__menu {
    display: flex;
    align-items: cenetr;
    list-style: none;
    text-align: center;
}


.navbar__item{
    height: 80px;
}


.navbar__links {
    color: #fff;
    display: flex;
    align-items: center;
    justify-content: center;
    text-decoration: none;
    padding: 0 1rem;
    height: 100%;
}

.navbar__btn{
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 0 1rem;
    width: 100%;
}

.button {
    display: flex;
    justify-content: center;
    align-items: center;
    text-decoration: none;
    padding: 10px 20px;
    height: 100%;
    width: 100%;
    border: none;
    outline: none;
    border-radius: 4px;
    background: #f77062;
    color: #fff;

}

.button:hover{
    background: #4837ff;
    transform: all 0.3s ease;

}


.navbar__links:hover{
    color: #f77062;
    transform: all 0.3s ease;
}

@media screen and(max-width: 960px){
    .navbar__container{
        display: flex;
        justify-content: space-between;
        height: 80px;
        z-index: 1;
        width: 100%;
        max-width: 1300px;
        padding: 0;
    }


    .navbar__menu{
        display: grid;
        grid-template-columns: auto;
        margin: 0;
        width: 100%;
        position: absolute;
        /* top: -1000px;
        opacity: 0; */
        transition: all 0.5s ease;
        height: 50vh;
        z-index: -1;
    }

    .navbar__menu.active{
        background: #131313;
        top: 100%;
        opacity: 1;
        transition: all 0.5s ease;
        z-index: 99;
        height: 50vh;
        font-size: 1.6rem;
    }
    
    #navbar__logo{
        padding-left: 25px;
    }

    .navbar__toggle .bar{
        width: 25px;
        height: 3px;
        margin: 5px auto;
        transition: all 0.3s ease-in-out;
        background: #fff;
    }

    .navbar__item{
        width: 100%;
    }

    .navbar__links {
        text-align: center;
        padding: 2rem;
        width: 100%;
        display: table;
    }

    #mobile-menu{
        position: absolute;
        top: 20%;
        right: 5%;
        transform: translate(5%, 20%);

    }

    .navbar__btn{
        padding-bottom: 2rem;

    }

    .button {
        display: flex;
        justify-content: center;
        
    }
}

#settings_div {
    margin-top: 35px;
    margin-left: 35%;
    margin-right: 35%;
}

#settings_title{
    margin-bottom: 20px;
}
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
  final_distance();
}

