/* 
 * This file is part of the Lampe distribution (https://github.com/pierreblavy2).
 * Copyright (c) 2022 Pierre BLAVY.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Lamp.hpp"
#include "Wifi.hpp"

#include "html_index.hpp"
#include "ArduinoJson.h"
#include "Timer.hpp"


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

//--- SWITCH wires ---
#define SW1 D1mini_A0
#define SW2 D1mini_D1
#define SW3 D1mini_D2

#include "secret/wifi_login.hpp"
  //#define WIFI_SSID "xxxx"
  //#define WIFI_PASS "yyyy"

//--- data ---
Lamp lamp;
Wifi wifi;
ESP8266WebServer server(80);
decltype( millis() ) last_loop;

void fn_wifi_debug();
Timer wifi_debug(5000, fn_wifi_debug);


//--- web pages ---
//need server and lamp
#include "web_pages.hpp"


void setup() {
  //--- debug ---
  Serial.begin(9600);
  Serial.println("Setup");
 
  //---  wires ---
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);

  const Color tmp;
  analogWrite(LED_R, tmp.r);
  analogWrite(LED_G, tmp.g);
  analogWrite(LED_B, tmp.b);

  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);

  //--- data ---
  last_loop = millis();
  lamp.push_color(Color(255,0,0));
  lamp.push_color(Color(0,255,0));
  lamp.push_color(Color(0,0,255)); 
  


  //--- filesystem ---
  SPIFFSConfig filesystem_cfg;
  filesystem_cfg.setAutoFormat(true);
  SPIFFS.setConfig(filesystem_cfg);
  SPIFFS.begin();

  //--- load from filesystem ---
  lamp.load();
  wifi.load();
  Serial.print("MY_SSID (AP) :");Serial.println(wifi.get_my_ssid());
  Serial.print("SSID (ST) :");   Serial.println(wifi.get_ssid());

  
  //--- web pages & wifi ---
  //doc : https://siytek.com/esp8266-ap-and-station-mode/
  WiFi.mode(WIFI_AP_STA); //WIFI_AP_STA
  WiFi.softAP( wifi.get_my_ssid() , "magicword");//todo wifi.get_my_ssid()
  WiFi.begin(wifi.get_ssid()   , wifi.get_pass() );

  setup_web_pages();
  server.begin();


  //--- debug ---
  Serial.println("Setup OK");
}

void loop() {
   //compute delta
   auto now_v = millis();
   auto delta = now_v-last_loop;
   last_loop=now_v;

   //call run_loop
   lamp      .run_loop(delta);
   wifi_debug.run_loop(delta);

   //run server
   server.handleClient();
}



    

void fn_wifi_debug(){
  // Connected to WiFi
  Serial.println();
  Serial.print("Status : ");
  Serial.println( wifi_status_str( WiFi.status() ) );
  Serial.print("IP address ST : ");
  Serial.println(WiFi.localIP());
  Serial.print("IP address AP : ");
  Serial.println(WiFi.softAPIP());
}
