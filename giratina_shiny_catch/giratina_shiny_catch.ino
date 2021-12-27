// Include the Servo library 
#include <Servo.h> 

#define BUTTON_A 'a'
#define BUTTON_X 'x'
#define BUTTON_HOME 'h'

// Declare the Servo pin 
int servoPin = 5; 
int servoPin3 = 6; 
int calibration = false;

int servoAngleAX = 90;
int servoAngleA = servoAngleAX;
int servoAngleX = servoAngleAX;
int servoAngleHome = 90;
int totalEncounters = 5;
bool keepLooping = true;

// Create a servo object 
Servo ServoA; 
Servo ServoX; 
Servo ServoHome;
void setup() { 
   // We need to attach the servo to the used pin number 
   ServoA.attach(servoPin); 
   ServoHome.attach(servoPin3);

   initialButtons();

   delay(10000);
   
   Serial.begin(9600); // open a serial connection to your computer
}

void initialButtons() {
      ServoA.write(servoAngleA);
      ServoHome.write(servoAngleHome); 
}

void pressAButton() {
    int pressDurationInMs = 300;
    ServoA.write(60); 
    delay(pressDurationInMs);
    ServoA.write(servoAngleA); 
}

void pressXButton() {
    int pressDurationInMs = 300;
    ServoA.write(130);
    delay(pressDurationInMs);
    ServoA.write(servoAngleA);
}

void pressHomeButton() {
      int pressDurationInMs = 300;
      ServoHome.write(50); 
      delay(pressDurationInMs);
      ServoHome.write(servoAngleHome); 
}
 
void loop(){ 

   if (calibration) {
        int letter = Serial.read();
        switch (letter) {
          case BUTTON_A:
            pressAButton();
            break;
          case BUTTON_X:
            pressXButton();  
            break;
          case BUTTON_HOME:
            pressHomeButton();
            break;  
        }

        return;
    }

   if (!keepLooping) {
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


   Serial.println("Waiting entering the game and meet pokemon");
   delay(12000);      //wait entering the game;

   //-------At Game, meet legendary pokemen

   Serial.println("Talk to legendary pokemen and start battle");
   pressAButton(); // Talk to legendary pokemen
   delay(2000);
   pressAButton(); // ...Blablabal!
   delay(17000);   // Starting battle.. start anime


   // ---- Send commond to do picture analysis

    Serial.println("Flushing leftover data");
    
    // flush the input buffer in case there is any leftover data
    while (Serial.available() != 0) {
      char t = Serial.read();
    }
    
    // Send message to Python
    Serial.println("Command: checkIfShiny");
    
    // Wait until we receive a message from Python
    while (Serial.available() == 0) {
      delay(200);
    }


   Serial.println("Reading response from Python");
   // Read response from Python
    char inByte = Serial.read();


     // Check if shiny
    if (inByte == 'y') {
      // it's shiny! Stop looping
      
      Serial.println("It's shiny!");
      keepLooping = false;
    } else {
      // not shiny. Do a soft reset and keep on looping!
      
       Serial.println("Not shiny. Doing soft reset");

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

   
}
