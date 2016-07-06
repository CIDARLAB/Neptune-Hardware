//Capabilities the Arduino-side code should have: 
// 1) Initially, GUI should send information to Arduino regarding the number of pumps. With this data 
//    the Arduino can initiate an appropriate number of motors to control the microfluidic. 
// 2) We needa function to MAP pins to motors. 

#include <Servo.h>

String command = "";                // a string to hold incoming command
String sync_signal = "";            // a string to hold syncronization signal 
char singleChar;                    // a single byte from Serial -- buffered into either 'command' and 'sync_signal', depon the STAGE
int number_of_objects_to_initiate;    // We'll pull this variable from the sync_signal, and then we use this variable to initiate our servos and pins

int STAGE = 1;                                // THIS IS THE STATE THE FIRMWARE IS IN. POSSIBLE STATES ARE: 
                                              // 1:Searching for sync signal: Before the user can control the pumps, a signal must be sent 
                                              //                              that indicates the number of pumps to initiate. Once this 
                                              //                              signal is recieved, we move to the next state,
                                              // 2:Searching for commands:    After the servo objects are initiated, we move into the state 
                                              //                              where we search for commands to control the pumps. Here, the 
                                              //                              Arduino is searching for serial data that indicates a command. 
                                              
boolean command_recieved; // We use this variable as a toggle 
boolean sync_recieved;    // We use this variable as a toggle 

String servo_to_actuate_s;  // Represents the pin/pump that will be controlled 
int servo_to_actuate;
String PWM_to_apply_s;    // Represents the PWM value that will be applied to the pin above 
int PWM_to_apply;
Servo *PUMP[16];

void init_pins(int number_of_objects_to_initiate)
{
  for (int i = 0; i < number_of_objects_to_initiate; i++)
  {
    pinMode(2+i,OUTPUT); 
  }
}

void init_servos(int number_of_objects_to_initiate)
{
    for (int i = 0; i < number_of_objects_to_initiate; i++)
    {
        Servo *pump = new Servo();
        PUMP[i] = pump;
    }
}

void attach_servos(int number_of_objects_to_initiate)
{
  for (int i = 0; i < number_of_objects_to_initiate; i++)
  {
    PUMP[i]->attach(i+2);
  }
}

void set_servos_to_zero(int number_of_objects_to_initiate)
{
  for (int i = 0; i < number_of_objects_to_initiate; i++)
  {
    PUMP[i]->write(0);
    delay(200);
  }
}

// ------------------------------------- ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ -----------------------------------------------------------------
//
// -------------------------------------        All helper functions go above here      -----------------------------------------------------------------
// ------------------------------------- SETUP(), LOOP(), SERIALEVENT() go below here   -----------------------------------------------------------------
//
// ------------------------------------- ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ -----------------------------------------------------------------

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  //init_pins();
  //attach_servo();
  //set_servo_to_zero();
  command_recieved = false; 
  sync_recieved = false;
  
}

void loop() 
{
  switch (STAGE)
  {
    case 1:
        if (sync_recieved)
        {
          number_of_objects_to_initiate = sync_signal.toInt();
          init_pins(number_of_objects_to_initiate);
          init_servos(number_of_objects_to_initiate);
          attach_servos(number_of_objects_to_initiate);
          set_servos_to_zero(number_of_objects_to_initiate);
          STAGE = 2;
        }
    break;
    case 2:
        if (command_recieved)
        {
           servo_to_actuate_s = command.substring(1,5);
           servo_to_actuate = servo_to_actuate_s.toInt();
            
           PWM_to_apply_s = command.substring(5);
           PWM_to_apply = PWM_to_apply_s.toInt();
           command = "";

           PUMP[servo_to_actuate]->write(PWM_to_apply);
        }
    break;
  }
}

void serialEvent()
{
  
  while (Serial.available() > 0)
  {
    switch (STAGE)
    {
      
      case 1:
        singleChar = (char)Serial.read();
        if (singleChar == '\n')
          {sync_recieved = true;}
        else 
          {sync_signal += singleChar;}
      break;
      
      case 2:
        singleChar = (char)Serial.read();
        if (singleChar == '\n') 
          {command_recieved = true;}
        else                    
          {command += singleChar;}
      break;
      
    }
  
  }
}

          
    
