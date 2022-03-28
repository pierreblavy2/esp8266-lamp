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

#ifndef TIMER_PIERRE_HPP_
#define TIMER_PIERRE_HPP_

struct Timer{
  typedef decltype(millis() ) millis_t;  
  
  Timer(){};
  
  template<typename Fn> explicit Timer(millis_t ms, Fn fn):run_me(fn), intervall(ms){}

  millis_t intervall = 0;
  millis_t sum_delta = 0;
  void (*run_me)()   = nullptr;
  
  void run_loop(millis_t delta){
    //intervall zero : don't run
    if(intervall==0){sum_delta=0; return;}
    
    //run
    sum_delta+=delta;
    while (sum_delta>=intervall){
      sum_delta-=intervall;
      if(run_me!=nullptr){run_me();}
    }
  }
};


#endif
