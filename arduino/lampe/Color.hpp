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


#ifndef COLOR_HPP_PIERRE_
#define COLOR_HPP_PIERRE_

#include <type_traits>

struct Color{
  byte r = 0;
  byte g = 0;
  byte b = 0;

  constexpr Color(){}
  constexpr Color(byte r_, byte g_, byte b_):r(r_),g(g_),b(b_){}
  explicit  Color(const char*s){from_str(s);}
  
  void print()const;

  void to_str(char* write_here)const{
    write_here[0]='#';
    write_here[1]=byte_to_char(r/16);
    write_here[2]=byte_to_char(r%16);
    write_here[3]=byte_to_char(g/16);
    write_here[4]=byte_to_char(g%16);
    write_here[5]=byte_to_char(b/16);
    write_here[6]=byte_to_char(b%16);
    write_here[7]='\0';
  }

  void from_str(const char* s){
    if(s[0]!='#'){return;}
    r = 16*char_to_byte(s[1]) + char_to_byte(s[2]);
    g = 16*char_to_byte(s[3]) + char_to_byte(s[4]);
    b = 16*char_to_byte(s[5]) + char_to_byte(s[6]);
  }

  static char byte_to_char(byte x);
  static byte char_to_byte(char s);


   
  
};

bool operator == (const Color &x, const Color &y );


//====


inline void Color::print()const{
    Serial.print("");
    Serial.print(r); Serial.print(" ");
    Serial.print(g); Serial.print(" ");
    Serial.print(b);
    Serial.print("");
}

inline bool operator == (const Color &x, const Color &y ){
  return x.r == y.r and
         x.g == y.g and
         x.b == y.b;
}


inline char Color::byte_to_char(byte x){
     switch(x){
     case 0 :return '0';
     case 1 :return '1';
     case 2 :return '2';
     case 3 :return '3';
     case 4 :return '4';
     case 5 :return '5';
     case 6 :return '6';
     case 7 :return '7';
     case 8 :return '8';
     case 9 :return '9';
     case 10 :return 'a';
     case 11 :return 'b';
     case 12 :return 'c';
     case 13 :return 'd';
     case 14 :return 'e';
     case 15 :return 'f';
     }
     return '0';
  }


inline byte Color::char_to_byte(char c){
     switch(c){
     case '0' :return 0;
     case '1' :return 1;
     case '2' :return 2;
     case '3' :return 3;
     case '4' :return 4;
     case '5' :return 5;
     case '6' :return 6;
     case '7' :return 7;
     case '8' :return 8;
     case '9' :return 9;
     case 'a' :return 10;
     case 'A' :return 10;
     case 'b' :return 11;
     case 'B' :return 11;
     case 'c' :return 12;
     case 'C' :return 12;
     case 'd' :return 13;
     case 'D' :return 13;
     case 'e' :return 14;
     case 'E' :return 14;
     case 'f' :return 15;
     case 'F' :return 15;
     }
     return '0';
  }

  
#endif
