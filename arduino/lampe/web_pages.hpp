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

#ifndef WEB_PAGES_HPP_
#define WEB_PAGES_HPP_

//--- wifi status ---
const char* wifi_status_str( decltype(WL_CONNECTED) );

//--- web pages ---
void get_index();
void get_lamp();
void get_wifi();
void get_not_found();
void post_lamp();
void post_wifi();

void setup_web_pages(){
  server.on("/"          , get_index);
  server.on("/get_lamp"  , get_lamp );
  server.on("/get_wifi"  , get_wifi );
  server.on("/post_lamp" , post_lamp);
  server.on("/post_wifi" , post_wifi);
  server.onNotFound(       get_not_found);
}




//===========
//=== GET ===
//===========

inline void get_index() {
  Serial.println("get_index");
  server.send(200, "text/html", html_index);
}


inline void get_lamp() {
  Serial.println("get_lamp");

  DynamicJsonDocument doc(1024);
  doc["on"]    =lamp.get_on() ;
  doc["period"]=lamp.get_period();
  for(size_t i=0; i < lamp.size_color() ; ++i){
    char color_str[8];
    lamp.get_color(i).to_str(color_str);
    doc["colors"][i]=color_str;
  }

  char buf[1024];
  serializeJson(doc, static_cast<void*>(buf), 1024);

  server.send(200, "application/json", buf);
}


inline void get_wifi() {
  
  Serial.println("get_wifi");
  DynamicJsonDocument doc(1024);
  doc["ssid"]    = wifi.get_ssid();    
  doc["my_ssid"] = wifi.get_my_ssid();
  doc["status"]  = "not_connected";
  doc["status"]=wifi_status_str(WiFi.status());


  char buf[2048];
  serializeJson(doc, static_cast<void*>(buf), 2048);
  server.send(200, "application/json", buf);
}




//============
//=== POST ===
//============


inline void post_lamp() {
  Serial.println("post_lamp");
  Serial.print("arg:");
  Serial.println( server.arg("plain") );
  
  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, server.arg("plain") );
  if(err != DeserializationError::Ok ){
      Serial.println("ERROR deserializeJson");
      return;
  }

  if(!doc.containsKey("on"))    {Serial.println("missing on in json"); return;}
  if(!doc.containsKey("period")){Serial.println("missing period in json"); return;}
  if(!doc.containsKey("colors")){Serial.println("missing colors in json"); return;}

  lamp.set_on( doc["on"].as<bool>() );
  lamp.set_period( doc["period"].as<Lamp::millis_t>() );
  const auto & colors = doc["colors"].as<JsonArray>();

  lamp.reset_colors();
  for(size_t i =0; i <  min(lamp.color_max ,colors.size())  ; ++i){
     lamp.push_color( Color( colors[i].as<const char*>()) );
  }
}


inline void post_wifi() {
  Serial.println("post_wifi");
  Serial.print("arg:");
  Serial.println( server.arg("plain") );
  
  
  DynamicJsonDocument doc(1024);
  DeserializationError err = deserializeJson(doc, server.arg("plain") );
  if(err != DeserializationError::Ok ){
      Serial.println("ERROR deserializeJson");
      return;
  }


  auto get_json=[](const DynamicJsonDocument &d, const char*key, const char* default_v){
    if(d.containsKey(key)){return d[key].as<const char *>();}
    return default_v;
  };

  wifi.set_ssid    ( get_json(doc, "ssid"   , "") );
  wifi.set_pass    ( get_json(doc, "pass"   , "") );
  wifi.set_my_ssid ( get_json(doc, "my_ssid", "") );

  wifi.save();
  

  //WARNING : to specify a default value of v : doc["xxx"] || v;
  //          the documentation says to use | but it's actually ||
  
}


inline void get_not_found() {
  Serial.println("get_not_found");

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
}



const char* wifi_status_str(decltype(WL_CONNECTED) i ){
  switch(i){
     case WL_CONNECTED:       return "WL_CONNECTED";       // assigned when connected to a WiFi network; 
     case WL_NO_SHIELD:       return "WL_NO_SHIELD";       // assigned when no WiFi shield is present; 
     case WL_IDLE_STATUS:     return "WL_IDLE_STATUS";     // it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED); 
     case WL_NO_SSID_AVAIL:   return "WL_NO_SSID_AVAIL";   // assigned when no SSID are available; 
     case WL_SCAN_COMPLETED:  return "WL_SCAN_COMPLETED";  // assigned when the scan networks is completed; 
     case WL_CONNECT_FAILED:  return "WL_CONNECT_FAILED";  // assigned when the connection fails for all the attempts; 
     case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST"; // assigned when the connection is lost; 
     case WL_DISCONNECTED:    return "WL_DISCONNECTED";    // assigned when disconnected from a network; 
  }
  return "INVALID STATUS";  
}



#endif
