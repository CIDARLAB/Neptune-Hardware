#include <Servo.h> 
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
boolean error_flag = false;      // wheather input string is valid

int pos = 0;    // variable to store the servo position 
Servo myservo1;  // create servo object to control a servo 
Servo myservo2;
Servo myservo3;
Servo myservo4;
                

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(10);
  myservo3.attach(11);
}

void loop() {
 //#1 thing to do when installing servos: initalize this Arduino script
    // the initialization is as simple as the software defining what on and off
    // mean in terms of PWM values. This will automatically download to the Arduino
    // whenever the user changes these parameters.
  //#2 is main loop! wait for inputs of format '001N' or '001F' being on or off respectivly
  
  // String should arrive in following format:
  // 's00014096e'
  // 's' Signals the start of the string
  // '0001' Specifies the specific servo to target, from 0000 to 9999 (practial limits under 1000)
  // '4096' Specifies the PWM value to send to the servo, a specific on/off code determined by software up the ladder
  // '\n' Signals the end of the string


    // when newline arrives, induce PWM signal to match it
  if (stringComplete) {
/*    
      // Check to ensure valid
      error_flag = false;
        // Ensure first character is starting character 's'
        if (inputString[0] != 's') {
          error_flag = true;
          Serial.println("start isn't an 's'");
        }
        
      // ensure next four inputed values are numbers, between 0000 and 1000      
      for(byte i=1;i<5;i++)
      {
        if (!isdigit(inputString[i])) {
          error_flag = true;
          Serial.println("not numbers");
        }
      }
      if (!error_flag) {
        if (inputString.substring(1,4).toInt() > 1000) {
          error_flag = true;
          Serial.println("Not that many pumps numnuts");
        }
      }
      // ensure next four inputed values are numbers, no greater than 4096
      for(byte i=5;i<9;i++)
      {
        if (!isdigit(inputString[i])) {
          error_flag = true;
          Serial.println("They arn't number dork");
        }
      }
      
      if (!error_flag) {
        if (inputString.substring(5,9).toInt() > 4096) {
          error_flag = true;
          Serial.println("greater PWM then neccecary");
        }
      }
      
      // ensure last entered is a e
      if (inputString[9] != 'e') {
        error_flag = true;
        Serial.println("End line not detected");
      }
*/
    // if error flag is raised, stop and try again
    if (error_flag) {
        Serial.println("Invalid entry.");
        stringComplete  =false;
    }

    //If no errors, proceed to process data
    else {
      //Determine pump number
      Serial.print("Pump ");
      Serial.print(inputString.substring(1,5));
      Serial.print(" is turning to PWM value: ");
      Serial.print(inputString.substring(6,9));



      if(inputString.substring(1,5) == "0001") {
        Serial.print(" got to servo1");
        myservo1.write(inputString.substring(6,9).toInt());              // tell servo to go to position in variable 'pos' 
        delay(15);
      }
      else if(inputString.substring(1,5) == "0002") {
        myservo2.write(inputString.substring(6,9).toInt());              // tell servo to go to position in variable 'pos'
        Serial.print(" got to servo2"); 
        delay(15);
      }
      else if(inputString.substring(1,5) == "0003") {
        myservo3.write(inputString.substring(6,9).toInt());              // tell servo to go to position in variable 'pos' 
        Serial.print(" got to servo3"); 
        delay(15);
      }


      
      
//      myservo.write(inputString.substring(6,9).toInt());              // tell servo to go to position in variable 'pos' 
//      delay(15);
        
      Serial.println("");
      // clear the string:
      inputString = "";
      stringComplete = false;
    }   
  }
}
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
