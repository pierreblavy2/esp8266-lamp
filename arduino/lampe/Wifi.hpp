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

#ifndef WIFI_PIERRE_HPP_
#define WIFI_PIERRE_HPP_

//--- LAMP ----
struct Wifi_persistant{
    Wifi_persistant(){
      pass[0]='\0';
      ssid[0]='\0';
      my_ssid[0]='\0';
    }

    typedef decltype( millis() ) millis_t;

    //don't forget trailing '\0'
    constexpr static size_t ssid_max=33;
    constexpr static size_t pass_max=64;

    typedef char ssid_t[ssid_max];
    typedef char pass_t[pass_max];

    protected: // --- persistant data ---
    ssid_t ssid; //station ssid
    pass_t pass; //station password
    ssid_t my_ssid; //AP ssid
};


struct Wifi:Wifi_persistant{

    bool load(const char * path="/wifi")     {
      bool ok = ::load<Wifi_persistant>(*this,path); 

      is_saved=true; 
      return ok; 
    }

    void set_pass   (const char* p){set_string(pass   ,p,pass_max);}
    void set_ssid   (const char* p){set_string(ssid   ,p,ssid_max);}
    void set_my_ssid(const char* p){set_string(my_ssid,p,ssid_max);}

    const char * get_pass()   const{return pass;}
    const char * get_ssid()   const{return ssid;}
    const char * get_my_ssid()const{if(my_ssid==""){return "lamp";}else{return my_ssid;} }

    bool save(const char * path="/wifi")const{
      Serial.println("save_wifi");

      if(is_saved){return true;}
      is_saved=true; 
      return ::save<Wifi_persistant>(*this,path);
    }
    
    private:
    mutable bool is_saved=true;

    void set_string (char*dest, const char* source, size_t dest_max){
      //strncmp returns 0 if strings are the same
      if( strncmp(source,""  ,dest_max)==0 ){return;} //ignore empty strings
      if( strncmp(dest,source,dest_max)==0 ){return;} //ignore if no change
      strncpy (dest, source ,dest_max-1); dest[dest_max-1]='\0';
      is_saved=false;
    }
    
};




#endif
