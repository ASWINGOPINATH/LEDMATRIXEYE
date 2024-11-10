#include <SoftwareSerial.h>
#include <LedControl.h>
#include<Servo.h>
SoftwareSerial gsm(3,4);
Servo leftArm;
Servo rightArm;
Servo neckServo;
int DIN = 8;
int CS = 4;
int CLK  = 10;
LedControl lc=LedControl(DIN, CLK, CS,0);

int ON1[8] ={
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
  B00000000 };
int ON[8] ={B00111100,B01111110,B11111111,B11100111,B11100111,B11111111,B01111110,B00111100  };
byte OFF [8]={B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111110};

unsigned long eyeTime,neckTime,currentTime;

void setup() {
  pinMode(2,INPUT);
  attachInterrupt(digitalPinToInterrupt(2),emergency,FALLING);
  leftArm.attach(9);
  rightArm.attach(6);
  neckServo.attach(5);
  gsm.begin(9600);
  gsm.println("AT");
  leftArm.write(0);
  rightArm.write(0);
  neckServo.write(90);
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
  for(int i=7;i>=0;i--)
  {
    lc.setRow(0,i,ON1[i]);
    delay(50);
  }
  eyeTime=neckTime=millis();
}

void loop() {
  currentTime=millis();
  if((currentTime-eyeTime)>=5000)
    eye();
   if((currentTime-neckTime)>=60000)
    neck();
}
void eye()
{
  for(int i=0;i<8;i++)
    { 
      lc.setRow(0,i,OFF[i]);
      delay(10);
    }
    for(int i=7;i>=0;i--)
    {
      lc.setRow(0,i,ON[i]);
      delay(10);
    }
    eyeTime=millis();
}
void neck()
{
  for(int i=90,j=0;i>=0 && j<=90;i--,j++)
  {
    neckServo.write(i);
    rightArm.write(j);
    delay(30);
  }
  delay(50);
  for(int i=0,j=90;i<=90 && j>=0;i++,j--)
  {
    neckServo.write(i);
    rightArm.write(j);
    delay(30);
  }
  for(int i=90,j=0;i<=180 && j<=90;i++,j++)
  {
    neckServo.write(i);
    leftArm.write(j);
    delay(30);
  }
  delay(50);
  for(int i=180,j=90;i>=90 && j>=0;i--,j--)
  {
    neckServo.write(i);
    leftArm.write(j);
    delay(30);
  }
  neckTime=millis();
}
void emergency()
{
  gsm.println("ATD+ +919072545692");
  delay(20000);
  gsm.println("ATH");
}
