//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,8,1);  // set the LCD address to 0x27 for a 16 chars and 2 line display
String message = ""; //定义接收数据的字符串
bool received = false; //定义一个标志位，表示是否已经接收过字符串

void setup() {
  Serial.begin(9600); //初始化串口通信
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("init");
}

void lcd(){
  if (Serial.available() > 0) { //当有可用数据时
    if (!received) { //如果还没有接收过字符串
      message = Serial.readString(); //读取一个字符串
      received = true; //将标志位设为真
    }
    lcd.init();
    lcd.print(message); //打印接收到的字符串
    Serial.flush(); //清空缓冲区
  }
}

void loop() {
  
}

