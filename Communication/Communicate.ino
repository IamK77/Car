#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // 等待串口连接。仅适用于本机USB端口
  }

  Serial.println("Goodnight moon!");

  // 设置SoftwareSerial端口的数据速率
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
