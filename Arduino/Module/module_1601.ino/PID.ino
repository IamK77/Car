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
// 定义霍尔编码器的引脚
// #define encoder_A 2 // 中断引脚0
// #define encoder_B 3 // 中断引脚1

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
// AB电机是反的，记得改

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

// 定义用于存储测量速度和PWM输出的变量
int rpm = 0;
int pwm = 0;

// 定义用于存储PID控制器的误差和输出的变量
float error = 0;
float error1 = 0;
float error2 = 0;
float PwmControl = 0;

struct rpm_val {
  float FA_rpm;
  float FB_rpm;
  float BA_rpm;
  float BB_rpm;
};

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  

  // 设置编码器引脚为输入模式，并启用中断函数
  pinMode(FA_A, INPUT_PULLUP);
  pinMode(FA_B, INPUT_PULLUP);
  pinMode(FB_A, INPUT_PULLUP);
  pinMode(FB_B, INPUT_PULLUP);
  pinMode(BA_A, INPUT_PULLUP);
  pinMode(BA_B, INPUT_PULLUP);
  pinMode(BB_A, INPUT_PULLUP);
  pinMode(BB_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FA_A), ISR_Encoder('FA_A'), CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(FA_B), ISR_Encoder('FA_B'), CHANGE); // B相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(FB_A), ISR_Encoder('FB_A'), CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(FB_B), ISR_Encoder('FB_B'), CHANGE); // B相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BA_A), ISR_Encoder('BA_A'), CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BA_B), ISR_Encoder('BA_B'), CHANGE); // B相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BB_A), ISR_Encoder('BB_A'), CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(BB_B), ISR_Encoder('BB_B'), CHANGE); // B相位变化时触发中断
  int pins[] = {BAIN1, BAIN2, BBIN1, BBIN2, BPWMA, BPWMB, BSTBY, FAIN1, FAIN2, FBIN1, FBIN2, FPWMA, FPWMB, FSTBY};
  int pins_size = sizeof(pins) / sizeof(pins[0]);
  for (int i = 0; i < pins_size; i++) {
    pinMode(pins[i], OUTPUT); //设置引脚为输出
  }

}

void loop() {
  
  

}

struct rpm_val rpm_get(){
  struct rpm_val rpm_return;
  rpm_return.FA_rpm = (float)((encoder_value.FA_encoderCount / PPR) * (1000 / interval) * 60);
  rpm_return.FB_rpm = (float)((encoder_value.FB_encoderCount / PPR) * (1000 / interval) * 60);
  rpm_return.BA_rpm = (float)((encoder_value.BA_encoderCount / PPR) * (1000 / interval) * 60);
  rpm_return.BB_rpm = (float)((encoder_value.BB_encoderCount / PPR) * (1000 / interval) * 60);
  return rpm_return;
}

void PID_C(){
  // 获取当前时间
  currentMillis = millis();

  // 如果时间间隔达到设定值，则进行测速和控制
  if (currentMillis - previousMillis > interval) {  //放到loop

    // 更新上一次时间
    previousMillis = currentMillis;

    // 计算测量速度，单位为转每分钟
    struct rpm_val rpm;
    rpm = rpm_get();

    // 计算误差，即目标速度与测量速度的差值
    error2 = error1;
    error1 = error;
    error = setRPM - rpm;

    // 计算PID控制器的输出，即PWM调节值
    PwmControl += Kp * (error - error1) + Ki * error + Kd * (error - 2 * error1 + error2);

    // 将PID控制器的输出映射到PWM范围内，即0-255之间
    pwm = map(PwmControl, -60, 60, -255, 255);

    // 如果PWM值超出范围，则限制在范围内
    if (pwm > 255) {
      pwm = 255;
    }
    if (pwm < -255) {
      pwm = -255;
    }

    // // 如果PWM值为正，则设置电机正转，否则设置电机反转
    // if (pwm > 0) {
    //   digitalWrite(in3, HIGH);
    //   digitalWrite(in4, LOW);
    //   analogWrite(enB, pwm); // 输出PWM值给电机驱动
    //   } else {
    //   digitalWrite(in3, LOW);
    //   digitalWrite(in4, HIGH);
    //   analogWrite(enB, -pwm); // 输出PWM值给电机驱动
    // }

    // 重置编码器计数，为下一次测量做准备
    encoderCount = 0;

    // 在串口监视器上显示目标速度，测量速度和PWM值
    Serial.print("Set RPM: ");
    Serial.print(setRPM);
    Serial.print('\t');
    Serial.print("Measured RPM: ");
    Serial.print(rpm);
    Serial.print('\t');
    Serial.print("Current PWM: ");
    Serial.println(pwm);

  }
}

// 编码器A相位中断函数，根据B相位的状态判断旋转方向，并增加或减少计数
void ISR_Encoder(char encoder) {
  if (encoder == 'FA_A') {
    if (digitalRead(FA_B) == HIGH) {
      encoder_value.FA_encoderCount--;
    } else {
      encoder_value.FA_encoderCount++;
    }
  }
  if (encoder == 'FA_B') {
    if (digitalRead(FA_A) == HIGH) {
      encoder_value.FA_encoderCount++;
    } else {
      encoder_value.FA_encoderCount--;
    }
  }
  if (encoder == 'FB_A') {
    if (digitalRead(FB_B) == HIGH) {
      encoder_value.FB_encoderCount--;
    } else {
      encoder_value.FB_encoderCount++;
    }
  }
  if (encoder == 'FB_B') {
    if (digitalRead(FB_A) == HIGH) {
      encoder_value.FB_encoderCount++;
    } else {
      encoder_value.FB_encoderCount--;
    }
  }
  if (encoder == 'BA_A') {
    if (digitalRead(BA_B) == HIGH) {
      encoder_value.BA_encoderCount--;
    } else {
      encoder_value.BA_encoderCount++;
    }
  }
  if (encoder == 'BA_B') {
    if (digitalRead(BA_A) == HIGH) {
      encoder_value.BA_encoderCount++;
    } else {
      encoder_value.BA_encoderCount--;
    }
  }
  if (encoder == 'BB_A') {
    if (digitalRead(BB_B) == HIGH) {
      encoder_value.BB_encoderCount--;
    } else {
      encoder_value.BB_encoderCount++;
    }
  }
  if (encoder == 'BB_B') {
    if (digitalRead(BB_A) == HIGH) {
      encoder_value.BB_encoderCount++;
    } else {
      encoder_value.BB_encoderCount--;
    }
  }
}

// 编码器B相位中断函数，根据A相位的状态判断旋转方向，并增加或减少计数
// void ISR_EncoderB() {
//   if (digitalRead(encoder_A) == HIGH) {
//     encoderCount--;
//   } else {
//     encoderCount++;
//   }
// }