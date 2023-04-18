   /*
 * Author: quattro four
 * Date: 2023/3/20
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

#define FAIN1 24   //右上电机a1111
#define FAIN2 22
#define FPWMA 2
#define FSTBY 3

#define FBIN1 26     //左上电机b
#define FBIN2 28
#define FPWMB 4 

#define BAIN1 32    //左下电机c
#define BAIN2 30
#define BPWMA 5  
#define BSTBY 6

#define BBIN2 34   //D
#define BBIN1 36
#define BPWMB 7
#define Voltage A0 //使用模拟引脚

int FPwmA,FPwmB,BPwmA,BPwmB;
double V;

void setup(){
 
Serial.begin(9600);
 //TB6612电机驱动模块控制信号初始化
  pinMode(FAIN1,OUTPUT);//控制左上电机（A）的方向（PAIN1,PAIN2)
  pinMode(FAIN2,OUTPUT);
  pinMode(FPWMA,OUTPUT);//A电机PWM
  pinMode(FBIN1,OUTPUT);//控制右上电机（B）的方向（PBIN1,PBIN2)
  pinMode(FBIN2,OUTPUT);
  pinMode(FPWMB,OUTPUT);//B电机的PWM
  pinMode(BAIN1,OUTPUT);//控制左下电机（C）的方向（BPIN1,BPIN2)
  pinMode(BAIN2,OUTPUT);
  pinMode(BPWMA,OUTPUT);//C电机的PWM
  pinMode(BBIN1,OUTPUT);//控制右下电机(D)的方向（BBIN1,BBIN2)
  pinMode(BBIN2,OUTPUT);
  pinMode(BPWMB,OUTPUT);//D电机的PWM
  
  //初始化tb6612电机的驱动模块
  digitalWrite(FAIN1, 1);
  digitalWrite(FAIN2, 0);
  digitalWrite(FBIN1, 1);
  digitalWrite(FBIN2, 0);
  digitalWrite(BAIN1, 1);
  digitalWrite(BAIN2, 0);
  digitalWrite(BBIN1, 1);
  digitalWrite(BBIN2, 0);
  digitalWrite(FSTBY, 1);
  digitalWrite(BSTBY, 1);
  analogWrite(FPWMA, 0);
  analogWrite(FPWMB, 0);
  analogWrite(BPWMA, 0);
  analogWrite(BPWMB, 0);
}
void SetPWM(int motor, int pwm){
   if(motor==1&&pwm>=0)//motor=1代表控制电机A，pwm>=0则(AIN1, AIN2)=(1, 0)为正转
  {
    digitalWrite(FAIN1, 1);
    digitalWrite(FAIN2, 0);
    analogWrite(FPWMA, pwm);
  }
   else if(motor==1&&pwm<0)//motor=1代表控制电机A，pwm<0则(AIN1, AIN2)=(0, 1)为反转
  {
    digitalWrite(FAIN1, 0);
    digitalWrite(FAIN2, 1);
    analogWrite(FPWMA, -pwm);
  }
  else if(motor==2&&pwm>=0)//motor=2代表控制电机B，pwm>=0则(BIN1, BIN2)=(0, 1)为正转
  {
    digitalWrite(FBIN1, 0);
    digitalWrite(FBIN2, 1);
    analogWrite(FPWMB, pwm);
  }
  else if(motor==2&&pwm<0)//motor=2代表控制电机B，pwm<0则(BIN1, BIN2)=(1, 0)为反转
  {
    digitalWrite(FBIN1, 1);
    digitalWrite(FBIN2, 0);
    analogWrite(FPWMB, -pwm);
  }
   else if(motor==3&&pwm>=0)//motor=3代表控制电机c，pwm>=0则(BAIN1, BAIN2)=(0, 1)为正转
  {
    digitalWrite(BAIN1, 0);
    digitalWrite(BAIN2, 1);
    analogWrite(BPWMA, pwm);
  }
  else if(motor==3&&pwm<0)//motor=代表控制电机C，pwm<0则(BAIN1, BAIN2)=(1, 0)为反转
  {
    digitalWrite(BAIN1, 1);
    digitalWrite(BAIN2, 0);
    analogWrite(BPWMA, -pwm);
  }
  
   else if(motor==4&&pwm>=0)//motor=4代表控制电机D，pwm>=0则(BBIN1, BBIN2)=(0, 1)为正转
  {
    digitalWrite(BBIN1, 0);
    digitalWrite(BBIN2, 1);
    analogWrite(BPWMB, pwm);
  }
  else if(motor==4&&pwm<0)//motor=4代表控制电机D，pwm<0则(BBIN1, BBIN2)=(1, 0)为反转
  {
    digitalWrite(BBIN1, 1);
    digitalWrite(BBIN2, 0);
    analogWrite(BPWMB, -pwm);
  }
}
void up(){
  SetPWM(1,-51);  //右前
  SetPWM(2,57);   //左前
  SetPWM(3,-56);   //左后
  SetPWM(4,52);   //右后
}
void back(){
  SetPWM(1,51);
  SetPWM(2,-57);
  SetPWM(3,56);
  SetPWM(4,-52);
}
void TurnLeft(){
  SetPWM(1,-128);
  SetPWM(2,-128);
  SetPWM(3,128);
  SetPWM(4,128);
}
void TurnRight(){
  SetPWM(1,128);
  SetPWM(2,128);
  SetPWM(3,-128);
  SetPWM(4,-128);
}
void RightLine(){
   SetPWM(1,-65);//r1 前57
   SetPWM(2,-55);//l1   后
   SetPWM(3,-65);//l2 00000057
   SetPWM(4,-54);//r2  前
}
void LeftLine(){
   SetPWM(1,60);
   SetPWM(2,50);
   SetPWM(3,45);
   SetPWM(4,60);
}
void loop() 
{
  // V=analogRead(Voltage); //读取模拟引脚A0模拟量
  // Serial.print(V*0.05371);  //对模拟量转换并通过串口输出
  // Serial.println("V");
//  up(); 
//  delay(5000);
//  back();
//   delay(5000 );
  
  // TurnLeft();
  // delay(500);
  // TurnRight();
  // delay(500);
   RightLine();
   delay(5000);
   LeftLine();
  // delay(500);
  // LeftLine();
  // delay(500);
}