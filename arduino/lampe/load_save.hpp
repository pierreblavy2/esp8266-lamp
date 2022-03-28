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

#ifndef LOAD_SAVE_PIERRE_HPP_
#define LOAD_SAVE_PIERRE_HPP_

#include <LittleFS.h>

template<typename T>  bool load(T&load_here    , const char * path);
template<typename T>  bool save(const T&save_me, const char * path);



//=== Impl ===
template<typename T>
bool load(T&load_here, const char * path){

  static_assert(std::is_standard_layout<T>::value ,"");

  File f2 = SPIFFS.open(path, "r" );
  
  if (!f2) {
    Serial.print("read file open failed : ");
    Serial.println(path);
    f2.close();
    return false;
  }

  //check size
  const auto n = f2.available();
  if( n!=sizeof(T) ){ 
    Serial.println("read file wrong size"); 
    f2.close();
    return false;
  }
  
  f2.read( reinterpret_cast<uint8_t*>(&load_here) ,n);
  f2.close();
  return true;
}


template<typename T>
bool save(const T&save_me, const char * path){
  Serial.print("save : ");
  Serial.println(path);
  
  File f = SPIFFS.open(path, "w" );
  if (!f) {
    Serial.print("write file open failed : ");
    Serial.println(path);
    f.close();
    return false;
  }

  f.write( reinterpret_cast<const uint8_t*>(&save_me) ,  sizeof(T) );
  f.close();
  return true;
}

#endif
