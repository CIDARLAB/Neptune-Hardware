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
    // when newline arrives, induce PWM signal to match it

  if (stringComplete) {
      //Determine pump number
      Serial.print("Pump: ");
      Serial.print(inputString[0]);
      Serial.print("  PWM: ");
      Serial.print(inputString.substring(2,5));

      if(inputString[0] == '1') {
        Serial.print(" got to servo1");
        myservo1.write(inputString.substring(2,5).toInt());              // tell servo to go to position in variable 'pos' 
        delay(15);
      }
      else if(inputString[0] == '2') {
        myservo2.write(inputString.substring(2,5).toInt());              // tell servo to go to position in variable 'pos'
        Serial.print(" got to servo2"); 
        delay(15);
      }
      else if(inputString[0] == '3') {
        myservo3.write(inputString.substring(2,5).toInt());              // tell servo to go to position in variable 'pos' 
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
