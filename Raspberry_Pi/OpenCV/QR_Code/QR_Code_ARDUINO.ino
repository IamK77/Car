#include <Adafruit_GFX.h> //需要使用Adafruit_GFX库
#include <Adafruit_SSD1306.h> //需要使用Adafruit_SSD1306库
#include <QRCode.h> //需要使用QRCode库

#define OLED_RESET 4
//需要使用OLED_RESET引脚
Adafruit_SSD1306 display(OLED_RESET); //需要使用SSD1306显示屏

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //需要使用0x3C地址
  display.clearDisplay(); //清空显示屏
  display.display(); //显示屏显示
}

void loop() {
  // 传输图片
  // 清空显示屏
  display.clearDisplay(); //清空显示屏
  // 设置字体大小和颜色
  display.setTextSize(1); //设置字体大小
  display.setTextColor(WHITE); //设置字体颜色
  // 设置光标位置
  display.setCursor(0, 0); //设置光标位置
  // 在显示屏上打印图片
  display.println("Insert image here"); //在显示屏上打印图片
  // 显示图片
  display.display(); //显示屏显示
}
