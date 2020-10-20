// LCD - 16 Servo controller
//Disaster of Puppets 2020
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal_I2C.h>
/*

LCD Screen

Buttons show 1 when open, 0 when pressed

Need to figure out how to change to use A0 and A1 instead
SDA - A4 (currently in use by PWM)
SCL - A5 (currently in use by PWM)

Buttons
D2
D4
D6
D9

*/
int Button1Pin = 2;
int Button2Pin = 4;
int Button3Pin = 6;
int Button4Pin = 9;

int Button1;
int Button2;
int Button3;
int Button4;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

//int pos =0;
//int pos1 =180;

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  620 // this is the 'maximum' pulse length count (out of 4096)
// 125/620 for the 9g 
//125/610 for HobbyKing™ High Torque Servo MG/BB W/Proof 12.8kg / 0.22sec / 58g
void setup(){
       
Serial.begin(115200);

pinMode(Button1Pin, INPUT_PULLUP);
pinMode(Button2Pin, INPUT_PULLUP);
pinMode(Button3Pin, INPUT_PULLUP);
pinMode(Button4Pin, INPUT_PULLUP);

       
pwm.begin();
pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

lcd.init();
lcd.begin(20,4);
lcd.backlight();

// Set default display

lcd.setCursor(0, 0);  // Move the cursor at origin
lcd.print("1 Set to Neutral");
lcd.setCursor(0, 1);
lcd.print("2 Sweep Test");
lcd.setCursor(0, 2);
lcd.print("3 Live Set");
lcd.setCursor(0, 3);
lcd.print("1     2     3");

}

void loop(){
Button1 = digitalRead(Button1Pin);
Button2 = digitalRead(Button2Pin);
Button3 = digitalRead(Button3Pin);
Button4 = digitalRead(Button4Pin);
  
/*
Serial.print("Button 1 : ");
Serial.println(Button1);
Serial.print("Button 2 : ");
Serial.println(Button2);
Serial.print("Button 3 : ");
Serial.println(Button3);
Serial.print("Button 4 : ");
Serial.println(Button4);
delay(300);
*/
Serial.println("Main Menu");
delay(300);

  if (Button1 == 0) {
    ninety();
  }
  
  if (Button2 == 0) {
    sweep();
  }
  
  if (Button3 == 0) {
    manual();
  }
  
}
   

int angleToPulse(int ang) 
{
  int pulse = map(ang,0,180, SERVOMIN,SERVOMAX);
  return pulse;
}

void sweep(){
       Serial.println("Servos Sweep");
       clearscreen();
       
       for (int i = 0; i <= 180; i++){
          for (int s = 0; s <= 15; s++){ 
            pwm.setPWM(s,0,angleToPulse(i));
          delay(100);
          }
          delay(100);
       }
                   
       lcd.setCursor(0, 0);  // Move the cursor at origin
       lcd.print("Servo Sweep in Progress..");
       lcd.setCursor(0, 2);
       lcd.print("Press Button 4 to Return");

       Button4 = digitalRead(Button4Pin);

  if (Button4 == 0) {
    menu();
  }
       
}

void ninety(){
       Button4 = digitalRead(Button4Pin);
       Serial.println("Servos set to 90");
       clearscreen();
       lcd.setCursor(0, 0);  // Move the cursor at origin
       lcd.print("All Servos Moved to 90");
       lcd.setCursor(0, 2);
       lcd.print("Press Button 4 to Return");
       for (int i = 0; i <= 15; i++) {
       pwm.setPWM(i,0,angleToPulse(90));
       delay(300);    
 
      if (Button4 == 0) {
        menu();
        }
         
       }
}

void manual(){

       for (int i = 0; i <= 15; i++){
       pwm.setPWM(i,0,angleToPulse(180));
       delay(100);

       Button4 = digitalRead(Button4Pin);

         if (Button4 == 0) {
    menu();
  }
       }
}
void clearscreen(){
  lcd.setCursor(0, 0);  // Move the cursor at origin
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("1     2     3     4");
  delay(100);
}      

void menu(){
  Serial.print("Main Menu");
  lcd.setCursor(0, 0);  // Move the cursor at origin
  lcd.print("1 Set to Neutral");
  lcd.setCursor(0, 1);
  lcd.print("2 Sweep Test");
  lcd.setCursor(0, 2);
  lcd.print("3 Live Set");
  lcd.setCursor(0, 3);
  lcd.print("1     2     3");
}
