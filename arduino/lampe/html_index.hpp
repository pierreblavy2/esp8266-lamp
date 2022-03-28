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
 

#ifndef INDEX_HPP_PIERRE_
#define INDEX_HPP_PIERRE_

const char* html_index = R"htmlstring(<!DOCTYPE html>
<html lang="en" >
<head>
<meta charset="utf-8"/>

<title>Lamp</title>


<script>
function new_id(name=""){
  new_id.id = new_id.id +1;
  return name + new_id.id 
}
new_id.id = 0;


function color_add(color="#ffffff", update=true) {
  var el = document.getElementById("colors_table");
  
  var newRow = el.insertRow();
  
  var newCell1 = newRow.insertCell();
  newCell1.id= new_id("rm_color");
  newCell1.innerHTML='<button type="button" title="Remove color" onclick="color_rm(\'' + newCell1.id + '\')" > - </button>';
  
  var newCell2 = newRow.insertCell();
  newCell2.innerHTML='<input type="color" value="'+color+'"  onchange="lamp_send();" >';
  
  if(update){lamp_send();}
}


function color_rm(id) {
  //don't remove last one
  if(document.getElementById("colors_table").rows.length<=1){return;}
  
  //remove me
  var el = document.getElementById(id);
  el2 =el.parentNode
  el2.parentNode.removeChild(el2); 
  lamp_send();
}


function color_rm_all(){
  el = document.getElementById("colors_table");
  while(el.rows.length>0){
    el.deleteRow(0);
  }
}


function post_json(url,j){
  console.log("--- POST "+url+" ---");
  console.log(JSON.stringify(j));
  
  var xhr = new XMLHttpRequest();
  xhr.open("POST", url , true);
  xhr.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
  xhr.send(JSON.stringify(j));
}


function log_error(status , r){
  console.log("--- ERROR "+status+" ---");
  console.log(r);
}


function get_json(url,callback_ok, callback_err=log_error){
  //doc : https://stackoverflow.com/questions/12460378/how-to-get-json-from-url-in-javascript
  console.log("--- GET "+url+" ---");

  var xhr = new XMLHttpRequest();
  xhr.open('GET', url, true);
  xhr.responseType = 'json';
  xhr.onload = function() {
      var status = xhr.status;
      if (status === 200) {
        callback_ok( xhr.response );
      } else {
        callback_err(status, xhr.response);
      }
  };
  xhr.send();
}



function lamp_send(){
  j = {
    "on"    : document.getElementById("on").checked ,
    "period": document.getElementById("period").value ,
    "colors": []
  };
  
  var table = document.getElementById("colors_table");
  for (var i = 0, row  ; row = table.rows[i]; i++) {
    color = row.children[1].children[0].value;
    j.colors.push(color);
  }

  console.log("--- lamp_send TODO ---");
  console.log(j);
  post_json("post_lamp",j);
}

function lamp_receive(j){
  console.log("--- lamp_receive ---");
  console.log(j);

  document.getElementById("period").value = j.period;
  document.getElementById("on").checked   = j.on;
  
  color_rm_all();
  j.colors.forEach(function(a){
    color_add(a,false);
  });
  
}

function wifi_receive(j){
  console.log("--- wifi_receive ---");
  console.log(j);
  
  document.getElementById("wifi_ssid").value = j.ssid;
  document.getElementById("wifi_pass").value = "";
  document.getElementById("wifi_my_ssid").value = j.my_ssid;
  document.getElementById("wifi_status").innerText = j.status;
}

function wifi_send(){
  j = {
    "ssid"    : document.getElementById("wifi_ssid").value ,
    "pass"    : document.getElementById("wifi_pass").value ,
    "my_ssid" : document.getElementById("wifi_my_ssid").value 
  };
  post_json("post_wifi",j);
}


function main(){
  get_json("get_wifi",wifi_receive);
  get_json("get_lamp",lamp_receive);
}
main();

</script>
</head>


<body>

<h1>Light</h1>
<table>
<tr>
<td><label for="on">ON</label></td>
<td><input type="checkbox" id="on" name="on" onchange="lamp_send();"></td>
</tr>
<tr>
<td><label for="period">Period(ms)</label></td>
<td><input type="number" id="period" name="period" min="2" value = "100" onchange="lamp_send();" ></td>
</tr>
</table>

<h1>Colors</h1>
<table id = "colors_table"></table>
<button type="button" title="Add color" onclick="color_add()" > + </button>


<h1>Wifi</h1>
<form onsubmit="wifi_send(); return false;"  name="wifi">
<table>
<tr>
<td><label for="wifi_ssid">SSID</label></td>
<td><input required type="text" name="ssid" id="wifi_ssid" title = "Try to connect to this network"></td>
</tr>
	
<tr>
<td><label for="wifi_pass">Password</label></td>
<td><input type="password" name="pass" id="wifi_pass"  title = "Leave empty for no change"></td>
</tr>

<tr>
<td><label for="wifi_my_ssid">My ssid</label></td>
<td><input required type="text" name="wifi_my_ssid" id="wifi_my_ssid" title="When there is no connection to wifi, creates a standalone access point with this name"></td>
</tr>

<tr>
<td><label for="wifi_status">Status</label></td>
<td id="wifi_status">unknown</td>
</tr>


</table>
<input value="Submit" type="submit"> Wifi changes require to reset the light.
</form>
    
   


</body>

</html>
)htmlstring";

#endif
