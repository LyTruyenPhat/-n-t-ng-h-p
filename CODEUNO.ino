#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h> // thư viện để sử dụng sensor DHT11
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // thư viên để sử dụng LCD
#include <Servo.h> // thư viện để sử dụng servo
#include <RTC.h> // thư viên để sử dụng REAL TIME

byte degree[8] = {0B01110,0B01010,0B01110,0B00000,0B00000,0B00000,0B00000,0B00000};

SoftwareSerial nodemcu(2,3);//Rx-2, Tx-3
const int DHTPIN = 7;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

const int RST_PIN = 9;
const int SS_PIN = 10;
MFRC522 mfrc522 (SS_PIN, RST_PIN);
int UID[4];
int i;
int ID1[4] = {115, 9, 212, 46};
int ID2[4] = {233, 183, 115, 242};
unsigned long thoigian;
int khoangcach;

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo;
static DS1307 RTC;

int sensormq5 = A0; // cảm biến gas/khói
int firesensor = A1; // cảm biến lửa
int buzz = 8; // loa
int servo = 6; // động cơ
int echo = 5; // cảm biến siêu âm
int trig = 4; // cảm biến siêu âm

int vitri = 0;
 
String cdata; 
 
void setup()
{
  Serial.begin(9600); 
  dht.begin();
  RTC.begin();
  nodemcu.begin(9600);
  RTC.startClock();
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, degree);
  pinMode(firesensor, INPUT);
  pinMode(sensormq5, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzz, OUTPUT);
  myservo.attach(servo);
  myservo.write(0);
}
 
void loop()
{
    
    float tempvalue = dht.readTemperature();
    float tempFvalue = dht.readTemperature(true);
    int humivalue = dht.readHumidity();
    float gasvalue = analogRead(sensormq5);
    int firevalue = analogRead(firesensor);
    khoang_cach();

    lcd.setCursor(0, 0);
    lcd.print("ND: ");
    lcd.setCursor(3, 0);
    lcd.print(tempvalue);
    lcd.setCursor(7, 0);
    lcd.write(1);
    lcd.print("C");
    lcd.setCursor(9, 0);
    lcd.print("|");
    lcd.setCursor(10, 0);
    lcd.print("GAS: ");
    lcd.setCursor(14, 0);
    lcd.print(gasvalue);
    lcd.setCursor(0, 1);
    lcd.print("DA: ");
    lcd.setCursor(4, 1);
    lcd.print(humivalue);
    lcd.setCursor(6, 1);
    lcd.print("%");
    lcd.setCursor(9, 1);
    lcd.print("|");
    lcd.setCursor(10, 1);
    lcd.print("Rada: ");
    lcd.setCursor(16, 1);
    lcd.print(khoangcach);
    lcd.setCursor(18, 1);
    lcd.print("cm");
 

  String day = String(RTC.getDay());
  String month = String(RTC.getMonth());
  String year = String(RTC.getYear());
  String hours = String(RTC.getHours());
  String minutes = String(RTC.getMinutes());
  String seconds = String(RTC.getSeconds());

  String days = String(day + "/" + month + "/" + year);
  String times = String(hours + ":" + minutes);

  lcd.setCursor(0, 2);
  lcd.print("DAY: ");
  lcd.setCursor(7, 2);
  lcd.print(days);
  lcd.setCursor(0, 3);
  lcd.print("TIME: ");
  lcd.setCursor(7, 3);
  lcd.print(times);
  Serial.print("Do am: ");
  Serial.println(humivalue);
  Serial.print("Nhiet do: ");
  Serial.println(tempvalue);
  Serial.print("NhietdoF: ");
  Serial.println(tempFvalue);
  Serial.print("Khi GAS: ");
  Serial.println(gasvalue);
  Serial.print("Lua: ");
  Serial.println(firevalue);
  Serial.print(" Khoang cach: ");
  Serial.println(khoangcach);
  if (tempvalue >= 40 || gasvalue >= 200 || firevalue < 200 || humivalue >= 80)
  {
    digitalWrite(buzz, HIGH);
    myservo.write(90);
  }
  else
  {
    digitalWrite(buzz, LOW);
    myservo.write(0);
  }
  
  cdata = cdata + tempvalue + "," + gasvalue + "," + humivalue + "," + firevalue + "," + khoangcach; // comma will be used a delimeter
  Serial.println(cdata); 
  nodemcu.println(cdata);
  //delay(200); // 100 milli seconds
  cdata = ""; 

  delay(1000);

  if ( ! mfrc522.PICC_IsNewCardPresent()) //Đọc dữ liệu thẻ hiện tại
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) //Đọc dữ liệu toàn bộ thẻ
  {
    return;
  }
  //Hiện thị dữ liệu ra cổng serial
  Serial.print("UID tag :");
  RFID();
}

void RFID()
{
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.println(UID[i]);
  }
  Serial.println(" ");
  if (UID[i] == ID1[i]) //change here the UID of the card/cards that you want to give access
  {
    for (vitri = 0; vitri <= 110; vitri +=1)
    {
      myservo.write(vitri);
      delay(5);
    }
    delay(5000);
    for (vitri = 110; vitri >=1; vitri-=1)
    {
      myservo.write(vitri);
      delay(5);
    }
    digitalWrite(buzz, LOW);
  }
  else if (UID[i] == ID2[i]) //change here the UID of the card/cards that you want to give access
  {
    for (vitri = 0; vitri <= 110; vitri +=1)
    {
      myservo.write(vitri);
      delay(5);
    }
    delay(5000);
    for (vitri = 110; vitri >=1; vitri-=1)
    {
      myservo.write(vitri);
      delay(5);
    }
    digitalWrite(buzz, LOW);
  }
  else   
  {
    digitalWrite(buzz, HIGH);
    delay(5000);
    digitalWrite(buzz, LOW);
  }
}
void khoang_cach()
{
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);

  thoigian = pulseIn(echo, HIGH);

  khoangcach = int (thoigian / 2 / 29.412);

  return khoangcach;
}
