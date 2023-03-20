#define FAIN1 28
#define FAIN2 30
#define FBIN1 24
#define FBIN2 22
#define FPWMB 6
#define FSTBY 26
#define FPWMA 10
#define BAIN1 34
#define BAIN2 32
#define BBIN1 40
#define BBIN2 39
#define BPWMA 5
#define BPWMB 9
#define BSTBY 36

void setup() {
  pinMode(FAIN1, OUTPUT); //设置引脚FAIN1为输出
  pinMode(FAIN2, OUTPUT); //设置引脚FAIN2为输出
  pinMode(FBIN1, OUTPUT); //设置引脚FBIN1为输出
  pinMode(FBIN2, OUTPUT); //设置引脚FBIN2为输出
  pinMode(FPWMA, OUTPUT); //设置引脚FPWMA为输出
  pinMode(FPWMB, OUTPUT); //设置引脚FPWMB为输出
  pinMode(FSTBY, OUTPUT); //设置引脚FSTBY为输出
  pinMode(BAIN1, OUTPUT); //设置引脚BAIN1为输出
  pinMode(BAIN2, OUTPUT); //设置引脚BAIN2为输出
  pinMode(BBIN1, OUTPUT); //设置引脚BBIN1为输出
  pinMode(BBIN2, OUTPUT); //设置引脚BBIN2为输出
  pinMode(BPWMA, OUTPUT); //设置引脚BPWMA为输出
  pinMode(BPWMB, OUTPUT); //设置引脚BPWMB为输出
  pinMode(BSTBY, OUTPUT); //设置引脚BSTBY为输出
}

void moveForward(int duration, int speed) {
  digitalWrite(FAIN1, HIGH); //设置引脚FAIN1为高电平
  digitalWrite(FAIN2, LOW); //设置引脚FAIN2为低电平
  digitalWrite(FBIN1, HIGH); //设置引脚FBIN1为高电平
  digitalWrite(FBIN2, LOW); //设置引脚FBIN2为低电平
  analogWrite(FPWMA, speed); //设置引脚FPWMA的PWM输出为speed
  analogWrite(FPWMB, speed); //设置引脚FPWMB的PWM输出为speed
  digitalWrite(FSTBY, HIGH); //设置引脚FSTBY为高电平
  digitalWrite(BAIN1, HIGH); //设置引脚BAIN1为高电平
  digitalWrite(BAIN2, LOW); //设置引脚BAIN2为低电平
  digitalWrite(BBIN1, HIGH); //设置引脚BBIN1为高电平
  digitalWrite(BBIN2, LOW); //设置引脚BBIN2为低电平
  analogWrite(BPWMA, speed); //设置引脚BPWMA的PWM输出为speed
  analogWrite(BPWMB, speed); //设置引脚BPWMB的PWM输出为speed
  digitalWrite(BSTBY, HIGH); //设置引脚BSTBY为高电平
  delay(duration); //延时duration毫秒
}

void moveBackward(int duration, int speed) {
  digitalWrite(FAIN1, LOW); //设置引脚FAIN1为低电平
  digitalWrite(FAIN2, HIGH); //设置引脚FAIN2为高电平
  digitalWrite(FBIN1, HIGH); //设置引脚FBIN1为高电平
  digitalWrite(FBIN2, LOW); //设置引脚FBIN2为低电平
  analogWrite(FPWMA, speed); //设置引脚FPWMA的PWM输出为speed
  analogWrite(FPWMB, speed); //设置引脚FPWMB的PWM输出为speed
  digitalWrite(FSTBY, HIGH); //设置引脚FSTBY为高电平
  digitalWrite(BAIN1, LOW); //设置引脚BAIN1为低电平
  digitalWrite(BAIN2, HIGH); //设置引脚BAIN2为高电平
  digitalWrite(BBIN1, HIGH); //设置引脚BBIN1为高电平
  digitalWrite(BBIN2, LOW); //设置引脚BBIN2为低电平
  analogWrite(BPWMA, speed); //设置引脚BPWMA的PWM输出为speed
  analogWrite(BPWMB, speed); //设置引脚BPWMB的PWM输出为speed
  digitalWrite(BSTBY, HIGH); //设置引脚BSTBY为高电平
  delay(duration); //延时duration毫秒
}

void moveLeft(int duration, int speed) {
  digitalWrite(FAIN1, LOW); //设置引脚FAIN1为低电平
  digitalWrite(FAIN2, HIGH); //设置引脚FAIN2为高电平
  digitalWrite(FBIN1, LOW); //设置引脚FBIN1为低电平
  digitalWrite(FBIN2, HIGH); //设置引脚FBIN2为高电平
  analogWrite(FPWMA, speed); //设置引脚FPWMA的PWM输出为speed
  analogWrite(FPWMB, speed); //设置引脚FPWMB的PWM输出为speed
  digitalWrite(FSTBY, HIGH); //设置引脚FSTBY为高电平
  digitalWrite(BAIN1, LOW); //设置引脚BAIN1为低电平
  digitalWrite(BAIN2, HIGH); //设置引脚BAIN2为高电平
  digitalWrite(BBIN1, LOW); //设置引脚BBIN1为低电平
  digitalWrite(BBIN2, HIGH); //设置引脚BBIN2为高电平
  analogWrite(BPWMA, speed); //设置引脚BPWMA的PWM输出为speed
  analogWrite(BPWMB, speed); //设置引脚BPWMB的PWM输出为speed
  digitalWrite(BSTBY, HIGH); //设置引脚BSTBY为高电平
  delay(duration); //延时duration毫秒
}

void moveRight(int duration, int speed) {
  digitalWrite(FAIN1, HIGH); //设置引脚FAIN1为高电平
  digitalWrite(FAIN2, LOW); //设置引脚FAIN2为低电平
  digitalWrite(FBIN1, HIGH); //设置引脚FBIN1为高电平
  digitalWrite(FBIN2, LOW); //设置引脚FBIN2为低电平
  analogWrite(FPWMA, speed); //设置引脚FPWMA的PWM输出为speed
  analogWrite(FPWMB, speed); //设置引脚FPWMB的PWM输出为speed
  digitalWrite(FSTBY, HIGH); //设置引脚FSTBY为高电平
  digitalWrite(BAIN1, HIGH); //设置引脚BAIN1为高电平
  digitalWrite(BAIN2, LOW); //设置引脚BAIN2为低电平
  digitalWrite(BBIN1, HIGH); //设置引脚BBIN1为高电平
  digitalWrite(BBIN2, LOW); //设置引脚BBIN2为低电平
  analogWrite(BPWMA, speed); //设置引脚BPWMA的PWM输出为speed
  analogWrite(BPWMB, speed); //设置引脚BPWMB的PWM输出为speed
  digitalWrite(BSTBY, HIGH); //设置引脚BSTBY为高电平
  delay(duration); //延时duration毫秒
}

void loop() {
  moveForward(1000, 255); //向前移动1000毫秒，速度为255
  // moveBackward(1000, 255); //向后移动1000毫秒，速度为255
  // moveLeft(1000, 255); //向左移动1000毫秒，速度为255
  // moveRight(1000, 255); //向右移动1000毫秒，速度为255
}