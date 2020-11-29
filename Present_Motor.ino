const int trigPin = 3;
const int echoPin = 2;
long duration=0;
int distance=0;
int light=0;
int percentage=0;
int lpercentage=0;
// include the library code:
#include <LiquidCrystal.h>
#include<EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(0, 1, 8, 9, 10, 11);

//RESET FUNCTION
void(* resetFunc)(void)=0;

//WATER PERCENTAGE FUNCTION
void percentage1()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(10);
digitalWrite(trigPin, HIGH);
delayMicroseconds(20);
digitalWrite(trigPin, LOW);
delayMicroseconds(10);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
percentage=(100-distance)*1.25;
}

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);  
}


void loop() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STARTING....");
  delay(6000);

  for(;;)
  {
//CHECK WATER PERCENTAGE
int lcheck =0;
int fcheck =0;
light=analogRead(1);
percentage1();

//FOR ABNORMAL VALUE RESET SYSTEM
if(percentage>110)
{
  resetFunc();
}

int mem=EEPROM.read(0);
int t=0;

if(mem==1)
  {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TANK:");
  
if((percentage>-1)&&(percentage<101))
{
  lcd.setCursor(5, 0);
  lcd.print(percentage);
}

if(percentage<0)
{
  lcd.setCursor(5, 0);
  lcd.print(0);
}

if(percentage>100)
{
  lcd.setCursor(5, 0);
  lcd.print(100);
}
  lcd.setCursor(8, 0);
  lcd.print("% FULL");
  lcd.setCursor(0, 1);
  lcd.print("MOTOR:");
  lcd.setCursor(6, 1);
  lcd.print("ON");
}

  else
  {
    
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TANK:");

if((percentage>-1)&&(percentage<101))
{
  lcd.setCursor(5, 0);
  lcd.print(percentage);
}

if(percentage<0)
{
  lcd.setCursor(5, 0);
  lcd.print(0);
}

if(percentage>100)
{
  lcd.setCursor(5, 0);
  lcd.print(100);
}

  lcd.setCursor(8, 0);
  lcd.print("% FULL");
  lcd.setCursor(0, 1);
  lcd.print("MOTOR:");
  lcd.setCursor(6, 1);
  lcd.print("OFF");
  }
  
  delay(500);


//MOTOR START

if((percentage<1)||(mem==1))
{
  if(mem!=1)
  {
  for(int i=0;i<10;i++)
  {
    percentage1();
    
    lcheck+=percentage;
  }
  
lcheck=lcheck/10;
  }
  
if((lcheck<1)||(mem==1))
{
  digitalWrite(6,HIGH);
  EEPROM.write(0,1);
}
}


//MOTOR STOP

if((percentage>100))
{
  
  for(int i=0;i<10;i++)
  {
    percentage1();
    fcheck+=percentage;
  }
fcheck=fcheck/10;
  
  if((fcheck>100))
{
  digitalWrite(6,LOW);
  EEPROM.write(0,2);
}
}


//Indicator
if(digitalRead(6)==HIGH)
{
  digitalWrite(7,HIGH);
}
else
{
   digitalWrite(7,LOW);
}


//LIGHT ON
if(light<21)
{
  digitalWrite(5,HIGH);
}

//LIGHT OFF
if(light>41)
{
  digitalWrite(5,LOW);
}
  
}

}
