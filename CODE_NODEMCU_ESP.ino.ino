#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL7PpbPOXG"
#define BLYNK_DEVICE_NAME "demoLED"
#define BLYNK_AUTH_TOKEN "aYZ65bdepNPmjHci8Lk0C7riWi_EDVcW"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

#define ledhongngoai D1
#define int1 D5
#define int2 D6
#define ledphongngu D4
#define ledphongkhach D3
#define ledphongbep D8
#define ledsan D0
#define fan D7
#define cbcua D2
 
char auth[] = "aYZ65bdepNPmjHci8Lk0C7riWi_EDVcW";
char ssid[] = "Redmi Note 7";
char pass[] = "88888888";
 
SimpleTimer timer;

int timerID1, timerID2, timerID3;
boolean runModecbcua;
boolean canhbaoStatecbcua = HIGH;
boolean runMode;
boolean canhbaoState = HIGH;
 
String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors
 
int NhietDo, KhiGas, Doam, Lua, Rada; // sensors 
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(cbcua, INPUT_PULLUP);
  pinMode(ledhongngoai, INPUT_PULLUP);
  pinMode(int1, OUTPUT);
  pinMode(int2, OUTPUT);
  pinMode(ledphongngu, OUTPUT);
  pinMode(ledphongkhach, OUTPUT);
  pinMode(ledphongbep, OUTPUT);
  pinMode(ledsan, OUTPUT);
  pinMode(fan, OUTPUT);
  timerID1 = timer.setInterval(1000L,handleTimerID1);
}

BLYNK_WRITE(V4)
{
  int p = param.asInt();
  digitalWrite(ledphongngu, p); 
}
BLYNK_WRITE(V5)
{
  int p = param.asInt();
  digitalWrite(ledphongkhach, p); 
}
BLYNK_WRITE(V6)
{
  int p = param.asInt();
  digitalWrite(ledphongbep, p); 
}
BLYNK_WRITE(V7)
{
  int p = param.asInt();
  digitalWrite(ledsan, p); 
}
BLYNK_WRITE(V8) 
{
  runMode = param.asInt();
}
BLYNK_WRITE(V9)
{
  int p = param.asInt();
  digitalWrite(fan, p);
}
BLYNK_WRITE(V12)
{
  runModecbcua = param.asInt();
}
BLYNK_WRITE(V14)
{
  int p = param.asInt();
  if (p == 1)
  {
    digitalWrite(int1, HIGH);
    digitalWrite(int2, LOW);
  }
  else
  {
    digitalWrite(int1, LOW);
    digitalWrite(int2, LOW);
  }
}
BLYNK_WRITE(V15)
{
  int p = param.asInt();
  if (p == 1)
  {
    digitalWrite(int1, LOW);
    digitalWrite(int2, HIGH);
  }
  else
  {
    digitalWrite(int1, LOW);
    digitalWrite(int2, LOW);
  }
}

void loop()
{
  if (Serial.available() == 0 ) 
  {
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
  }
  if (Serial.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
    if( rdata == '\n')
    {
      String l = getValue(myString, ',', 0);
      String m = getValue(myString, ',', 1);
      String n = getValue(myString, ',', 2);
      String o = getValue(myString, ',', 3);
      String q = getValue(myString, ',', 4);
 
      NhietDo = l.toInt();
      KhiGas = m.toInt();
      Doam = n.toInt();
      Lua = o.toInt();
      Rada = q.toInt();

      myString = "";
// end new code
    }
  }

}

void sensorvalue1()
{
  int sdata = NhietDo;
  Blynk.virtualWrite(V2, sdata);
}
void sensorvalue2()
{
  int sdata = KhiGas;
  Blynk.virtualWrite(V3, sdata);
}
void sensorvalue3()
{
  int sdata = Doam;
  Blynk.virtualWrite(V10, sdata);
}
void sensorvalue4()
{
  int sdata = Lua;
}
void sensorvalue5()
{
  int sdata = Rada;
  Blynk.virtualWrite(V16, sdata);
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void handleTimerID1()
{
  sensorvalue1();
  Serial.print(" nhiet do: ");
  Serial.println(NhietDo);
  sensorvalue2();
  Serial.print(" khi gas: ");
  Serial.println(KhiGas);
  sensorvalue3();
  Serial.print("do am: ");
  Serial.println(Doam);
  Serial.print("Lua: ");
  Serial.println(Lua);
  sensorvalue5();
  Serial.print("khoang cach: ");
  Serial.println(Rada);
  String s;
  if (digitalRead(cbcua) == LOW)
  {
    s = " cửa đóng ";
  }
  else
  {
    s = " cửa mở ";
  }
  Blynk.virtualWrite(V11, s);
  if(runMode==1)
  {
    if(KhiGas > 200)
    {
      if(canhbaoState==HIGH)
      {
        digitalWrite(fan, 1);
        canhbaoState=LOW;
        Blynk.logEvent("canhbaokhigas", String("Cảnh báo! Khí gas = " + String(KhiGas)+" vượt quá mức cho phép!"));
        timerID2 = timer.setTimeout(60000L, handleTimerID2);
      }
      else
      {
        canhbaoState = HIGH;
      }
    }
    else if(NhietDo > 40)
    {
      if(canhbaoState==HIGH)
      {
        digitalWrite(fan, 1);
        canhbaoState=LOW;
        Blynk.logEvent("canhbaonhietdo", String("Cảnh báo! Nhiệt độ = " + String(NhietDo)+" vượt quá mức cho phép!"));
        timerID2 = timer.setTimeout(60000L, handleTimerID2);
      }
      else
      {
        canhbaoState = HIGH;
      }
    }
    else if(Doam > 80)
    {
      if(canhbaoState==HIGH)
      {
        digitalWrite(fan, 1);
        canhbaoState=LOW;
        Blynk.logEvent("canhbaodoam", String("Cảnh báo! Độ ẩm = " + String(Doam)+" vượt quá mức cho phép!"));
        timerID2 = timer.setTimeout(60000L, handleTimerID2);
      }
      else
      {
        canhbaoState = HIGH;
      }
    }
    else if(Lua < 500)
    {
      if(canhbaoState==HIGH)
      {
        digitalWrite(fan, 1);
        canhbaoState=LOW;
        Blynk.logEvent("canhbaolua", String("Cảnh báo! Có xảy ra hỏa hoạn!"));
        timerID2 = timer.setTimeout(60000L, handleTimerID2);
      }
      else
      {
        canhbaoState = HIGH;
      }
    }
    else if(Rada < 10)
    {
      if(canhbaoState==HIGH)
      {
        canhbaoState=LOW;
        Blynk.logEvent("canhbaorada", String("Cảnh báo! Cảnh báo có người tiếp cận!"));
        timerID2 = timer.setTimeout(60000L, handleTimerID2);
      }
      else
      {
        canhbaoState = HIGH;
      }
    }
    else
    {
      digitalWrite(fan, 0);
    }
  }
  else
  {
    Blynk.virtualWrite(V8,LOW);
    Serial.println("Đã tắt cảnh báo!");
  }
  if (runModecbcua==1)
  {
    if (digitalRead(cbcua)==HIGH)
    {
      if(canhbaoStatecbcua==HIGH)
      {
        canhbaoStatecbcua=LOW;
        Blynk.logEvent("canhbaocua", String(" Cửa đang mở !"));
        timerID3 = timer.setTimeout(60000L, handleTimerID3);
      }
      else
      {
        canhbaoStatecbcua = HIGH;
      }
    }
  }
  else
  {
    Blynk.virtualWrite(V12,LOW);
  }

  /////// hồng ngoại ////////
  if (digitalRead(ledhongngoai) == 0)
  {
    digitalWrite(ledsan, HIGH);
    Blynk.virtualWrite(V7, HIGH);
  }
  else
  {
    digitalWrite(ledsan, LOW);
    Blynk.virtualWrite(V7, LOW);
  }
  /////////////////////////////
}
void handleTimerID2()
{
  canhbaoState = 0;
}
void handleTimerID3()
{
  canhbaoStatecbcua=0;
}