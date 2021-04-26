//将遥控器不同按键的红外编码设置成对应动作所接受的信号
#define IR_FORWARD  0x18
#define IR_BACKWARD 0x52

#define IR_LEFT     0x08
#define IR_RIGHT    0x5A

#define IR_UP       0x45
#define IR_DOWN     0x46

#define IR_PAUSE    0x1C
#define IR_STOP     0x47

//导入相关库
#include <SCoop.h>    //多线程库
#include <IRLremote.h>    //红外遥控库
#include <L298N.h>    //L298N电机库

CNec IRLremote;

#define pinIR 2   //设置2号引脚为
#define pinLED LED_BUILTIN    //设置13号引脚为LED灯亮

const int TrigPin = 7;    //设置超声波模块信号输入为5号引脚
const int EchoPin = 4;    //设置超声波模块信号输出为4号引脚

unsigned short New_command = 0;
unsigned short Curr_Cmd;    //初始化红外接收参数
bool Pause = false;
bool Direction = true;    //初始化方向与暂停
unsigned long Last_Time;
unsigned long Curr_Time;    //定义时间点
void Motor_Stop();
void Motor_Pause();   //函数说明

float cm;   //定义距离

//定义L298N电机引脚
#define ENA 11      
#define IN1 6        
#define IN2 5        
#define ENB 10       
#define IN3 9         
#define IN4 8

//设置速度
#define MIN_SPEED 55 
#define MAX_SPEED 100
#define INC_SPEED 10

//L298N电机设置
L298N motorA(ENA, IN1, IN2);
L298N motorB(ENB, IN3, IN4);

//初始化速度，为最小速度
short speedA = MIN_SPEED;
short speedB = MIN_SPEED;

//超声波测距循环
defineTask(Task1);    //多线程实现，两个函数分别循环，此处定义第一个循环
void Task1::setup()   //第一个循环的设置
{  
    Serial.begin(9600); 
    pinMode(TrigPin, OUTPUT); 
    pinMode(EchoPin, INPUT);    //设置超声波模块输入输出
} 

//函数说明
int Get_IR_Command();
void Motor_Forward();
void Motor_Backward();
void Motor_SpeedUp();
void Motor_SpeedDown();
void Motor_Left();
void Motor_Right();

void Task1::loop()    //第一个循环的循环体
{ 
    digitalWrite(TrigPin, LOW);  
    delayMicroseconds(2);  
    digitalWrite(TrigPin, HIGH); 
    delayMicroseconds(10);  
    digitalWrite(TrigPin, LOW);   //超声波输入
    cm = pulseIn(EchoPin, HIGH) / 58.0;
    cm = (int(cm * 100.0)) / 100.0;   //距离输出
    if(cm < 20)
    {
      Motor_Backward();
      delay(1000);    //保持后退1.5s
      Motor_Left();
      delay(1500);    //保持左转1.5s
      Motor_Forward();
    }   //判断距离，做出相应动作
} 

//红外遥控循环
defineTask(Task2);    //多线程实现，两个函数分别循环，此处定义第二个循环
void Task2::setup()   //第二个循环的设置
{
    Serial.begin(9600);
    Serial.println(F("Setup"));
    pinMode(pinLED, OUTPUT);
    if (!IRLremote.begin(pinIR))
        Serial.println(F("You did not choose a valid pin.")); 
    //设置红外遥控模块输入输出
        
    Last_Time = millis();   //时间点初始化获取

    motorA.setSpeed(speedA);
    motorB.setSpeed(speedB);    //设置速度
}

void Task2::loop()    //第二个循环的循环体
{
    Curr_Time = millis();
    if((Curr_Time - Last_Time) > 200)   //判断两次操作时间，避免误差
    {
      if (Curr_Cmd = Get_IR_Command())
      {
        if (Curr_Cmd == IR_UP)        
            Motor_SpeedUp();
        else if (Curr_Cmd == IR_DOWN)
            Motor_SpeedDown();
        else if (Curr_Cmd == IR_LEFT) 
            Motor_Left();
        else if (Curr_Cmd == IR_RIGHT) 
            Motor_Right();       
        else if (Curr_Cmd == IR_STOP) 
            Motor_Stop();
        else if (Curr_Cmd == IR_PAUSE)
            Motor_Pause();
        else if (Curr_Cmd == IR_FORWARD) 
            Motor_Forward();
        else if (Curr_Cmd == IR_BACKWARD)
            Motor_Backward();
      }   //红外接收判断，做出相应动作
    Last_Time = millis();
    }
}

void setup()
{
  mySCoop.start();
}
void loop()
{
  yield();
}   //多线程实现，主函数

int Get_IR_Command()    //获取红外信号
{
    if (IRLremote.available()) 
    {
        digitalWrite(pinLED, HIGH);
        auto IRData = IRLremote.read();
        if (IRData.command != 0)
        {
            New_command = IRData.command;
            Serial.println();
            Serial.print(F(" Cmd: 0x"));
            Serial.print(IRData.command, HEX);
        }
        else
        {
            Serial.print(" R");
        }
        delay(50);
        digitalWrite(pinLED, LOW);
        return New_command;
    }
    return 0;
}

void Speed_Min(short &M,short &N)   //速度变为最小值，即恢复至初始状态
{
    M = MIN_SPEED;
    N = MIN_SPEED;
}

void Compare(short &M,short &N)   //速度比较，使其在合理范围内
{
    if (M > MAX_SPEED)
        M = MAX_SPEED;
    if (N > MAX_SPEED)
        N = MAX_SPEED;
    if (M < MIN_SPEED)
        M = MIN_SPEED;
    if (N < MIN_SPEED)
        N = MIN_SPEED;
}

void Set(short &M,short &N)   //速度设置
{
    motorA.setSpeed(M);
    motorB.setSpeed(N);
}

void Motor_Run()    //电机驱动
{  
    if(Direction == true)   //方向判断，决定电机正反转
    {
      motorA.forward();
      motorB.forward();
    }
    else
    {
      motorA.backward();
      motorB.backward();
    }
}

void Motor_SpeedUp()    //加速
{
    speedA += INC_SPEED;    //每次改变设定值
    speedB += INC_SPEED;
    Compare(speedA,speedB);
    Set(speedA,speedB);
    Motor_Run();
}

void Motor_SpeedDown()    //减速
{
    speedA -= INC_SPEED;    //每次改变设定值
    speedB -= INC_SPEED;
    Compare(speedA,speedB);
    Set(speedA,speedB);
    Motor_Run();
}

void Motor_Left()   //左转弯
{   
    speedA = 70;
    speedB = 70;
    Set(speedA,speedB);
    motorA.backward();
    motorB.forward();   //两电机正反转实现
}

void Motor_Right()    //右转弯
{
    speedA = 70;
    speedB = 70;                                                                                                                                                                                                                                                                                                0;
    Set(speedA,speedB);
    motorB.backward();
    motorA.forward();   //两电机正反转实现
}

void Motor_Stop()   //停止
{
    Speed_Min(speedA,speedB);   //停止前将电机速度恢复初始值
    Set(speedA,speedB);
    motorA.stop();
    motorB.stop();
    delay(100);
}

void Motor_Pause()    //暂停
{
  if(Pause == true)   //判断小车状态                   
  {
    Speed_Min(speedA,speedB);
    Set(speedA,speedB);
    Pause=false;                      
  }
  else
  {
    Set(speedA,speedB);
    Pause=true;
  }
}

void Motor_Forward()    //前进
{
    Speed_Min(speedA,speedB);
    Set(speedA,speedB);
    Direction = true;
    Motor_Run();
}

void Motor_Backward()   //后退
{
    Speed_Min(speedA,speedB);
    Set(speedA,speedB);
    Direction = false;
    Motor_Run();
}
