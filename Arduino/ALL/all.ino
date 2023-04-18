/*
 * Author: chenyinfeng
 * Date: 2023/4/8
 * Description: The Function For Control TB6612FNG And McCnaum
 * 开发约定:
 *        FA为前A轮，即左前轮，受FPWMA控制
 *        FB为前B轮，即右前轮
 *        BA为后A轮，即右后轮
 *        BB为后B轮，即左后轮
 *        非PWM引脚均使用arduino 并排的数字引脚
 *        传参中速度为数组, FA使用0, FB使用1, BA使用2, BB使用3
 *        IN1给HIGH, IN2给LOW 为正转    反之为反转
 *        FA使用dirc 0  FB 1  BA 2  BB 3
 *TODO: 整合特殊方向的速度数组
 */

// TODO AB电机是反的，记得改
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,1);
String comdata = "";

#define FAIN1 24   //右前电机a
#define FAIN2 22
#define FPWMA 2
#define FSTBY 3
#define FA_A 23 
#define FA_B 25 

#define FBIN1 26     //左前电机b
#define FBIN2 28
#define FPWMB 4 
#define FB_A 27 
#define FB_B 29 

#define BAIN1 32    //左后电机a
#define BAIN2 30
#define BPWMA 5  
#define BSTBY 6
#define BA_A 31 
#define BA_B 33 

#define BBIN2 34   //右后b
#define BBIN1 36
#define BPWMB 7
#define BB_A 35 
#define BB_B 37 

#define Voltage A0 //使用模拟引脚

// 定义每圈的脉冲数，根据电机的型号和减速比确定
#define PPR 1560 // 13 * 120

// 定义定时器的时间间隔，单位为毫秒
#define interval 50

// 定义目标速度，单位为转每分钟
#define setRPM 1000

// 定义PID控制器的参数，根据实际情况调整
#define Kp 50
#define Ki 9
#define Kd 0.1

// 定义方向数组
#define FORWARD {1, 0}
#define BACKWARD {0, 1}


// 定义用于存储编码器计数的变量，使用volatile修饰符
struct encoder_val {
  volatile long FA_encoderCount = 0;
  volatile long FB_encoderCount = 0;
  volatile long BA_encoderCount = 0;
  volatile long BB_encoderCount = 0;
};
struct encoder_val encoder_value;

// 定义用于存储当前时间和上一次时间的变量
long currentMillis = 0;
long previousMillis = 0;

// 用struct定义用于PWM输出的变量
struct pwm_val {
  int FA_pwm = 0;
  int FB_pwm = 0;
  int BA_pwm = 0;
  int BB_pwm = 0;
};

// 定义用于存储PID控制器的误差和输出的变量
struct error {
  float FA_error = 0;
  float FA_error1 = 0;
  float FA_error2 = 0;
  float FA_PwmControl = 0;
  float FB_error = 0;
  float FB_error1 = 0;
  float FB_error2 = 0;
  float FB_PwmControl = 0;
  float BA_error = 0;
  float BA_error1 = 0;
  float BA_error2 = 0;
  float BA_PwmControl = 0;
  float BB_error = 0;
  float BB_error1 = 0;
  float BB_error2 = 0;
  float BB_PwmControl = 0;
};
struct error error_value;

struct rpm_val {
  float FA_rpm;
  float FB_rpm;
  float BA_rpm;
  float BB_rpm;
};

SoftwareSerial mySerial(2, 3); // RX, TX

char var;


// 定义方向数组  {左前 右前 右后 左后}
int forward[4][2] {FORWARD, FORWARD, FORWARD, FORWARD}; // 前进
int backward[4][2] {BACKWARD, BACKWARD, BACKWARD, BACKWARD}; // 后退
int left[4][2] {FORWARD, BACKWARD, FORWARD, BACKWARD}; // 左转
int right[4][2] {BACKWARD, FORWARD, BACKWARD, FORWARD}; // 右转
int forward_left[4][2] {FORWARD, FORWARD, FORWARD, FORWARD}; // 左前方
int forward_right[4][2] {FORWARD, FORWARD, FORWARD, BACKWARD}; // 右前方
int backward_left[4][2] {BACKWARD, BACKWARD, BACKWARD, BACKWARD}; // 左后方
int backward_right[4][2] {BACKWARD, BACKWARD, BACKWARD, BACKWARD}; // 右后方
int speed[4] = {255, 255, 255, 255};
int speed_A[4] = {255, 0, 255, 0};    //for forward_left and backward_right
int speed_B[4] = {0, 255, 0, 255};    //for forward_right and backward_left


void setup(){
   // 初始化串口通信
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight();
  lcd.print("init");

  // 设置编码器引脚为输入模式，并启用中断函数
  pinMode(FA_A, INPUT_PULLUP);
  pinMode(FA_B, INPUT_PULLUP);
  pinMode(FB_A, INPUT_PULLUP);
  pinMode(FB_B, INPUT_PULLUP);
  pinMode(BA_A, INPUT_PULLUP);
  pinMode(BA_B, INPUT_PULLUP);
  pinMode(BB_A, INPUT_PULLUP);
  pinMode(BB_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FA_A), ISR_Encoder_FA_A, CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(FA_B), ISR_Encoder_FA_B, CHANGE); // B相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(FB_A), ISR_Encoder_FB_A, CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(FB_B), ISR_Encoder_FB_B, CHANGE); // B相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BA_A), ISR_Encoder_BA_A, CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BA_B), ISR_Encoder_BA_B, CHANGE); // B相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BB_A), ISR_Encoder_BB_A, CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BB_B), ISR_Encoder_BB_B, CHANGE); // B相位变化时触发中断
  int pins[] = {BAIN1, BAIN2, BBIN1, BBIN2, BPWMA, BPWMB, BSTBY, FAIN1, FAIN2, FBIN1, FBIN2, FPWMA, FPWMB, FSTBY};
  int pins_size = sizeof(pins) / sizeof(pins[0]);
  for (int i = 0; i < pins_size; i++) {
    pinMode(pins[i], OUTPUT); //设置引脚为输出
  }
 }


void loop(){
   // 获取当前时间
  currentMillis = millis();
  // 如果时间间隔达到设定值，则进行测速和控制
  if (currentMillis - previousMillis > interval) {
    struct pwm_val PID = PID_C();
    int speed[] = {PID.FA_pwm, PID.FB_pwm, PID.BA_pwm, PID.BB_pwm};
    move(interval, speed, forward); //向前移动interval毫秒，速度为pwm
  }
  
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

struct rpm_val rpm_get(){
  struct rpm_val rpm_return;
  rpm_return.FA_rpm = (float)((encoder_value.FA_encoderCount / PPR) * (1000 / interval) * 60);
  rpm_return.FB_rpm = (float)((encoder_value.FB_encoderCount / PPR) * (1000 / interval) * 60);
  rpm_return.BA_rpm = (float)((encoder_value.BA_encoderCount / PPR) * (1000 / interval) * 60);
  rpm_return.BB_rpm = (float)((encoder_value.BB_encoderCount / PPR) * (1000 / interval) * 60);
  return rpm_return;
}

struct pwm_val PID_C(){
  struct pwm_val pwm_value;
  // 更新上一次时间
  previousMillis = currentMillis;

  // 计算测量速度，单位为转每分钟
  struct rpm_val rpm;
  rpm = rpm_get();

  // 计算误差，即目标速度与测量速度的差值
  error_value.FA_error2 = error_value.FA_error1;
  error_value.FA_error1 = error_value.FA_error;
  error_value.FA_error = setRPM - rpm.FA_rpm;
  error_value.FB_error2 = error_value.FB_error1;
  error_value.FB_error1 = error_value.FB_error;
  error_value.FB_error = setRPM - rpm.FB_rpm;
  error_value.BA_error2 = error_value.BA_error1;
  error_value.BA_error1 = error_value.BA_error;
  error_value.BA_error = setRPM - rpm.BA_rpm;
  error_value.BB_error2 = error_value.BB_error1;
  error_value.BB_error1 = error_value.BB_error;
  error_value.BB_error = setRPM - rpm.BB_rpm;

  // 计算PID控制器的输出，即PWM调节值
  // PwmControl += Kp * (error - error1) + Ki * error + Kd * (error - 2 * error1 + error2);
  error_value.FA_PwmControl += Kp * (error_value.FA_error - error_value.FA_error1) + Ki * error_value.FA_error + Kd * (error_value.FA_error - 2 * error_value.FA_error1 + error_value.FA_error2);
  error_value.FB_PwmControl += Kp * (error_value.FB_error - error_value.FB_error1) + Ki * error_value.FB_error + Kd * (error_value.FB_error - 2 * error_value.FB_error1 + error_value.FB_error2);
  error_value.BA_PwmControl += Kp * (error_value.BA_error - error_value.BA_error1) + Ki * error_value.BA_error + Kd * (error_value.BA_error - 2 * error_value.BA_error1 + error_value.BA_error2);
  error_value.BB_PwmControl += Kp * (error_value.BB_error - error_value.BB_error1) + Ki * error_value.BB_error + Kd * (error_value.BB_error - 2 * error_value.BB_error1 + error_value.BB_error2);


  // 将PID控制器的输出映射到PWM范围内，即0-255之间
  // pwm = map(PwmControl, -60, 60, -255, 255);
  pwm_value.FA_pwm = map(error_value.FA_PwmControl, -60, 60, -255, 255);
  pwm_value.FB_pwm = map(error_value.FB_PwmControl, -60, 60, -255, 255);
  pwm_value.BA_pwm = map(error_value.BA_PwmControl, -60, 60, -255, 255);
  pwm_value.BB_pwm = map(error_value.BB_PwmControl, -60, 60, -255, 255);


  // 如果PWM值超出范围，则限制在范围内
  if (pwm_value.FA_pwm > 255) {
    pwm_value.FA_pwm = 255;
  }
  if (pwm_value.FA_pwm < -255) {
    pwm_value.FA_pwm = -255;
  }
  if (pwm_value.FB_pwm > 255) {
    pwm_value.FB_pwm = 255;
  }
  if (pwm_value.FB_pwm < -255) {
    pwm_value.FB_pwm = -255;
  }
  if (pwm_value.BA_pwm > 255) {
    pwm_value.BA_pwm = 255;
  }
  if (pwm_value.BA_pwm < -255) {
    pwm_value.BA_pwm = -255;
  }
  if (pwm_value.BB_pwm > 255) {
    pwm_value.BB_pwm = 255;
  }
  if (pwm_value.BB_pwm < -255) {
    pwm_value.BB_pwm = -255;
  }


  // 重置编码器计数，为下一次测量做准备
  encoder_value.FA_encoderCount = 0;
  encoder_value.FB_encoderCount = 0;
  encoder_value.BA_encoderCount = 0;
  encoder_value.BB_encoderCount = 0;

  return pwm_value;
}

// 编码器A相位中断函数，根据B相位的状态判断旋转方向，并增加或减少计数 
// 但不能传参，请改成八个函数
void ISR_Encoder_FA_A() {
    if (digitalRead(FA_B) == HIGH) {
      encoder_value.FA_encoderCount--;
    } else {
      encoder_value.FA_encoderCount++;
    }
}
void ISR_Encoder_FA_B() {
    if (digitalRead(FA_A) == HIGH) {
      encoder_value.FA_encoderCount++;
    } else {
      encoder_value.FA_encoderCount--;
    }
}
void ISR_Encoder_FB_A() {
    if (digitalRead(FB_B) == HIGH) {
      encoder_value.FB_encoderCount--;
    } else {
      encoder_value.FB_encoderCount++;
    }
}
void ISR_Encoder_FB_B() {
    if (digitalRead(FB_A) == HIGH) {
      encoder_value.FB_encoderCount++;
    } else {
      encoder_value.FB_encoderCount--;
    }
}
void ISR_Encoder_BA_A() {
    if (digitalRead(BA_B) == HIGH) {
      encoder_value.BA_encoderCount--;
    } else {
      encoder_value.BA_encoderCount++;
    }
}
void ISR_Encoder_BA_B() {
    if (digitalRead(BA_A) == HIGH) {
      encoder_value.BA_encoderCount++;
    } else {
      encoder_value.BA_encoderCount--;
    }
}
void ISR_Encoder_BB_A() {
    if (digitalRead(BB_B) == HIGH) {
      encoder_value.BB_encoderCount--;
    } else {
      encoder_value.BB_encoderCount++;
    }
}
void ISR_Encoder_BB_B() {
    if (digitalRead(BB_A) == HIGH) {
      encoder_value.BB_encoderCount++;
    } else {
      encoder_value.BB_encoderCount--;
    }
}


void move(int duration, int speed[4], int direction[4][2]) {
  digitalWrite(BAIN1, direction[2][0]);
  digitalWrite(BAIN2, direction[2][1]);
  digitalWrite(BBIN1, direction[3][0]);
  digitalWrite(BBIN2, direction[3][1]);
  analogWrite(BPWMA, speed[2]); //设置引脚BPWMA的PWM输出为speed[2]
  analogWrite(BPWMB, speed[3]); //设置引脚BPWMB的PWM输出为speed[3]
  digitalWrite(BSTBY, 1); //设置引脚BSTBY为高电平
  digitalWrite(FAIN1, direction[0][0]);
  digitalWrite(FAIN2, direction[0][1]);
  digitalWrite(FBIN1, direction[1][0]);
  digitalWrite(FBIN2, direction[1][1]);
  analogWrite(FPWMA, speed[0]); //设置引脚FPWMA的PWM输出为speed[0]
  analogWrite(FPWMB, speed[1]); //设置引脚FPWMB的PWM输出为speed[1]
  digitalWrite(FSTBY, 1); //设置引脚FSTBY为高电平
  delay(duration); //延时duration毫秒
}



