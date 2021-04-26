#include <IRremote.h>
int RECV_PIN = 11;
 
IRrecv irrecv(RECV_PIN);    //定义接收引脚为11
 
decode_results results;   
//声明一个IRremote库函数独有的变量类型(decode_resultc)
 
void setup()
{
  Serial.begin(9600);
  //假如启动过程出现问题，提示用户启动失败
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}
 
void loop() {
  if (irrecv.decode(&results)) 
  { 
    //检查是否接收到红外遥控信号
    Serial.println(results.value, HEX);   //输出指令信息
    irrecv.resume();    //接收下一指令
  }
  delay(100);
}
