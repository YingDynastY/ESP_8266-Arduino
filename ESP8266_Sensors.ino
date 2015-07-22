#include<SoftwareSerial.h>
#define D0 3
#define D1 2
#define D2 5
#define D3 4
#define D4 9
#define D5 8
#define AN0 3
#define AN1 1
#define AN2 2
#define DELAY 800
//all define for Sensors finish here
//*********************
#define SSID "Michael"
#define PASS "zhuying123"
#define DST_IP "184.106.153.149"
int fail_cnt=0;
#define _SoftRxpin 6
#define _SoftTxpin 7


SoftwareSerial ESP8266(_SoftRxpin,_SoftTxpin);
String apiKey="NYOMG3R0B1DOCKYJ";
boolean connectModule(){ 
  ESP8266.println("AT+RST"); 
  Serial.println("Sent AT+RST, response: "); 
   
  delay(1000); 

  if(ESP8266.find("ready")){ 
    Serial.println("conect module success");
    return true; 
  }else{ 
     Serial.println("conect module fail");
    return false; 
  } 
} 

boolean connectWiFi(){ 
  
  String cmd="AT+CWJAP=\""; 
  cmd+=SSID; 
  cmd+="\",\""; 
  cmd+=PASS; 
  cmd+="\""; 
   
  ESP8266.println(cmd); 
  Serial.println(cmd); 
   
  delay(5000); 

  if(ESP8266.find("OK")){ 
    Serial.println("WiFi connected"); 
    return true; 
  }else{ 
    Serial.println("Could not connect"); 
    return false; 
  } 
} 

void setup()   
{
  Serial.begin(115200); 
  Serial.setTimeout(2000);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  ESP8266.begin(115200);
  Serial.println("Wi-Fi Module start to Work");
  while(!connectModule())
  {
      delay(100);    
  }
  while(!connectWiFi())
  {
      delay(100);
   }
}
void send(String msg){ 
  /* specify connection channel and IP */ 
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ESP8266.println(cmd); 
  Serial.println("------------> send(message):"); 
  Serial.println(cmd); 
   
  if(ESP8266.find("ERROR")){ 
    Serial.print("ERROR"); 
    return; 
  } 
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += msg;
  getStr += "\r\n\r\n";
  Serial.print("AT+CIPSEND="); 
  Serial.println(msg.length()); 
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ESP8266.println(cmd);
   
  if(ESP8266.find(">")){ 
    ESP8266.print(getStr); 
    Serial.println("Data Update Success"); 
  /*  fail_cnt++; 
    if(fail_cnt == 5){ 
       // re-establish link to Wifi 
       connectModule(); 
       
       connectWiFi(); 
       fail_cnt = 0; 
    } 
    delay(1000); 
    return; 
  } 
  Serial.println(msg); 
  ESP8266.println(msg); */
  }
  else{
    ESP8266.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
    
   
/*  // get response 
  while (Serial.available()){ 
    char c = Serial.read(); 
    ESP8266.write(c); 
  } 
  Serial.println("------------> OK: send(message):");*/ 
} 

void loop()                     
{
  // Get zero point ('tare') of scale
  int tare = measure ();
 // if(tare<0)
 // {
  //    tare=-tare;
  //  }
  int tare1= measure1();
  /*if(tare1<0)
  {
      tare1=-tare1;
    }*/
  int tare2= measure2();
 /*  if(tare2<0)
  {
      tare2=-tare2;
    }*/
  while (true) {
    long resultA=measure()-tare;
    long resultB=measure1()-tare1;
    long resultC=measure2()-tare2;
    if(resultA<0)
    {
      resultA=-resultA;
    }
     if(resultB<0)
    {
      resultB=-resultB;
    }
     if(resultC<0)
    {
      resultC=-resultC;
    }
     Serial.println("**************************");
     Serial.println("Load Sensor A Feedback:");
     Serial.println ( resultA );
     Serial.println("Load Sensor A Tare:");
     Serial.println ( tare );
     Serial.println("Load Sensor B Feedback:");
     Serial.println ( resultB );
     Serial.println("Load Sensor B Tare:");
     Serial.println ( tare1 );
     Serial.println("Load Sensor C Feedback:");
     Serial.println ( resultC );
     Serial.println("Load Sensor C Tare:");
     Serial.println ( tare2 );
     long Result=(resultA+resultB+resultC)/3;
     Result=Result*16.6+40;
     Serial.println("The real weight of object");
     Serial.println(Result);
     Serial.println("**************************");
     Serial.println("Measure Unit:GM");
     Serial.println("**************************");
     delay (1000);
     Serial.println("data ready to upload");
     String SensorA=String(resultA);
     String SensorB=String(resultB);
     String SensorC=String(resultC);
     String Sensors=String(Result);
     send(Sensors);
     delay(15000);
  }
  
}

int measure () {
  int i,a0,a1;
  int s;
  for (i = 0; i < 256; i++) {
    digitalWrite (D0,HIGH);
    digitalWrite (D1,LOW);
    delayMicroseconds(DELAY);
    a0 = analogRead(AN0);
     
    // reverse polarity
    digitalWrite (D0,LOW);
    digitalWrite (D1,HIGH);
    delayMicroseconds(DELAY);
    a1 = analogRead(AN0);
    s +=(long)(a0 - a1);      
   } // next i
   
   // Don't care about polarity
   if ( s < 0) {
       s = -s;
   }
   long result;
   result=s/128;
   // As there is a large number of samples being averaged we
   // can probably extract another bit or two from the ADC.
   // Going to be conservative and going for one extra bit.
   // So instead of dividing by 256, will divide by just 128
   // instead (ie right shift 7 bits).
   return result;
}

int measure1 () {
  int i,a0,a1;
  int s;
  for (i = 0; i <256 ; i++) {
    digitalWrite (D2,HIGH);
    digitalWrite (D3,LOW);
    delayMicroseconds(DELAY);
    a0 = analogRead(AN1);
     
    // reverse polarity
    digitalWrite (D2,LOW);
    digitalWrite (D3,HIGH);
    delayMicroseconds(DELAY);
    a1 = analogRead(AN1);
    s += (long) (a0 - a1);      
   } // next i
   
   // Don't care about polarity
   if ( s < 0) {
       s = -s;
   }
   long result;
   result=s/128;
   // As there is a large number of samples being averaged we
   // can probably extract another bit or two from the ADC.
   // Going to be conservative and going for one extra bit.
   // So instead of dividing by 256, will divide by just 128
   // instead (ie right shift 7 bits).
   return result;
}

int measure2 () {
  int i,a0,a1;
  int s;
  for (i = 0; i <256 ; i++) {
    digitalWrite (D4,HIGH);
    digitalWrite (D5,LOW);
    delayMicroseconds(DELAY);
    a0 = analogRead(AN2);
     
    // reverse polarity
    digitalWrite (D4,LOW);
    digitalWrite (D5,HIGH);
    delayMicroseconds(DELAY);
    a1 = analogRead(AN2);
    s +=(long)(a0 - a1);      
   } // next i
   
   // Don't care about polarity
   if ( s < 0) {
       s = -s;
   }
   long result;
   result=s/128;
   // As there is a large number of samples being averaged we
   // can probably extract another bit or two from the ADC.
   // Going to be conservative and going for one extra bit.
   // So instead of dividing by 256, will divide by just 128
   // instead (ie right shift 7 bits).
   return result;
}
