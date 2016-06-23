//Capabilities the Arduino-side code should have: 
// 1) Initially, GUI should send information to Arduino regarding the number of pumps. With this data 
//    the Arduino can initiate an appropriate number of motors to control the microfluidic. 
// 2) We needa function to MAP pins to motors. 

#include <Servo.h>

String command = "";         // a string to hold incoming command
char singleChar;

Servo myPump1;
Servo myPump2;
Servo myPump3;
Servo myPump4;

String pin_to_actuate_s;
String PWM_to_apply_s;
int pin_to_actuate;
int PWM_to_apply;

int angle;   // variable to hold the angle for the servo motor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_pins();
  attach_servo();
  
}

void init_pins()
{
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
}

void attach_servo()
{
  myPump1.attach(9);  // Servo 1
  myPump2.attach(10); // Servo 2
  myPump3.attach(11); // Servo 3
  myPump4.attach(12); // Servo 4
}

void loop() {
  // put your main code here, to run repeatedly:
  while ( Serial.available() > 0 ) {
        singleChar = (char)Serial.read();
        if (singleChar == '\n') 
        {
           pin_to_actuate_s = command.substring(1,5);
           pin_to_actuate = pin_to_actuate_s.toInt();
           
           PWM_to_apply_s = command.substring(5);
           PWM_to_apply = PWM_to_apply_s.toInt();
           
           command = "";

   
           switch (pin_to_actuate)
           {
            case 1: myPump1.write(PWM_to_apply); break;
            case 2: myPump2.write(PWM_to_apply); break;
            case 3: myPump3.write(PWM_to_apply); break;
            case 4: myPump4.write(PWM_to_apply); break;
           }
           delay(200);
        }
        else {command += singleChar;}
  }
}
