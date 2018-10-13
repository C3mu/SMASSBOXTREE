#include <ESP8266WiFi.h>
#include <EEPROM.h>
const char WiFiAPPSK[] = "123456789"; //mat khau wifi
char thingSpeakAddress[] = "api.thingspeak.com";//sever
String APIKey = "FFK5E1KRI74KIF6Q";
unsigned long tim,tcn;
unsigned long tup = 30000;
String hd, t, h, times, to;
String l = "10";
String sm = "30";
String fe = "0.1";
String ssid, pass;
char MODE = 3;
char wifi = 0;
boolean so;
String xd = "\n";
WiFiServer server(80);
WiFiClient client;
IPAddress ip;
void disconnectwifi() {
  WiFi.setAutoConnect(false);
  WiFi.disconnect();
  Serial.println("disconected");
}
void connectwifi() {
  Serial.println("connected");
  WiFi.reconnect();
  WiFi.setAutoConnect(true);
  if ( ssid.length() > 1 ) {
  WiFi.begin(ssid.c_str(), pass.c_str());
  tcn = millis();
  while ((WiFi.status() != WL_CONNECTED) && ((millis() - tcn) < 5000)) {
    delay(10);
  }
  if (WiFi.status() != WL_CONNECTED){
    WiFi.setAutoConnect(false);
  }
  }
  
}
void updateThingSpeak(String tsData) {
  if (client.connect(thingSpeakAddress,80)) {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
    client.stop();
  }
}
void setupWiFi() {//HAM
  WiFi.mode(WIFI_AP_STA);
  uint8_t mac[WL_MAC_ADDR_LENGTH];//6bytes char
  WiFi.softAPmacAddress(mac);//get
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);//lay 2 byte cuoi
  macID.toUpperCase();// chuyen macId thanh viet hoa
  String AP_NameString = "CSCTM" + macID;//tranh trung lap dia chi ssid
  char AP_NameChar[AP_NameString.length() + 1];// do dai cua string + 1
  memset(AP_NameChar, 0, AP_NameString.length() + 1);// tuy chinh vung nho tu
  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);// String vao char
//  WiFi.softAPConfig(Ip, Ip, NMask);
  WiFi.softAP(AP_NameChar, WiFiAPPSK);//hamchichapnhanmangchar
  
}
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  WiFi.setOutputPower(10);
  for (int i = 0; i < 32; ++i) {
    ssid += char(EEPROM.read(i));
  }
  for (int i = 32; i < 96; ++i) {
    pass += char(EEPROM.read(i));
  }
  for (int i = 96; i < 99; ++i) {
    to += char(EEPROM.read(i));
  }
  if ( to.length() > 1 ) {
    if (to.indexOf("0") != -1) {
      tup = 0;
    }
    if (to.indexOf("30s") != -1) {
      tup = 30000;
    }
    if (to.indexOf("1m") != -1) {
      tup = 60000;
    }
    if (to.indexOf("1h") != -1) {
      tup = 3600000;
    }
    if (to.indexOf("1d") != -1) {
      tup = 86400000;
    }
  }
  WiFi.softAPdisconnect(true);
  disconnectwifi();
  
}
String next() {
  String nextpage = nextpage + "<!DOCTYPE html>\n<html>\n<head>\n please wait :D... <meta http-equiv='refresh' content='0; /home'></head></html> \r";
  return nextpage;
}
String ipToString(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
String style() {
  String htmlPage = htmlPage +
                    "<!DOCTYPE HTML>" + xd +
                    "<html>" + xd +
                    "<head>" + xd +
                    "<meta http-equiv = 'Content-Type' content = 'text / html; charset = utf-8'>" + xd +
                    "<TITLE> HỆ THỐNG CHĂM SÓC CÂY THÔNG MINH </TITLE>" + xd +
                    "<style>" + xd +
                    "body {" + xd +
                    "background-color: green;" + xd +
                    "font-family: Arial, Helvetica, sans-serif;" + xd +
                    "}" + xd +
                    "HR {" + xd +
                    "border: 1px solid green;" + xd +
                    "background-color: lightgreen;" + xd +
                    "margin-left: 1%;" + xd +
                    "margin-right: 1%;" + xd +
                    "}" + xd +
                    "HR.mini{" + xd +
                    "width: 200px " + xd +
                    "}" + xd +
                    "p {" + xd +
                    " font-size: 40px;" + xd +
                    " color: blue;" + xd +
                    "-webkit-margin-before: 0em;" + xd +
                    "-webkit-margin-after: 0em;" + xd +
                    "background: lightgreen; " + xd +
                    "background: -webkit-linear-gradient(lightgreen, white); " + xd +
                    "background: linear-gradient(lightgreen, white);" + xd +
                    "text-align:center" + xd +
                    "}" + xd +
                    "p.end{" + xd +
                    " font-size: 10px;" + xd +
                    "font-style: italic;" + xd +
                    "background: -webkit-linear-gradient(white, lightgreen); " + xd +
                    "background: linear-gradient(white, lightgreen);" + xd +
                    "text-align:left" + xd +
                    "}" + xd +
                    "a {" + xd +
                    "color: black;" + xd +
                    "border: 2px solid #4CAF50;" + xd +
                    "padding: 10px 10px;" + xd +
                    "text-align: center;" + xd +
                    "text-decoration: none;" + xd +
                    "display: inline-block;" + xd +
                    "font-size: 20px;" + xd +
                    "margin-left: 1px;" + xd +
                    "margin-top: 1px;" + xd +
                    "text-indent: 0px;" + xd +
                    "}" + xd +
                    "a:hover{" + xd +
                    "color: blue;" + xd +
                    "background-color: #4CAF50;" + xd +
                    "}" + xd +
                    "a.hightlight{" + xd +
                    "color: red;" + xd +
                    "background-color: lightgreen;" + xd +
                    "}" + xd +
                    "a.buton{" + xd +
                    "font-size: 10px;" + xd +
                    "margin-left: 5px;" + xd +
                    "border: 1px solid red;" + xd +
                    "}" + xd +
                    "div {" + xd +
                    "border: 2px solid lightgreen;" + xd +
                    "margin-left: 8%;" + xd +
                    "margin-right: 8%;" + xd +
                    "background-color: white;" + xd +
                    "padding: 10px 10px;" + xd +
                    "}" + xd +
                    "input {" + xd +
                    "padding: 2px 2px;" + xd +
                    "border: 1px solid red;" + xd +
                    "margin-left: 5px;" + xd +
                    "margin-bottom: 2px;" + xd +
                    "}" + xd +
                    "input:hover{" + xd +
                    "background-color: lightyellow;" + xd +
                    "}" + xd +
                    "</style>" + xd;
  return htmlPage;
}
String setupPage() {
  String htmlPage = htmlPage +
                    "</head>" + xd +
                    "<body>" + xd +
                    "<div>" + xd +
                    "<p>" + xd +
                    "<br/> HỆ THỐNG CHĂM SÓC CÂY THÔNG MINH <br/> <br/>" + xd +
                    "</p>" + xd +
                    "<HR/>" + xd +
                    "<a href='/home' > TRANG CHỦ </a>" + xd +
                    "<a href='/manual'> CÀI ĐẶT TÙY CHỈNH </a>" + xd +
                    "<a href='/setup' class = 'hightlight'> CÀI ĐẶT WIFI </a>" + xd +
                    "<HR/> " + xd +
                    "<br/><strong> Nhập tên WIFI (SSID) và PASSWORD của bạn </strong> <br/> " + xd +
                    "<form action='#'>" + xd +
                    "<br/> &emsp;SSID <input type='text' name='ssid' size='20'/>" + xd +
                    "<HR class = 'mini'/>" + xd +
                    "&emsp;PASS<input type='password' name='pass' size='20'/> " + xd +
                    "<input type='submit' value = ' OK ' class = 'button' onclick='reset'><br/><br/>" + xd +
                    "<EM> LƯU Ý : Điều này sẽ làm mất pass và ssid trước đó của bạn !</EM>" + xd +
                    "</form>" + xd +
//                    "<HR/>" + xd +
//                    "<strong> Hoặc sử dụng kết nối nhanh ! </strong>" + xd +
//                    "<a href = '/wps' class = 'buton' >WPS</a>  " + xd +
                    "<HR/>" + xd +
                    "<br/> <Strong> Chỉnh giờ hệ thống : </Strong><br/><br/>" + xd +
                    "<br/> &emsp; Thời gian hệ thống : " + times + xd +
                    "<form action='#'' >&emsp;" + xd +
                    "<input type='text' name='h' size='2'/> giờ " + xd +
                    "<input type='text' name='m' size='2'/> phút " + xd +
                    "<input type='text' name='s' size='2'/> giây " + xd +
                    "<input type='submit' value=' OK '  >" + xd +
                    "</form>" + xd +
                    "<br/>" + xd +
                    "<HR/>" + xd +
                    "<strong> Cài đặt thời gian gửi dữ liệu lên server </Strong><br/><br/>" + xd +
                    "<form action='#'>" + xd +
                    "&emsp; <select name= 'timeoff'>" + xd +
                    "<option value='0'>off</option>" + xd +
                    "<option value='30s'>30 giây</option>" + xd +
                    "<option value='1m' >1 phút</option>" + xd +
                    "<option value='1h'>1 giờ</option>" + xd +
                    "<option value='1d' >1 ngày</option>" + xd +
                    "</select>" + xd +
                    "<input type='submit' value=' OK '>" + xd +
                    "</form>" + xd +
                    "<br/>" + xd +
                    "<HR/>" + xd +
                    "<p class='end'>" + xd +
                    "<br/>" + xd +
                    "&emsp;Website được viết bởi Nguyễn Đỗ Quốc Anh (C3mu)<br/>" + xd +
                    "&emsp;Trường THCS Nguyễn Huệ - TP.Pleiku - Gialai<br/>" + xd +
                    "&emsp;Năm 2016" + xd +
                    "<br/><br/>" + xd +
                    "</p>" + xd +
                    "</div>" + xd +
                    "</body>" + xd +
                    "</html>\r";
  return htmlPage;
}
String manualPage() {
  String htmlPage2 = htmlPage2 +
                     "</head>" + xd +
                     "<body>" + xd +
                     "<div>" + xd +
                     "<p><br/> HỆ THỐNG CHĂM SÓC CÂY THÔNG MINH <br/> <br/> </p><HR/>" + xd +
                     "<a href='/home' > TRANG CHỦ </a>" + xd +
                     "<a href='/manual' class = 'hightlight'> CÀI ĐẶT TÙY CHỈNH </a>   " + xd +
                     "<a href='/setup'> CÀI ĐẶT WIFI </a>" + xd +
                     "<HR/><br/>" + xd +
                     "<form action='#'> " + xd +
                     "<ul>" + xd +
                     "<li>Thời gian chiếu sáng (0-24h)&emsp;&emsp;<input type='text' name='l' value='" + l + "' size='4'/> hours/day</li>" + xd +
                     "<li>Độ ẩm đất (0-100%)&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<input type='text' name='sm' value='" + sm + "' size='4'/> %</li>" + xd +
                     "<li>Lượng phân bón hằng ngày(~~) &nbsp;" + xd +
                     "<select name= 'fe'>" + xd +
                     "<option value='0'>off</option>" + xd +
                     "<option value='0.1'>0,1</option>" + xd +
                     "<option value='0.2'>0,2</option>" + xd +
                     "<option value='0.3'>0,3</option>" + xd +
                     "<option value='0.4'>0,4</option>" + xd +
                     "<option value='0.5'>0,5</option>" + xd +
                     "<option value='0.6'>0,6</option>" + xd +
                     "<option value='0.7'>0,7</option>" + xd +
                     "<option value='0.8'>0,8</option>" + xd +
                     "<option value='0.9'>0,9</option> " + xd +
                     "<option value='1'>1</option> " + xd +
                     "<option value='2'>2</option>" + xd +
                     "</select>ml/day<br/>" + xd +
                     "&emsp;&emsp;&emsp;<input type='submit' value=' OK '></li>" + xd +
                     "</ul><br/>" + xd +
                     "</form> <HR/>" + xd +
                     "<p class='end'>" + xd +
                     "<br/>" + xd +
                     "&emsp;Website được viết bởi Nguyễn Đỗ Quốc Anh (C3mu)<br/>" + xd +
                     "&emsp;Trường THCS Nguyễn Huệ - TP.Pleiku - Gialai<br/>" + xd +
                     "&emsp;Năm 2016" + xd +
                     "<br/><br/>" + xd +
                     "</p>" + xd +
                     "</div>" + xd +
                     "</body>" + xd +
                     "</html>\r";
  return htmlPage2;
}
String HomePage() {
  ip = WiFi.localIP();
  String a1, a2, a3;
  switch (MODE) {
    case '2':
      a2 = "selected";
      break;
    case '3':
      a3 = "selected";
      break;
    default: a1 = "selected";
  }
  String htmlPage1 = htmlPage1 +
                     "</head>" + xd +
                     "<body>" + xd +
                     "<div>" + xd +
                     "<p><br/> HỆ THỐNG CHĂM SÓC CÂY THÔNG MINH <br/> <br/> </p><HR/>" + xd +
                     "<a href='/home' class = 'hightlight'> TRANG CHỦ </a>" + xd +
                     "<a href='/manual'> CÀI ĐẶT TÙY CHỈNH </a>" + xd +
                     "<a href='/setup'> CÀI ĐẶT WIFI </a>" + xd +
                     "<HR/>" + xd +
                     "<form action='#'>" + xd +
                     "<br/>" + xd +
                     "&emsp; Chế độ " + xd +
                     "<select name= 'mode'>" + xd +
                     "<option value='1' " + a1 + "> tự động1</option>" + xd +
                     "<option value='2' " + a2 + "> tự động2</option>" + xd +
                     "<option value='3' " + a3 + "> tùy chỉnh</option>" + xd +
                     "</select>" + xd +
                     "<input type='submit' value=' OK '>   " + xd +
                     "<br/><br/>" + xd +
                     "</form>" + xd +
                     "<HR/>" + xd +
                     "<br/>" + xd +
                     "<strong> Thống kê ! </strong>" + xd +
                     "<br/>" + xd +
                     "<br/>&emsp; Độ ẩm ~(+-5%) " + h + "%" + xd +
                     "<br/>&emsp; Nhiệt độ(+-2) " + t + "oC" + xd +
                     "<br/>&emsp; Độ ẩm đất " + hd + "%" + xd +
                     "<br/>&emsp; chế độ Wifi: " + String(WiFi.status()) + xd +
                     "<br/> &emsp; IP mạng lan : " + ipToString(ip) + xd +
                     "<br/> &emsp; Thời gian hệ thống : " + times + xd +
                     " <br/><br/> " + xd +
                     "<HR/>" + xd +
                     "<br/>" + xd +
                     "<strong> Điều khiển nhanh </strong> " + xd +
                     "<br/><br/> " + xd +
                     "<a href = '/pump' class = 'buton'> bật máy bơm trong 1s </a> <br/>" + xd +
                     "<a href = '/led' class = 'buton'> điều khiển đèn </a> <br/>" + xd +
                     "<a href = '/sup' class = 'buton'> bổ sung phân bón </a> <br/>" + xd +
                     "<a href = '/OK' class = 'buton'>  dừng phát WIFI </a> <br/>" + xd +
                     "<br/>" + xd +
                     "<HR/>" + xd +
                     "<p class = 'end'>" + xd +
                     "<br/>" + xd +
                     "&emsp; Website được viết bởi Nguyễn Đỗ Quốc Anh (C3mu) <br/> " + xd +
                     "&emsp; Trường THCS Nguyễn Huệ - TP.Pleiku - Gialai <br/> " + xd +
                     "&emsp; Năm 2016" + xd +
                     "<br/><br/>" + xd +
                     "</p>" + xd +
                     "</div>" + xd +
                     "</body>" + xd +
                     "</html>\r";
  return htmlPage1;
}
void loop() {
  if ((unsigned long) (millis() - tim) >= tup) {
    tim = millis();
    if (tup != 0) {
      if (wifi == 0) {
        connectwifi();
        if (WiFi.status() == WL_CONNECTED) {
          updateThingSpeak("field1=" + t + "&field2=" + h + "&field3=" + hd);
        }
        disconnectwifi();
      } else { /// cancle 
//        if (WiFi.status() == WL_CONNECTED) {
//          updateThingSpeak("field1=" + t + "&field2=" + h + "&field3=" + hd);
//        }
      }
    }
  }
  delay(10);
  if (Serial.available()) {
    String data = "";
    while (Serial.available() > 0) {
      data  = data + char(Serial.read());
      delay(1);    
      }
    if (data.indexOf("hd=") != -1) {
      hd = t = h = "";
      for (int i = data.indexOf("hd=") + 3; i < data.indexOf("&t"); i++) {
        hd = hd + data[i];
      }
      for (int i = data.indexOf("&t=") + 3; i < data.indexOf("&h"); i++) {
        t = t + data[i];
      }
      for (int i = data.indexOf("&h=") + 3; i < data.indexOf("#"); i++) {
        h = h + data[i];
      }
    }
    if (data.indexOf("time=") != -1) {
      times = "";
      for (int i = data.indexOf("time=") + 5; i < data.indexOf("#"); i++) {
        times = times + data[i];
      }
    }
    if (data.indexOf("mode1") != -1) {
      wifi = 1;
      Serial.print("mode1#");
      WiFi.softAPdisconnect(false);
      connectwifi();
      setupWiFi();
      server.begin();
    }
    if (data.indexOf("mode0") != -1) {
      Serial.print("mode0#");
      wifi = 0;
      WiFi.softAPdisconnect(true);
      disconnectwifi();
      server.close();
    }
  }
  WiFiClient client = server.available();
  if (client) {
    String line = client.readStringUntil('\r');
    client.flush();
    delay(10);
    client.println("");
        if (line.indexOf("/setup") != -1) {
      client.print(style());
      client.print(setupPage());
    }
    else if (line.indexOf("/home") != -1) {
      client.print(style());
      client.print(HomePage());
    }
    else if (line.indexOf("/manual") != -1) {
      client.print(style());
      client.print(manualPage());
    } else {
      client.println(next());
    }
    if (line.indexOf("/pump") != -1 ) {
      Serial.print("pump");
//      client.println(next());
    }
    if (line.indexOf("/sup") != -1 ) {
      Serial.print("sup");
//      client.println(next());
    }
    if (line.indexOf("/OK") != -1 ) {
      Serial.print("mode0");
      wifi = 0;
      WiFi.softAPdisconnect(true);
      disconnectwifi();
    }
    if (line.indexOf("/led") != -1 ) {
      Serial.print("led");
    }
    if (line.indexOf("/wps") != -1) { // text ---> not working !!!
      WiFi.mode(WIFI_STA);
      WiFi.setAutoConnect(true);
      WiFi.begin("foobar",""); 
      WiFi.beginWPSConfig();
    }
    if (line.indexOf("h=") != -1) {
      String h, m, s;
      for (int i = line.indexOf("=") + 1; i < line.indexOf("&m"); i++) {
        h = h + line[i];
      }
      if (h==""){h='0';}
      for (int i = line.indexOf("m=") + 2; i < line.indexOf("&s"); i++) {
        m = m + line[i];
      }
      if (m==""){m='0';}
      for (int i = line.indexOf("s=") + 2; i < line.indexOf(" HTTP"); i++) {
        s = s + line[i];
      }
      if (s==""){s='0';}
      String prs = "sth=" + h + "m=" + m + "s=" + s + "#";
      Serial.println(prs);
    }
    if (line.indexOf("l=") != -1) {
      l = fe = sm = "";
      for (int i = line.indexOf("=") + 1; i < line.indexOf("&sm"); i++) {
        l = l + line[i];
      }
      for (int i = line.indexOf("sm=") + 3; i < line.indexOf("&fe"); i++) {
        sm = sm + line[i];
      }
      for (int i = line.indexOf("fe=") + 3; i < line.indexOf(" HTTP"); i++) {
        fe = fe + line[i];
      }
      String pr = "l=" + l + "sm=" + sm + "fe=" + fe + "#";
      MODE='3';
      Serial.print(pr);
    }
    if (line.indexOf("timeoff=") != -1) {
      to = "";
      for (int i = line.indexOf("=") + 1; i < line.indexOf(" HTTP"); i++) {//error
        to = to + line[i];
      }
      for (int i = 96; i < 99; i++) {
        EEPROM.write(i, 0);
      }
      for (int i = 0; i < to.length(); i++)
      {
        EEPROM.write(i + 96, to[i]);
      }
      if (to.indexOf("0") != -1) {
        tup = 0;
      }
      if (to.indexOf("30s") != -1) {
        tup = 30000;
      }
      if (to.indexOf("1m") != -1) {
        tup = 60000;
      }
      if (to.indexOf("1h") != -1) {
        tup = 3600000;
      }
      if (to.indexOf("1d") != -1) {
        tup = 86400000;
      }
      EEPROM.commit();
    }
    if (line.indexOf("mode=") != -1) {
      MODE = line[line.indexOf("mode=") + 5];
      switch (MODE) {
        case '1':
          Serial.print("l=12sm=30fe=0.1#");
          break;
        case '2':
          Serial.print("l=12sm=20fe=0.1#");
          break;
        case '3':
          String prn = "l=" + l + "sm= " + sm + "fe=" + fe + "#";
          Serial.print(prn);
          break;
      }
    }
    if (line.indexOf("ssid=") != -1) {
      ssid = pass = "";
      for (int i = line.indexOf("ssid=") + 5; i < line.indexOf("&"); i++) {
        ssid = ssid + line[i];
      }
      for (int i = line.indexOf("&pass=") + 6; i < line.indexOf(" HTTP"); i++) {
        pass = pass + line[i];
      }
      for (int i = 0; i < 96; i++) {
        EEPROM.write(i, 0);
      }
      for (int i = 0; i < ssid.length(); i++)
      {
        EEPROM.write(i, ssid[i]);
      }
      for (int i = 0; i < pass.length(); i++)
      {
        EEPROM.write(i + 32, pass[i]);
      }
//      Serial.println("get");
//      Serial.println(ssid);
//      Serial.println(pass);
      EEPROM.commit();
      connectwifi();
    }
    delay(1);
    client.stop();
  }
}
//YAY DONE :D
