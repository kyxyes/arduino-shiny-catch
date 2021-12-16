// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 5; 
int servoPin2 = 6; 
int servoPin3 = 9; 
int calibration = false;

int servoAngleA = 180;
int servoAngleX = 0;
int servoAngleHome = 180;
int totalEncounters = 1200;

// Create a servo object 
Servo ServoA; 
Servo ServoX; 
Servo ServoHome;
void setup() { 
   // We need to attach the servo to the used pin number 
   ServoA.attach(servoPin); 
   ServoX.attach(servoPin2);
   ServoHome.attach(servoPin3);

   initialButtons();
   
   if (calibration) {
    
      delay(2000);
      Serial.println("Test press button");
    //  pressAButton();
      //pressXButton();
      pressHomeButton();
      
   } 

    delay(10000);  // wait 10 second to start the program so we can stop program if need

   Serial.begin(9600); // open a serial connection to your computer
}

void initialButtons() {
      ServoA.write(servoAngleA);
      ServoX.write(servoAngleX);
      ServoHome.write(servoAngleHome); 
}

void pressAButton() {
    int pressDurationInMs = 300;
    ServoA.write(145); 
    delay(pressDurationInMs);
    ServoA.write(servoAngleA); 
}

void pressXButton() {
    int pressDurationInMs = 300;
    ServoX.write(30);
    delay(pressDurationInMs);
    ServoX.write(0);
}

void pressHomeButton() {
      int pressDurationInMs = 300;
      ServoHome.write(110); 
      delay(pressDurationInMs);
      ServoHome.write(180); 
}
 
void loop(){ 

   if (calibration) {
        return;
    }
  
   Serial.println("Starting loop");

   Serial.println("Select game Pokemon shining pearl");
   pressAButton(); // press game icon and start game 
   delay(2750);

   Serial.println("Select current user");
   pressAButton(); // select current user

   Serial.println("Waiting loooong black screen...");
   delay(24000);    //wait for black screen wait until year company;

   Serial.println("Skip anime screen");
   pressAButton(); 
   delay(4000);

   Serial.println("Confirm `Press the A button screen`");
   pressAButton(); // Press the A button screen


   Serial.println("Waiting entering the game and meet giratina");
   delay(10000);      //wait entering the game;

   //-------At Game, meet giratina

   Serial.println("Talk to giratina and start battle");
   pressAButton(); // Talk to giratina
   delay(2000);
   pressAButton(); // ...Bishaan!
   delay(22000);   // Starting battle.. start anime


   // ---- Catch giratina and picture analysis
   // -----

   Serial.println("Press home button exit");
   pressHomeButton();
   delay(2000);

   Serial.println("Exit game");
   pressXButton(); // Press X button to exit game
   delay(1000);
    pressAButton(); // Press A to confirm close game

   totalEncounters = totalEncounters + 1;

   String outString = String("Total softresets:") + '\t' + String(totalEncounters) + '\n'; 
   Serial.println(outString);
   delay(5000);
   
    
   
}
