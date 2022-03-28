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

#ifndef LAMP_HPP_PIERRE_
#define LAMP_HPP_PIERRE_

#include "Color.hpp"
#include "D1mini.hpp"
#include "load_save.hpp"




//--- HARDWARE INTERFACE ----
#define LED_R D1mini_D5
#define LED_G D1mini_D6
#define LED_B D1mini_D7

void set_rgb(const Color &a );
void set_rgb(byte r, byte g, byte b);




//--- LAMP ----
struct Lamp_persistant{
    typedef decltype( millis() )     millis_t;
    constexpr static size_t color_max=32;
    static inline Color dummy_color = Color(255,255,255);

    protected: // --- persistant data ---
    bool     is_on     = true;  
    millis_t period    = 1000;
    size_t   color_n   = 0;
    Color    colors[color_max];
};


struct Lamp: Lamp_persistant{
  
  typedef Lamp_persistant::millis_t    millis_t;

  //--- const ---
  size_t        size_color()const {return color_n;}
  const Color & get_color(size_t i)const;
  bool          get_on()const     {return is_on;}
  millis_t      get_period()const {return period;}

  //--- mutate ---
  void reset_colors(){if(color_n==0){return;}  color_n=0; something_changed();}
  
  void push_color(const Color &c){
    if(color_n+1 >=  color_max){return;}
    colors[color_n]=c;
    ++color_n;
    something_changed();
  }

  void run_loop(millis_t delta);

  void set_on    (bool b    ){if(is_on  == b){return;} is_on=b;  something_changed(); }
  void set_period(millis_t p){if(period == p){return;} period=p; something_changed(); }
  
  bool load(const char * path="/lamp")     {bool ok = ::load<Lamp_persistant>(*this,path); is_saved=true; return ok; }

  private:
  bool save(const char * path="/lamp")const{ is_saved=true; return ::save<Lamp_persistant>(*this,path);}
  millis_t delta_sum = 0;

  void something_changed(){is_saved=false; last_change_delta=0;  };
  mutable millis_t last_change_delta=0;
  mutable bool is_saved=true;
};


//--- helpers ---
//choose a color between x, and y
Color map_color(const Color &x, const Color &y, double change);





//--- IMPLEMENTATION ---
inline void Lamp::run_loop(millis_t delta){
  if(!is_on){ set_rgb(0,0,0);  return;}

  if(period==0){
    set_rgb(get_color(0));
    return;
  }

  delta_sum+=delta;
  delta_sum%=period;

  size_t color_index  = (color_n*delta_sum)/period;
  const Color & c1 = get_color(  color_index              );
  const Color & c2 = get_color(  (color_index +1)%color_n  );
  
  double a = static_cast<double>(delta_sum)/period;
  double av = fmod(a*color_n,1);
  set_rgb(map_color(c1,c2,av));

  //--- save if changed, and if the delay is long enough ---
  if(! is_saved){
    this->last_change_delta +=  delta;
    if(last_change_delta > 30*1000){
          this->save();
    }
  }
 

}


inline const Color &  Lamp::get_color(size_t i)const{
  if(i >= color_max){
    Serial.println("ERROR wrong color index");
    return dummy_color;
  }
  
  return colors[i];
}


/*
inline Color &  Lamp::get_color(size_t i){
  if(i >= color_max){
    Serial.println("ERROR wrong color index");
    return dummy_color;
  }
  
  color_n = max(color_n, i+1);
  return colors[i];
}
*/

inline void set_rgb(const Color &a ){
  static Color last_color;
  if(a == last_color){return;}
  last_color = a;

  static constexpr int x = D1mini_PWM_MAX/255;
  analogWrite(LED_R, a.r*x);
  analogWrite(LED_G, a.g*x) ;
  analogWrite(LED_B, a.b*x) ;

  //a.print();Serial.println();

  
}

inline void set_rgb(byte r, byte g, byte b){
  Color tmp(r,g,b);
  set_rgb(tmp);
}




inline Color map_color(const Color &x, const Color &y, double change){
  if(change >1){change=1;}
  if(change <0){change=0;}

  
  auto mc = [](double c1, double c2, double change)->double{
    return c1 + (  c2-c1 )*change;
  };

  Color result(
    mc(x.r, y.r, change) ,
    mc(x.g, y.g, change) ,
    mc(x.b, y.b, change) 
  );

  //Serial.print(change*255);Serial.print(" ");
  //result.print();
  //Serial.println("");
  
  return result;

   
  
}
#endif
