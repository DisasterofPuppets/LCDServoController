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

Functions
=========

ninety (Menu option #1)
=====
Sets all servos yo 90 degrees

sweep (Menu option #2)
=====
Performs 3 sweeps the servos from 0 - 180 degree


manual (Menu option #3)
=====
Sets all servos to 0 degrees
Button 1 cycles between 0 - 90 - 180 degrees
Button 2 subtracts 1 degree
Button 3 adds one degree
Button 4 Exits


*/
int Button1Pin = 2;
int Button2Pin = 4;
int Button3Pin = 6;
int Button4Pin = 9;

int Button1;
int Button2;
int Button3;
int Button4;

int Sangle;
int NewSangle;
bool Fninety = false;
bool Fmanual = false;
bool Fsweep = false;
bool forward;
int countdown;

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

countdown = 3;

  Button1 = digitalRead(Button1Pin);
  Button2 = digitalRead(Button2Pin);
  Button3 = digitalRead(Button3Pin);
  Button4 = digitalRead(Button4Pin);

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
 clearscreen();
 ninety();
}
  else if (Button2 == 0){
      //Serial.println("You pressed button 2 in the menu function");
      Fsweep = true;  
      sweep();
  }
    else if (Button3 == 0){

      //Serial.println("You pressed button 3 in the menu function");
      Fmanual = true;
      Sangle = 0;
        for (int i = 0; i <= 15; i++) {
              pwm.setPWM(i,0,angleToPulse(0));
        }      
      manual();
  }
    else if (Button4 == 0){ 
      //Serial.println("You pressed button 4 in the menu function");
      clearscreen();
      menu();
      }
  
}

void ninety(){

           Button4 = digitalRead(Button4Pin);
           lcd.setCursor(0, 0);  // Move the cursor at origin
           lcd.print("  Servos set to 90  ");
           lcd.setCursor(0, 1);
           lcd.print("                    ");
           lcd.setCursor(0, 2);
           lcd.print("                    ");
           lcd.setCursor(0, 3);
           lcd.print("                    ");
           
           //Servo action

           for (int i = 0; i <= 15; i++) {
           pwm.setPWM(i,0,angleToPulse(90));
           delay(10);
           }

     delay(1000);
     clearscreen();
}


void sweep(){

          lcd.setCursor(0, 0);  // Move the cursor at origin
          lcd.print(" Sweep in Progress ");
          lcd.setCursor(0, 1);
          lcd.print("                    ");
          lcd.setCursor(0, 2);
          lcd.print("                    ");
          lcd.setCursor(9, 2);
          lcd.print("3");
          lcd.setCursor(0, 3);
          lcd.print("                    ");

          //set to 180 before beginning sweep
          for (int s = 0; s <= 15; s++){ 
                 pwm.setPWM(s,0,angleToPulse(0));
                 delay(10);
            }
          

          while (countdown > 0){
           Button4 = digitalRead(Button4Pin);  
    
            for (int s = 0; s <= 15; s++){ 
                 pwm.setPWM(s,0,angleToPulse(0));
                 delay(100);
            }

            delay (400);
            
            for (int s= 0; s <=15; s++){
              pwm.setPWM(s,0,angleToPulse(180));
              delay(100);
            }
            
                 if (countdown !=0){
                 countdown = countdown -1;
                 }
  
                 lcd.setCursor(0, 0);  // Move the cursor at origin
                 lcd.print(" Sweep in Progress ");
                 lcd.setCursor(0, 1);
                 lcd.print("                    ");
                 lcd.setCursor(0, 2);
                 lcd.print("                    ");
                 lcd.setCursor(9, 2);
                 lcd.print(countdown);
                 lcd.setCursor(0, 3);
                 lcd.print("                    ");
                 }
     
     clearscreen(); 
     menu();   
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
          lcd.print(" Adjust with - & + ");
          lcd.setCursor(0, 1);
          lcd.print("   Current Angle :  ");
          lcd.setCursor(0, 2);
          lcd.print("                    ");
          lcd.setCursor(9, 2);
          lcd.print(Sangle);
          lcd.setCursor(0, 3);
          lcd.print("^90   -      +  Exit");
          
// If the First button (0-90-180) is pressed
          if (Button1 == 0){
// Check if the current angle is 0 degrees
            if (Sangle == 0){
// Make the new angle 90 degrees              
                NewSangle = 90;
            }
// Runs if the above angle was not 0. Checks if the current angle is 90 degrees            
            else if (Sangle == 90){
// Make the new angle 180 degrees                            
                NewSangle = 180;
            }
// Runs if the above angle was not 0 or 90. Means the current angle must me 180 degrees
            else if (Sangle == 180){
// Make the new angle 0 degrees               
                NewSangle = 0;
            }
            else {
              NewSangle = Sangle + 90;
              if (NewSangle >=180){
                NewSangle = 0;
              }
           }    
            Sangle = NewSangle;

// Move the Servo  
            for (int i = 0; i <= 15; i++) {
            pwm.setPWM(i,0,angleToPulse(Sangle));
            delay(10);
            }
}

// If pressing the - Button
          if (Button2 == 0){
            if (Sangle >= 1){
                Sangle = Sangle -1;
// Turn the servo                
                for (int i = 0; i <= 15; i++) {
                  pwm.setPWM(i,0,angleToPulse(Sangle));
                  lcd.setCursor(9, 2);
                  lcd.print("   ");
                }
            } 
          }

// If pressing the + Button
          if (Button3 == 0){
            if (Sangle <= 179){            
               Sangle = Sangle +1;
// Turn the servo                
               for (int i = 0; i <= 15; i++) {
                 pwm.setPWM(i,0,angleToPulse(Sangle));
                 lcd.setCursor(8, 2);
                 lcd.print("   ");
                 }
               }
            }
// Exit to menu button
          if (Button4 == 0){
            Fmanual = false;
          }

    }
    clearscreen();    
}

void clearscreen(){
  lcd.setCursor(0, 0);  // Move the cursor to origin
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
//  lcd.print("1     2     3       ");
  lcd.print("                    ");
}      
