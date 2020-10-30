// LCD - 16 Servo controller
//Disaster of Puppets 2020
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal_I2C.h>
/*

LCD Screen

Buttons show 1 when open, 0 when pressed

SDA - A5 (currently in use by PWM) 
SCL - A4 (currently in use by PWM) 
Don't know why but I had to switch these for LCD to work which seems to contradict online schematics

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

int runonce = 0;
int Sangle;
bool Fninety = false;
bool Fmanual = false;
bool Fsweep = false;
bool forward;

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

  lcd.setCursor(0, 0);  // Move the cursor at origin
  lcd.print("1 Set to Neutral");
  lcd.setCursor(0, 1);
  lcd.print("2 Sweep Test");
  lcd.setCursor(0, 2);
  lcd.print("3 Live Set");
  lcd.setCursor(0, 3);
  lcd.print("1     2     3     4");

}

void loop(){

   menu();
}

   

int angleToPulse(int ang) 
{
  int pulse = map(ang,0,180, SERVOMIN,SERVOMAX);
  return pulse;
}


void menu(){

  Button1 = digitalRead(Button1Pin);
  Button2 = digitalRead(Button2Pin);
  Button3 = digitalRead(Button3Pin);
  Button4 = digitalRead(Button4Pin);

  //Serial.print("Button 1 : ");
  //Serial.println(Button1);
  //Serial.print("Button 2 : ");
  //Serial.println(Button2);
  //Serial.print("Button 3 : ");
  //Serial.println(Button3);
  //Serial.print("Button 4 : ");
  //Serial.println(Button4);
  //delay(400);
  
  //Serial.println("Function Main Menu");
  lcd.setCursor(0, 0);  // Move the cursor at origin
  lcd.print("1 Set to Neutral");
  lcd.setCursor(0, 1);
  lcd.print("2 Sweep Test");
  lcd.setCursor(0, 2);
  lcd.print("3 Live Set");
  lcd.setCursor(0, 3);
  lcd.print("1     2     3      ");
 
  
if (Button1 == 0){
 //Serial.println("You pressed button 1 in the menu function");
 Fninety = true;
 runonce = 0;
 ninety();
}
  else if (Button2 == 0){
      //Serial.println("You pressed button 2 in the menu function");
      Fsweep = true;  
      runonce = 0;
      sweep();
  }
    else if (Button3 == 0){

      //Serial.println("You pressed button 3 in the menu function");
      Fmanual = true;
      runonce = 0;
      Sangle = 0;
      manual();
  }
    else if (Button4 == 0){ 
      //Serial.println("You pressed button 4 in the menu function");
      menu();
      }
  
}

void ninety(){

       while (Fninety == true){
           Button4 = digitalRead(Button4Pin);
           //if (runonce == 0) {
           //Serial.println("Servos set to 90");
           //}
           lcd.setCursor(0, 0);  // Move the cursor at origin
           lcd.print("  Servos set to 90  ");
           lcd.setCursor(0, 1);
           lcd.print("                    ");
           lcd.setCursor(0, 2);
           lcd.print("                    ");
           lcd.setCursor(0, 3);
           lcd.print("                Exit");
           
                      //Servo action
            if (runonce == 0){
                   for (int i = 0; i <= 15; i++) {
                   pwm.setPWM(i,0,angleToPulse(90));
                   delay(10);
                   runonce = 1;
           }                              
       }

          if (Button4 == 0){
            Fninety = false;
          }
  }
  clearscreen();
}


void sweep(){
  
        while (Fsweep == true){
          
            Button4 = digitalRead(Button4Pin);  
           // Serial.println("Function Servos Sweep");
            //Serial.print("Button 4 : ");
            //Serial.println(Button4);
            lcd.setCursor(0, 0);  // Move the cursor at origin
            lcd.print("Sweep in Progress   ");
            lcd.setCursor(0, 1);
            lcd.print("                    ");
            lcd.setCursor(0, 2);
            lcd.print("                    ");
            lcd.setCursor(0, 3);
            lcd.print("                Exit");

            if (Button4 == 0){
              Fsweep = false;
              }
         
    
          if (forward == true) {
          for (int i = 0; i <= 180; i++){
               for (int s = 0; s <= 15; s++){ 
                    pwm.setPWM(s,0,angleToPulse(i));
                   }
                forward = false;
                }
                
                
               
          }
          else if (forward == false) {
          for (int i = 180; i >= 0; i--){
               for (int s = 0; s <= 15; s++){ 
                    pwm.setPWM(s,0,angleToPulse(i));
                    }
                    forward = true;
                }
          }



    }   
    clearscreen();  
}

void manual(){

clearscreen();
delay(20);

       while (Fmanual == true){
          Button1 = digitalRead(Button1Pin); 
          Button2 = digitalRead(Button2Pin); 
          Button3 = digitalRead(Button3Pin); 
          Button4 = digitalRead(Button4Pin); 

            lcd.setCursor(0, 0);  // Move the cursor at origin
            lcd.print("Adjust with - & +   ");
            lcd.setCursor(0, 1);
            lcd.print("Current Angle :");
            lcd.setCursor(8, 2);
            lcd.print(Sangle);
            lcd.setCursor(0, 3);
            lcd.print("90    -      +  Exit");
          

            if (Button1 == 0){
              Sangle = 90;
                 for (int i = 0; i <= 15; i++) {
                        pwm.setPWM(i,0,angleToPulse(90));
                        delay(10);
                        
                 }
            }

            if (Button2 == 0){
              if (Sangle >= 1){
                  Sangle = Sangle -1;
                  for (int i = 0; i <= 15; i++) {
                          pwm.setPWM(i,0,angleToPulse(Sangle));
                          lcd.setCursor(8, 2);
                          lcd.print("   ");
                    }
              } 
            }

            if (Button3 == 0){
              if (Sangle <= 179){            
                 Sangle = Sangle +1;
                 for (int i = 0; i <= 15; i++) {
                          pwm.setPWM(i,0,angleToPulse(Sangle));
                          lcd.setCursor(8, 2);
                          lcd.print("   ");
                    }
              }
            }

              if (Button4 == 0){
              Fmanual = false;
              }

  }
    clearscreen();    
}

void clearscreen(){
  lcd.setCursor(0, 0);  // Move the cursor at origin
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("1     2     3       ");
}      
