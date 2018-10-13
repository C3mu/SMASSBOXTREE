#include <DHT.h>
//#include <Wire.h>
#include <DS3231.h>
#include <Eeprom24Cxx.h>
#include <Servo.h>
DHT dht(A2, DHT22);
DS3231 clock;
RTCDateTime dt;
Eeprom24C eeprom(32,0x50);
Servo myservo;
// chan 
byte pump1 = 3;
//byte pump2 = 10;
byte lightpin1 = 10;
byte button = 5;
byte led1 = 6;//
byte led2 = 7;
byte led3 = 8;
byte led4 = 9;
// bien
boolean pump,led,mode,wu,ok,sended,error1,error2,DO,ps;
unsigned long t,ledl,pu,tout;
//byte timeoff;
byte dat[1][6];
float to, h;
int h1,h2,PG,x;
void turn(int s) {
  boolean Stop = false;
  int st = 1;
  while (!Stop) {
    for (int i = x; i <= 180; i += 2) { //!!! remember last *
      st = st + 1;
      if (st >= s) {
        Stop = true;
        break;
      }
      myservo.write(i);
      delay(15);
      x = i;
    }
    for (int i = x; i >= 0; i -= 2) {
      st = st + 1;
      if (st >= s) {
        Stop = true;
        break;
      }
      myservo.write(i);
      delay(15);
      x = i;
    }
    if (st >= s) {
      Stop = true;
      break;
    }
  }
}
void sup(){
    if (!error1){myservo.attach(4);
  unsigned long tis = millis();
    while (true) {
      for (int i = x; i <= 180; i += 7) {
        if ((analogRead(A0) > 100) || !((millis() - tis < 50000))) {
          break;
        }
        x=i;
        myservo.write(x);
        delay(20);
      }
      for (int i = x; i >= 0; i -= 7) {
        if ((analogRead(A0) > 100) || !((millis() - tis < 50000))) {
          break;
        }
        myservo.write(i);
        delay(20);
        x=i;
      }
      if ((analogRead(A0) > 100) || !((millis() - tis < 50000))) {
        break;
      }
    }
    if (millis() - tis > 50000) {
      error1 = true;
    } else {
turn(dat[0][3]*20);
  }
      myservo.detach(); 
}
}
void setup() {
  // khoi động các model
  Serial.begin(115200);
  dht.begin();
  clock.begin();
  pinMode(pump1, OUTPUT);
//  pinMode(pump2, OUTPUT);
  pinMode(lightpin1, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(led1, OUTPUT);//blue
  pinMode(led2, OUTPUT);//yellow
  pinMode(led3, OUTPUT);//red
  pinMode(led4, OUTPUT);//green
  //read data
  digitalWrite(led1,HIGH);
  for (int a = 0; a <= 1; a++) {
   for (int b = 1; b <= 6; b++) {
    dat[a][b]=eeprom.read_1_byte(b+6*a);
   }
}
dat[0][0] = eeprom.read_1_byte(0);
digitalWrite(led1,LOW);
error1=error2=0;
// send nowmal data
      String ds = "";
       int b =1;
        for (b = b; b < 6; b++) {
         ds = ds + "d"+String(b)+"=" + String(dat[0][b]) + "&" ;
        }
        ds = ds + "d6=" + String(dat[0][6]) + "#D";
       ds = ds + "tup=" + String(eeprom.read_1_byte(0));
       Serial.print(ds);
}
void loop() {
  tout = millis();
  while(!Serial.available() > 0){ 
    if((unsigned long)(millis() - tout) >= 10){
      break; 
    }}
  if (Serial.available()) {
   String data = Serial.readStringUntil('\r');
    Serial.flush();
    if (data.indexOf("tup") != -1) {
      Serial.print("tup=");
      Serial.print(eeprom.read_1_byte(0));
    }
    if (data.indexOf("ghd") != -1) { // just a bit hard
     String ds = "";
       int b =1;
        for (b = b; b < 6; b++) {
         ds = ds + "d"+String(b)+"=" + String(dat[0][b]) + "&" ;
        }
        ds = ds + "d6=" + String(dat[0][6]) + "#D";
//       ds = "tup=" + String(eeprom.read_1_byte(0));
       Serial.print(ds);
  }if (data.indexOf("gnd") != -1) {
        String ds = "";
   
    ds = ds + "ha=" +String(h)+ "&te=" +String(to)+ "&h1=" +String(h1)+ "&h2="+String(h2)+"#NM";
    ds = ds + "time="+String(dt.hour)+":"+String(dt.minute)+":"+String(dt.second)+"#T";
    if(pump){
        ds = ds + "pump";
      }
      if(led){
        ds = ds + "light";
      }
     Serial.println(ds);
  }
    if (data.indexOf("sth=") != -1) { /// it gooodddddd lol
      String h, m, s;
      for (int i = data.indexOf("h=") + 2; i < data.indexOf("m"); i++) {
        h = h + data[i];
      }
      for (int i = data.indexOf("m=") + 2; i < data.indexOf("s="); i++) {
        m = m + data[i];
      }
      for (int i = data.indexOf("s=") + 2; i < data.indexOf("#"); i++) {
        s = s + data[i];
      }
      clock.setDateTime(2016, 5, 25, h.toInt(), m.toInt(), s.toInt());
    }
    if (data.indexOf("tup=") != -1){
      dat[0][0] = data[data.indexOf("tup=") + 4] - 48;
       eeprom.write_1_byte(0,dat[0][0]);
    }
    if (data.indexOf("d1") != -1) { // not use for three more up tree
//      char pg = data[data.indexOf("PG")+2];
//      int n = 1;
//      if(pg == '1'){ // or =2
//        n=0;
//      }
      String hdt,db,de;
          for (int b = 1; b < 6; b++) {
            hdt="";
            db=de="";
            db = db+ "d" + String(b)+"=";
            de = de+"d" + String(b + 1) + "=";
            for (int i = data.indexOf(db) + 3; i < data.indexOf(de); i++) {
              hdt = hdt + data[i];
            }
             eeprom.write_1_byte(b,byte(hdt.toInt()));
          }hdt="";
           for (int i = data.indexOf("d6=") + 3; i < data.indexOf("#"); i++) {
              hdt = hdt + data[i];
            }
             eeprom.write_1_byte(6,byte(hdt.toInt()));
             for (int a = 0; a <= 1; a++) {
             for (int b = 1; b <= 6; b++) {
          dat[a][b]=eeprom.read_1_byte(b+6*a);
                }
               }
              }
    if (data.indexOf("pump") != -1) {
        digitalWrite(pump1,HIGH);
        delay(1000);
        digitalWrite(pump1,LOW);
      
    }
    if (data.indexOf("sup") != -1) {
    error1 = false;
    digitalRead(led2) == LOW ;
    sup();
    }
    if (data.indexOf("led") != -1) {
      if(!led){
        led = true;
        digitalWrite(lightpin1,HIGH);
      }else{
        led = false;
        digitalWrite(lightpin1,LOW);
      }
    }
    if (data.indexOf("mode1#") != -1 ) {
      digitalWrite(led1,HIGH);
      mode=1;
    }
    if (data.indexOf("mode0#") != -1 ) {
      digitalWrite(led1,LOW);
      mode=0;
    }
    if (data.indexOf("connected") != -1 ) {
      digitalWrite(led4,HIGH);
      ok = true;
    }
    if (data.indexOf("disconected") != -1 ) {
      digitalWrite(led4,LOW);
      ok = false;
    } 
  }
  if (!digitalRead(button)){
 if (!sended){
      sended=1;
      if (mode){
        Serial.println("mode0"); 
        digitalWrite(led1,LOW);
        delay(100);
        digitalWrite(led1,HIGH);
      }else{
        Serial.println("mode1");
        digitalWrite(led1,HIGH);
        delay(100);
        digitalWrite(led1,LOW);
      }
    }
  }
  else {
    sended=0;  
  }
if ( (unsigned long) (millis() - ledl) >= 1000 ){ 
if (error1){
        if (digitalRead(led3) == LOW )
        {
           digitalWrite(led3, HIGH);
        } else {
           digitalWrite(led3, LOW );
        }
    }else{
      digitalWrite(led3, LOW); 
    }
if (error2) {
        if (digitalRead(led2) == LOW )
        {
           digitalWrite(led2,HIGH);
        } else {
           digitalWrite(led2,LOW);
        }
  }else{
      digitalWrite(led2, LOW); 
    }
   ledl = millis();
  }
    
  if (dt.hour - dat[0][5] >= dat[0][1]){ 
    digitalWrite(lightpin1, LOW);
  } 
  if (led && ( dt.hour >= dat[0][5])) {
    digitalWrite(lightpin1, HIGH);
  }
   if ((h1 >= dat[0][2])) {
    digitalWrite(pump1, LOW);
     ps = true;
     error2 = false;
     pu = millis();
  }  if (h1 < dat[0][2] - 5) { 
    if (!error2){digitalWrite(pump1, HIGH);}
    if (ps) {
      pu = millis();
      ps = false;
    }
  }
    if (millis() - pu > 30000) {  
      digitalWrite(pump1, LOW);    
      error2 = true;
   }
//  if (h2 >= dat[1][5]) {
//    digitalWrite(pump2, LOW);
//  }  
//  if (h2 < dat[1][5] - 5) { 
//      digitalWrite(pump2, HIGH);
//  }
dt = clock.getDateTime();
h1 = map(analogRead(A3),1023,0,0,100);
h2 = map(analogRead(A1),1023,0,100,0);
if ((unsigned long) (millis() - t) >= 2000) { // cap nhap do am moi 2s
    float ha = dht.readHumidity();
    float te = dht.readTemperature();
    if (isnan(ha) || isnan(te)) {//bo qua du lieu gui ve neu ko co
    } else {
      h = ha;
      to = te;
    }
    t = millis();
    if(dat[0][6] == 1){
if (dt.hour == 5) {
    DO = true;
  }
if ((dt.hour == 6) && (DO == true)) {
    sup();
    DO = false;
}}
if(!mode){
if(dat[0][0]!=0){
      if(wu){ Serial.print("update");wu = false;   
      String ds = "";
      if(pump){
        ds = ds + "pump";
      }
      if(led){
        ds = ds + "light";
      }
      ds = ds + "ha=" +String(h)+ "&te=" +String(to)+ "&h1=" +String(h1)+ "&h2="+String(h2)+"#NM";
      Serial.println(ds);
      }
      if(dat[0][0] == 1){
       if (dt.second == 29 || dt.second == 0){wu = true;/*Serial.print("update");*/delay(1000);}
      }
      if(dat[0][0] == 2){
       if (dt.second == 0){wu = true;delay(1000);}
      }
      if(dat[0][0] == 3){
       if (dt.minute == 0 && dt.second == 0){wu = true;delay(1000);}
      }
      if(dat[0][0] == 4){
       if (dt.hour == 1 && dt.minute == 0 && dt.second == 0){wu = true;delay(1000);}
      }
    }
}
}
}

