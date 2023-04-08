#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,1);
String comdata = "";

SoftwareSerial mySerial(2, 3); // RX, TX
void setup(){
    Serial.begin(9600);
    lcd.init(); 
    lcd.backlight();
    lcd.print("init");
 }
char var;
void loop(){
   while(Serial.available()>0)//当有信号的时候
   {
    //将接收到的数据拼接成一串字符串 
    comdata += char(Serial.read());
    delay(2);
   }
   if (comdata.length() > 0)
   {
   Serial.print(var);
   lcd.init();
   lcd.print(var);
   }
}





