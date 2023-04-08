//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,8,1);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
  init_lcd();
}

void loop()
{
  print_lcd("Hello, world!");
}

void init_lcd()
{
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("init");
}

void print_lcd(String message)
{
  lcd.print(message);
}