#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
void setup(){
   Serial.begin(9600);
 }
char var;
void loop(){
   while(Serial.available()>0)//当有信号的时候
   {
     var=Serial.read();
     
    //  if(var=='0')//传过来的是0
    //    digitalWrite(13,LOW);
    //  if(var=='1')//传过来的是1
    //    digitalWrite(13,HIGH);
   }
}

// 假设你接收到了两个字节48和57
byte b1 = 48;
byte b2 = 57;
// 把它们合并成一个整数
int num = (b1 << 8) | b2;
// num的值就是12345
