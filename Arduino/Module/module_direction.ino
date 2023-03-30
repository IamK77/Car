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
#define FAIN1 24   //左上电机a1111
#define FAIN2 22
#define FPWMA 2
#define FSTBY 3

#define FBIN1 26     //右上电机b
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

#define FORWARD {1, 0}
#define BACKWARD {0, 1}

int FPwmA,FPwmB,BPwmA,BPwmB;
double V;
// {左前 右前 右后 左后}
// 定义方向数组
int forward[4][2] {FORWARD, FORWARD, FORWARD, FORWARD}; // 前进
int backward[4][2] {BACKWARD, BACKWARD, BACKWARD, BACKWARD}; // 后退
int left[4][2] {FORWARD, BACKWARD, FORWARD, BACKWARD}; // 左转
int right[4][2] {BACKWARD, FORWARD, BACKWARD, FORWARD}; // 右转
int forward_left[4][2] {FORWARD, FORWARD, FORWARD, FORWARD}; // 左前方
int forward_right[4][2] {FORWARD, FORWARD, FORWARD, BACKWARD}; // 右前方
int backward_left[4][2] {BACKWARD, BACKWARD, BACKWARD, BACKWARD}; // 左后方
int backward_right[4][2] {BACKWARD, BACKWARD, BACKWARD, BACKWARD}; // 右后方
int speed[4] = {50, 50, 50, 50};
int speed_A[4] = {255, 0, 255, 0};    //for forward_left and backward_right
int speed_B[4] = {0, 255, 0, 255};    //for forward_right and backward_left

// 
void setup() {
  int pins[] = {BAIN1, BAIN2, BBIN1, BBIN2, BPWMA, BPWMB, BSTBY, FAIN1, FAIN2, FBIN1, FBIN2, FPWMA, FPWMB, FSTBY};
  int pins_size = sizeof(pins) / sizeof(pins[0]);
  for (int i = 0; i < pins_size; i++) {
    pinMode(pins[i], OUTPUT); //设置引脚为输出
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

void loop() {
  move(1000, speed, forward); //向前移动1000毫秒，速度为255
  // direction[0] = BACKWARD;
  // direction[1] = BACKWARD;
  move(100000, {50,50,50,50}, direction); //向后移动1000毫秒，速度为255
  // direction[0] = LEFT;
  // direction[1] = RIGHT;
  // move(1000, speed, direction); //向左移动1000毫秒，速度为255
  // direction[0] = RIGHT;
  // direction[1] = LEFT;
  // move(1000, speed, direction); //向右移动1000毫秒，速度为255
}