// 定义霍尔编码器的引脚
#define encoder_A 2 // 中断引脚0
#define encoder_B 3 // 中断引脚1

// 定义电机驱动的引脚
#define enB 5 // PWM引脚
#define in3 6 // 方向引脚
#define in4 7 // 方向引脚

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
volatile long encoderCount = 0;

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

void setup() {
  // 初始化串口通信
  Serial.begin(9600);

  // 设置编码器引脚为输入模式，并启用中断函数
  pinMode(encoder_A, INPUT_PULLUP);
  pinMode(encoder_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder_A), ISR_EncoderA, CHANGE); // A相位变化时触发中断
  attachInterrupt(digitalPinToInterrupt(encoder_B), ISR_EncoderB, CHANGE); // B相位变化时触发中断

  // 设置电机驱动引脚为输出模式，并初始化方向和PWM值
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in3, HIGH); // 设置电机正转
  digitalWrite(in4, LOW);
  analogWrite(enB, pwm); // 设置PWM初始值

}

void loop() {
  
  

}

void PID_C(){
  // 获取当前时间
  currentMillis = millis();

  // 如果时间间隔达到设定值，则进行测速和控制
  if (currentMillis - previousMillis > interval) {

    // 更新上一次时间
    previousMillis = currentMillis;

    // 计算测量速度，单位为转每分钟
    rpm = (float)((encoderCount / PPR) * (1000 / interval) * 60);

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

    // 如果PWM值为正，则设置电机正转，否则设置电机反转
    if (pwm > 0) {
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enB, pwm); // 输出PWM值给电机驱动
      } else {
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enB, -pwm); // 输出PWM值给电机驱动
    }

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
void ISR_EncoderA() {
  if (digitalRead(encoder_B) == HIGH) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

// 编码器B相位中断函数，根据A相位的状态判断旋转方向，并增加或减少计数
void ISR_EncoderB() {
  if (digitalRead(encoder_A) == HIGH) {
    encoderCount--;
  } else {
    encoderCount++;
  }
}