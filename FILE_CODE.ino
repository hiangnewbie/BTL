#include <TimerOne.h>
#include <ServoTimer2.h> 
ServoTimer2 servo2;
#define IR2 5

#define S0     8
#define S1     7
#define S2     3
#define S3     4
#define OUT    2 
#define RELAY  12
#define LED 9

int   g_count = 0;                                                                       // count the frequecy
int   g_array[3];                                                                          // store the RGB value
int   g_flag = 0;                                                                           // filter of RGB queue
float g_SF[3];                                                                              // save the RGB Scale factor
unsigned char ketqua = 0;
unsigned char black = 0, white = 0, other = 0;
void TSC_Init()
{
  pinMode(S0, OUTPUT); // set ouput, input 
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT); 
  digitalWrite(S0, LOW);// OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH); 
}
// Select the filter color
void TSC_FilterColor(int Level01, int Level02)
{
  if(Level01 != 0)
    Level01 = HIGH;
    
  if(Level02 != 0)
    Level02 = HIGH;
    
  digitalWrite(S2, Level01); 
  digitalWrite(S3, Level02); 
}
void TSC_Count()
{
  g_count ++ ;
}
void TSC_Callback()
{
  switch(g_flag)
  {
    case 0:
//      Serial.println("->WB Start");
      TSC_WB(LOW, LOW);                                                       //Filter mau Red         
      break;
    case 1:
      g_array[0] = g_count;
      TSC_WB(HIGH, HIGH);                                                     //Filter mau Green        
      break;    
    case 2:
      g_array[1] = g_count;
      TSC_WB(LOW, HIGH);                                                     
      break; 
    case 3:
      g_array[2] = g_count;
      TSC_WB(HIGH, LOW);                                                        
      break;
    default:     
      g_count = 0;        
      break; 
  }
} 
 
 void TSC_WB(int Level0, int Level1)                                   
 {
   g_count = 0;
   g_flag ++;
   TSC_FilterColor(Level0, Level1);
   Timer1.setPeriod(1000000);
 }
void setup()
{
  pinMode(IR2, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  servo2.attach(11);      // ket noi servo vao chan 10
   
  TSC_Init();     //khoi dong cam bien mau
  Serial.begin(9600); 
  Timer1.initialize();                    // defaulte is 1s
  
  Timer1.attachInterrupt(TSC_Callback);   // gan ham vao ngat tran timer1
  attachInterrupt(0, TSC_Count, RISING);   //gan ham vao ngat canh len chan out cua cam bien mau
 
  delay(4000); 
  for(int i=0; i<3; i++)
//  Serial.println(g_array[i]);
  g_SF[0] = 255.0/ g_array[0]; //R lay thong so moi truong
  g_SF[1] = 255.0/ g_array[1] ;       //G lay thong so moi truong
  g_SF[2] = 255.0/ g_array[2] ;          //B lay thong so moi truong
  
  on_bangtai();
  off_led();
  return_servo(); // test servo va cho ve vi tri ngoai bang tai
}
void loop(){
  
  if(digitalRead(IR2) == 1){
    while(ketqua == 0){
      delay(220);
      off_bangtai();
      delay(500);
      on_led();      
      g_flag = 0; 
      delay(4000);   
      Serial.println("R");Serial.println(g_array[0]);
      Serial.println("G");Serial.println(g_array[1]);
      Serial.println("B");Serial.println(g_array[2]); 
      Serial.println(digitalRead(IR2));  
      if(g_array[0]> 200 && g_array[0]< 500 &&  g_array[0] > (g_array[1]) && g_array[0] < g_array[2]) {
        black++;
        black_servo();
        ketqua = 1;
        Serial.print("B");Serial.print(black);Serial.print("W");Serial.print(white);Serial.print("O");Serial.println(other);
      }
        else {
        other++;
        ketqua = 1;
        //Serial.print("B");Serial.print(black);Serial.print("W");Serial.print(white);Serial.print("O");Serial.println(other);
      }
    if(g_array[0]> 900 && g_array[0]< 1700 &&  g_array[0] > (g_array[1]) && g_array[0] < g_array[2]){
        ketqua = 1;
        white++;
        white_servo();
        Serial.print("B");Serial.print(black);Serial.print("W");Serial.print(white);Serial.print("O");Serial.println(other);
      }
      else {
        other++;
        ketqua = 1;
        //Serial.print("B");Serial.print(black);Serial.print("W");Serial.print(white);Serial.print("O");Serial.println(other);
      }
      
      off_led();
    }
  }
  if(ketqua == 1){
    ketqua = 0;
    on_bangtai();
    off_led();
    delay(4000);
    return_servo();
  }
  
}
void off_bangtai(){
  digitalWrite(RELAY, LOW);
//  Serial.println("OFF BTAI");
}
void on_bangtai(){
  digitalWrite(RELAY, HIGH);
//  Serial.println("ON BTAI");
}
void on_led(){
  digitalWrite(LED, HIGH);
//  Serial.println("ON LED");
}
void off_led(){
  digitalWrite(LED, LOW);
//  Serial.println("OFF LED");
}
void black_servo(){
  //delay(2000);
//  Serial.println("red servo");
}
void white_servo(){
  servo2.write(1500);
  delay(2000);
//  Serial.println("green servo");
}
void return_servo(){
  update_servo(1000,1000);
  
}
void update_servo(unsigned char ser1, unsigned char ser2)
{
  servo2.write(ser2);
  delay(2000);
}
