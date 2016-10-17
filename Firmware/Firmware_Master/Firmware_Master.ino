/*          
 *              -Zach Lasiuk
*/

      
/////////////////////     Import Libraries as needed      /////////////////////
#include <avr/interrupt.h>              // interupts
#include <avr/io.h>                     // interupts
#include <Wire.h>                       // motor control shield, needed for I2C
#include <Adafruit_PWMServoDriver.h>    // import motor control shield library




//////////////////////////     System Variables      //////////////////////////

/* Variables to init servos
 */
int j; // iterates through loops in trigger
int i; // iterates through loops in main
int f_or_s =1;  // Variable to determine which servo shield to send info to, 1 or 2
int num                  = 200;
int currentPWMs[200]       = {470,1000,1000,1000,1000,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470,470}; // 470 is all the way dispensed (for all servos, mini servo has issues round 470 it is only rated for 460)
int counters[200]          = {0};
int max_count[200]         = {0};
bool dispensers[200]       = {false};
int dispense_counter[200]   = {0};
int dispense_limit[200]    = {0};

// LED variables 
unsigned int LEDtoggle = 0;  //used to keep the state of the LED
unsigned int LEDcount = 0;   //used to keep count of how many interrupts were fired


/* Values to store GUI inputs 
*/
int IN_pump;            // stores pump number from GUI, from 0 to 9999
int IN_PWM;             // stores PWM from GUI
float IN_Flow;            // stores Flow rate from GUI

                                    
/* Serial input variables
 * 
 */
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

/* Servo initalization
 */
Adafruit_PWMServoDriver pwm0 =   Adafruit_PWMServoDriver(0x40);       //  0  - 15
Adafruit_PWMServoDriver pwm1 =   Adafruit_PWMServoDriver(0x41);       //  16 - 31
Adafruit_PWMServoDriver pwm2 =   Adafruit_PWMServoDriver(0x42);       //  32 - 47
Adafruit_PWMServoDriver pwm3 =   Adafruit_PWMServoDriver(0x43);       //  48 - 63
Adafruit_PWMServoDriver pwm4 =   Adafruit_PWMServoDriver(0x44);       //  64 - 79
Adafruit_PWMServoDriver pwm5 =   Adafruit_PWMServoDriver(0x45);       //  80 - 95
Adafruit_PWMServoDriver pwm6 =   Adafruit_PWMServoDriver(0x46);       //  96 - 111
Adafruit_PWMServoDriver pwm7 =   Adafruit_PWMServoDriver(0x47);       //  112 - 127
Adafruit_PWMServoDriver pwm8 =   Adafruit_PWMServoDriver(0x48);       //  128 - 143
Adafruit_PWMServoDriver pwm9 =   Adafruit_PWMServoDriver(0x49);       //  144 - 159
Adafruit_PWMServoDriver pwm10 =   Adafruit_PWMServoDriver(0x50);       //  160 - 175
Adafruit_PWMServoDriver pwm11 =   Adafruit_PWMServoDriver(0x51);       //  176 - 191
Adafruit_PWMServoDriver pwm12 =   Adafruit_PWMServoDriver(0x52);       //  192 - 199


void setup() {
  // initialize serial communication
  Serial.begin(9600);
  inputString.reserve(200);   // reserve 200 bytes for the inputString

  pwm0.begin();
  pwm0.setPWMFreq(50);    // ANALOG
  pwm1.begin();
  pwm1.setPWMFreq(300);   // DIGITAL
  pwm2.begin();
  pwm2.setPWMFreq(50);    // ANALOG
  pwm3.begin();
  pwm3.setPWMFreq(300);   // DIGITAL
  pwm4.begin();
  pwm4.setPWMFreq(50);    // ANALOG
  pwm5.begin();
  pwm5.setPWMFreq(300);   // DIGITAL
  pwm6.begin();
  pwm6.setPWMFreq(50);    // ANALOG
  pwm7.begin();
  pwm7.setPWMFreq(300);   // DIGITAL
  pwm8.begin();
  pwm8.setPWMFreq(50);    // ANALOG
  pwm9.begin();
  pwm9.setPWMFreq(300);   // DIGITAL
  pwm10.begin();
  pwm10.setPWMFreq(50);   // ANALOG
  pwm11.begin();
  pwm11.setPWMFreq(300);  // DIGITAL
  pwm12.begin();
  pwm12.setPWMFreq(50);   // ANALOG
  yield();
  
  //Setup Timer2 to fire every 1ms
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 130;         //Reset Timer Count to 130 out of 255 
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Normal port operation, Wave Gen Mode normal
  TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128
  
  // Print initalize statement
  Serial.println("Hello! Welcome to ServoServer. Ready to accept values");
}

/////////////////////     Looped Function      /////////////////////

void loop() { 
    if (stringComplete) {
      
        // Get inputs
        IN_pump = inputString.substring(0,4).toInt();
        IN_PWM = inputString.substring(4,8).toInt();
        IN_Flow = inputString.substring(8,12).toFloat();
        Serial.print(inputString);
        
        // Determine address of sheild to go to
        //if (IN_pump < 16)       {f_or_s = 1;}
        //else if (IN_pump >=16)  {f_or_s = 2;}
        switch((int) IN_pump/16) {
          case 0: {f_or_s = 0; break;}
          case 1: {f_or_s = 1; break;}
          case 2: {f_or_s = 2; break;}
          case 3: {f_or_s = 3; break;}
          case 4: {f_or_s = 4; break;}
          case 5: {f_or_s = 5; break;}
          case 6: {f_or_s = 6; break;}
          case 7: {f_or_s = 7; break;}
          case 8: {f_or_s = 8; break;}
          case 9: {f_or_s = 9; break;}
          case 10: {f_or_s = 10; break;}
          case 11: {f_or_s = 11; break;}
          case 12: {f_or_s = 12; break;}
          default: {f_or_s = 50; break;}
        }

        
        // Valve control (no set flow rate)
        if (IN_Flow == 0) {     
          currentPWMs[IN_pump] = IN_PWM;
          //if (f_or_s == 0)        {pwm0.setPWM(IN_pump%16,0,currentPWMs[IN_pump]);}
          //else if (f_or_s == 1)   {pwm1.setPWM(IN_pump%16,0,currentPWMs[IN_pump]);}
          switch(f_or_s) {
            case 0: {pwm0.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 1: {pwm1.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 2: {pwm2.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 3: {pwm3.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 4: {pwm4.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 5: {pwm5.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 6: {pwm6.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 7: {pwm7.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 8: {pwm8.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 9: {pwm9.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 10: {pwm10.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 11: {pwm11.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            case 12: {pwm12.setPWM(IN_pump%16,0,currentPWMs[IN_pump]); break;}
            default: {break;}
          }
        }

        // Despenser control initiation
        else {
          dispense_limit[IN_pump] = IN_PWM-currentPWMs[IN_pump];            //set dispense_limit (number of steps to increment PWM by one)
          max_count[IN_pump] = IN_Flow*1000/abs(dispense_limit[IN_pump]);   //Calculate max_count (number of ms between each PWM step)    
        }

        // clear the string:
        inputString = "";
        stringComplete = false;
        
        /* DEBUGGING ONLY
        for (i=0; i < num; i++) {
          Serial.print(i);
          Serial.print(" curentPWMs: ");
          Serial.println(currentPWMs[i]);
        }
        */ 
  } 

    // Take care of dispensors here, fires each loop
    // Checks each dispensor for True and set back to false
    for (i=0; i < num; i++) {
      // if dispensers[i] is true, time to change PWM by one
      if (dispensers[i]) {
        // if our counter exceeds our limit, stop this process!
        if (abs(dispense_counter[i]) >= abs(dispense_limit[i])) {
          //turn off this dispenser, stop everything
          max_count[i] = 0; //turning off max_count will stop the interupt from touching this
          dispense_counter[i] = 0;
          dispense_limit[i] = 0;
        }
        // if counter bellow limit, change PWM by one
        else {
          if (dispense_limit[i] < 0) {
             currentPWMs[i]--;                 // set current PWM to new value
             dispense_counter[i]--;            // decrese dispense_counter  
             switch((int) i/16) {
              case 0: {pwm0.setPWM(i%16,0,currentPWMs[i]); break;}
              case 1: {pwm1.setPWM(i%16,0,currentPWMs[i]); break;}
              case 2: {pwm2.setPWM(i%16,0,currentPWMs[i]); break;}
              case 3: {pwm3.setPWM(i%16,0,currentPWMs[i]); break;}
              case 4: {pwm4.setPWM(i%16,0,currentPWMs[i]); break;}
              case 5: {pwm5.setPWM(i%16,0,currentPWMs[i]); break;}
              case 6: {pwm6.setPWM(i%16,0,currentPWMs[i]); break;}
              case 7: {pwm7.setPWM(i%16,0,currentPWMs[i]); break;}
              case 8: {pwm8.setPWM(i%16,0,currentPWMs[i]); break;}
              case 9: {pwm9.setPWM(i%16,0,currentPWMs[i]); break;}
              case 10: {pwm10.setPWM(i%16,0,currentPWMs[i]); break;}
              case 11: {pwm11.setPWM(i%16,0,currentPWMs[i]); break;}
              case 12: {pwm12.setPWM(i%16,0,currentPWMs[i]); break;}
              default: {break;}
            }
          }
          else {
            currentPWMs[i]++;                 // set current PWM to new value
            dispense_counter[i]++;            // increment dispense_counter   
             switch((int) i/16) {
              case 0: {pwm0.setPWM(i%16,0,currentPWMs[i]); break;}
              case 1: {pwm1.setPWM(i%16,0,currentPWMs[i]); break;}
              case 2: {pwm2.setPWM(i%16,0,currentPWMs[i]); break;}
              case 3: {pwm3.setPWM(i%16,0,currentPWMs[i]); break;}
              case 4: {pwm4.setPWM(i%16,0,currentPWMs[i]); break;}
              case 5: {pwm5.setPWM(i%16,0,currentPWMs[i]); break;}
              case 6: {pwm6.setPWM(i%16,0,currentPWMs[i]); break;}
              case 7: {pwm7.setPWM(i%16,0,currentPWMs[i]); break;}
              case 8: {pwm8.setPWM(i%16,0,currentPWMs[i]); break;}
              case 9: {pwm9.setPWM(i%16,0,currentPWMs[i]); break;}
              case 10: {pwm10.setPWM(i%16,0,currentPWMs[i]); break;}
              case 11: {pwm11.setPWM(i%16,0,currentPWMs[i]); break;}
              case 12: {pwm12.setPWM(i%16,0,currentPWMs[i]); break;}
              default: {break;}
            }
          }  
          // Serial.print("fired "); Serial.print(i); Serial.println(currentPWMs[i]);  // DEBUGGING ONLY             
        }
        counters[i] = 0;                  // counter is reset
        dispensers[i] = false;            // reset dispenser boolean
      }
  }
}

/////////////////////     Timer2 Function      /////////////////////

//Timer2 Overflow Interrupt Vector, called every 1ms
ISR(TIMER2_OVF_vect) {
  LEDcount++;               //Increments the interrupt counter
  if(LEDcount > 999){
    LEDtoggle = !LEDtoggle;    //toggles the LED state
    LEDcount = 0;           //Resets the interrupt counter
  }
  digitalWrite(53,LEDtoggle);


  // Quick loop to set dispense variables to true or false
  for (j=0; j < num; j++) {
    if (max_count[j] == 0)             //if max_count is not set (equal to 0) check next trigger
      continue;
      
    counters[j]++;                    //increment counter
    
    if (counters[j] >= max_count[j])   //checks if count is equal to the max_count value
      dispensers[j] = true;           // dispense variable is set to true
      // reset counter and increment dispense_counter OUTSIDE of this loop to save time   
  }
  
   
  TCNT2 = 130;           //Reset Timer to 130 out of 255
  TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
};  




/////////////////////     Read Serial Function      /////////////////////
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
