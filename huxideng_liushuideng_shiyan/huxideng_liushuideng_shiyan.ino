#include <SCoop.h>

defineTask(Task1);    //多线程，第一部分流水灯
void Task1::setup()
{
  for(int i=4;i<=8;i++)   //4号至7号引脚设置为输出
  {
    pinMode(i,OUTPUT);
  }
}

void Task1::loop()
{
  for(int i=4;i<8;i++)    //4号至7号引脚依次亮起熄灭
  {
    digitalWrite(i,HIGH);
    delay(1000);
    digitalWrite(i,LOW);
  }
  for(int i=6;i>4;i--)    //6号至5号引脚依次亮起熄灭
  {
    digitalWrite(i,HIGH);
    delay(100);
    digitalWrite(i,LOW);
  }
}

defineTaskLoop(Task2)   //多线程，第二部分为渐变灯
{
  int pinMode=3;    //定义3号引脚
  for(int fadeValue=0;fadeValue<=255;fadeValue+=5)    //亮度逐渐增大
  {
    analogWrite(pinMode,fadeValue);
    delay(30);
  }
  for(int fadeValue=255;fadeValue>=0;fadeValue-=5)    //亮度逐渐减小
  {
    analogWrite(pinMode,fadeValue);
    delay(30);
  }
}

void setup()
{
  mySCoop.start();
}

void loop()
{
  yield();
}
