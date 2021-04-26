int pinI1=8;    //定义I1接口
int pinI2=9;    //定义I2接口
int speedpin=11;    //定义EA(PWM调速)接口
int pinI3=6;    //定义I3接口
int pinI4=7;    //定义I4接口
int speedpin1=10;   //定义EB(PWM调速)接口
void setup()
{
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(speedpin,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(speedpin1,OUTPUT);
}
void loop()
{
 //前进
     analogWrite(speedpin,100);   //输入模拟值进行设定速度
     analogWrite(speedpin1,100);
     digitalWrite(pinI4,LOW);   //使直流电机（右）逆时针转
     digitalWrite(pinI3,HIGH);
     digitalWrite(pinI1,LOW);   //使直流电机（左）顺时针转
     digitalWrite(pinI2,HIGH);
     delay(2000);
 //后退
     analogWrite(speedpin,100);   //输入模拟值进行设定速度
     analogWrite(speedpin1,100);
     digitalWrite(pinI4,HIGH);    //使直流电机（右）顺时针转
     digitalWrite(pinI3,LOW);
     digitalWrite(pinI1,HIGH);    //使直流电机（左）逆时针转
     digitalWrite(pinI2,LOW);
     delay(2000);
 //左转
     analogWrite(speedpin,60);    //输入模拟值进行设定速度
     analogWrite(speedpin1,60);
     digitalWrite(pinI4,LOW);   //使直流电机（右）逆时针转
     digitalWrite(pinI3,HIGH);
     digitalWrite(pinI1,HIGH);    //使直流电机（左）逆时针转
     digitalWrite(pinI2,LOW);
     delay(2000);
 //右转
     analogWrite(speedpin,60);    //输入模拟值进行设定速度
     analogWrite(speedpin1,60);
     digitalWrite(pinI4,HIGH);    //使直流电机（右）顺时针转
     digitalWrite(pinI3,LOW);
     digitalWrite(pinI1,LOW);   //使直流电机（左）顺时针转
     digitalWrite(pinI2,HIGH);
     delay(2000);
 //刹车
     digitalWrite(pinI4,HIGH);    //使直流电机（右）刹车
     digitalWrite(pinI3,HIGH);
     digitalWrite(pinI1,HIGH);    //使直流电机（左）刹车
     digitalWrite(pinI2,HIGH);
     delay(2000);
 }
