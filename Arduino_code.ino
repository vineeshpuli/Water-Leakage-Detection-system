volatile int flow_frequency1; 
volatile int flow_frequency2;
int a=0;
float vol = 0.0,l1_minute,l2_minute;
unsigned char flowsensor1 = 2; // Sensor Input
unsigned char flowsensor2 = 3; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
#include <Wire.h>

void intToBytes(int x);           //converts int to bytes
byte b0,b1; 
int x;

#include <LiquidCrystal.h>
int Contrast=75;
LiquidCrystal lcd(12, 11, 5, 4, 10, 9);
void flow1 () // Interrupt function
{
   flow_frequency1++;
}

void flow2 () // Interrupt function
{
   flow_frequency2++;
}

void setup() {
  Wire.begin(8);  
  Wire.onRequest(requestEvent);
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogWrite(6,Contrast);
  lcd.begin(16, 2);
  attachInterrupt(digitalPinToInterrupt(flowsensor1), flow1, RISING); // Setup Interrupt
  attachInterrupt(digitalPinToInterrupt(flowsensor2), flow2, RISING); // Setup Interrupt
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Water Flow Meter");
  lcd.setCursor(0,1);
  lcd.print("Circuit Digest");
  delay(600);
  cloopTime = 0;
  //flow_frequency1=30;
  //flow_frequency2=15;
  

  
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  if((currentTime-cloopTime)>= 900)
  {
    cloopTime = currentTime;
    if((flow_frequency1||flow_frequency2 )!= 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l1_minute = (flow_frequency1 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      l2_minute = (flow_frequency2 / 7.5);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(l1_minute);
      lcd.print(" L/M");
      
      
      
      
      
      
      //l1_minute = l1_minute*5/60;
      
      
      vol = vol +((l1_minute*9)/600);
      /*
      lcd.setCursor(0,1);
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      */
      
      flow_frequency1 = 0; // Reset Counter
      flow_frequency2 = 0;
      
      Serial.print(l1_minute, DEC); // Print litres/hour
      
      Serial.println(" L/min");
      Serial.print(vol, DEC); // Print litres/hour
      
      Serial.println(" L");
      
      Serial.print(l2_minute, DEC); // Print litres/hour
      
      Serial.println(" L/min");
      if(l1_minute!=l2_minute){
        a=1;
        Serial.println("Hole in between l and 2 flow sensors");
        lcd.setCursor(0,1);
        lcd.print("Vol:");
        lcd.print(vol);
        lcd.print(" L");
        lcd.print(" ");
        lcd.print( a);
      }
      else{
        Serial.println("No Hole in between l and 2 flow sensors");
        a=0;
        lcd.setCursor(0,1);
        lcd.print("Vol:");
        lcd.print(vol);
        lcd.print(" L");
        lcd.print(" ");
        lcd.print( a);
      }
      
    }
    /*
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(flow_frequency1*flow_frequency2);
      lcd.print(" L/M");
      lcd.setCursor(0,1);
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
    }
    */
  
  else
  {
    
    lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(flow_frequency1*flow_frequency2);
      lcd.print(" L/M");
      l1_minute=(flow_frequency1)/7.5;
      l2_minute=(flow_frequency2)/7.5;
      a=0;
      lcd.setCursor(0,1);
      lcd.print("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      Serial.print(l1_minute, DEC); // Print litres/hour
      
      Serial.println(" L/min");
      Serial.print(vol, DEC); // Print litres/hour
      
      Serial.println(" L");
      Serial.print(l2_minute, DEC); // Print litres/hour
      
      Serial.println(" L/min");
  }
  }
}

void requestEvent() {
  
  x=l1_minute*100;
  intToBytes(x);
  Wire.write(b0); //send string on request 
  Wire.write(b1);
  //Serial.println(vol);
  x=vol*100;
  intToBytes(x);
  Wire.write(b0); //send string on request 
  Wire.write(b1);
  
  x=l2_minute*100;
  intToBytes(x);
  Wire.write(b0); //send string on request 
  Wire.write(b1);
  //Serial.println(vol);

  x=a;
  intToBytes(x);
  Wire.write(b0); //send string on request 
  Wire.write(b1);
  
}


void intToBytes(int x){
  b0=(x>>8);
  b1= x & 0xFF;
}
