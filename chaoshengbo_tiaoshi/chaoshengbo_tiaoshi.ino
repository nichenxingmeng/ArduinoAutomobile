const int TrigPin = 7; 
const int EchoPin = 4;  
float cm; 

void setup() 
{  
  Serial.begin(9600); 
  pinMode(TrigPin, OUTPUT); 
  pinMode(EchoPin, INPUT);  
} 

void loop()  
{ 
  digitalWrite(TrigPin, LOW);  
  delayMicroseconds(2);  
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(TrigPin, LOW);   //数据读取
  cm = pulseIn(EchoPin, HIGH) / 58.0;
  cm = (int(cm * 100.0)) / 100.0;   //数据换算
  Serial.print(cm);   
  Serial.print("cm");   
  Serial.println();   //数据输出
  delay(1000);    //设置读取数据间隔
} 
