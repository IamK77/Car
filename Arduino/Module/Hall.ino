// 霍尔编码器 arduino 代码
// 参考来源：https://blog.csdn.net/robotixworkshop/article/details/114275629

// Motor encoder output pulse per rotation (change as required)
// 每圈的脉冲数量，可以从数据手册中查找
int PPR= 3;

// Encoder output to Arduino Interrupt pin
// 编码器的A相位接Arduino的D2.
int encoder_A= 2;

// L298n Motor Driver
// 马达驱动接线：D5控制PWM, D6与D7控制转动方向
int enB= 5;
int in3= 6;
int in4= 7;

// Analog pin for potentiometer
// 电位器接线：用来控制目标转速（不是PWM）
int pot = 0;

// Pulse count from encoder
// 用来储存脉冲数量
volatile long encoderCount = 0;

// One-second interval for measurements
// 每一秒进行一次测量与控制，可改小，ie: interval=100;
int interval = 1000;

// Counters for milliseconds during interval
// 用来计算时间
long previousMillis = 0;
long currentMillis = 0;

// Variable for RPM measuerment
//rpm: 实际测到的转速， setRPM:目标转速
int rpm = 0;
int setRPM=0;

// Variable for PWM motor speed output
// PWM初始值, 可以设为 0-255 之间的任意值
int pwm = 50;

void setup(){
  Serial.begin(9600);
  pinMode(encoder_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder_A), ISR_Encoder, RISING);
  previousMillis = millis();
}

void loop(){
  // Set desired RPM
  // 把电位器读数转换成目标转速（2000到10000转每分钟）
  setRPM = map(analogRead(pot), 0, 1023, 2000, 10000);

  //Set motor direction
  // 马达转动方向
  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);

  // Update RPM value every second
  // 每一秒更新一次测量转速
  currentMillis = millis();
  
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    
    // Calculate RPM
    // 计算 rpm = (float)((encoderCount/PPR)*(1000/interval)*60);

    // 串口监视器上显示目标，Arduino 输出的 PWM，以及测量到的RPM
    Serial.print("Set RPM: ");
    Serial.print(setRPM);
    Serial.print('\t');
    Serial.print("Measured RPM: ");
    Serial.print(rpm);
    Serial.print('\t');
    Serial.print("Crrent PWM: ");
    Serial.println(pwm);

    //Bang Bang control
    // 起停式控制了
    
    if(rpm < setRPM){
      pwm++;
    }
    
    if(rpm > setRPM){
      pwm--;
    }
    
    else{
      pwm=pwm;
    }
    
    analogWrite(enB, pwm); //调整马达转速
    
    //pulse count reset
    // 累计脉冲数量重置,为下一秒测量做准备
    
    encoderCount = 0;
    
   }
}

// Interupt service routine.

void ISR_Encoder(){
  
encoderCount++;

}