//We want a command that will be in the format of 'P#M#/n' /n is the escape character , the first # is at most 2 bytes (unless we get over 100 pumps) and then
//the second # is at most 3 bytes (0-100%) Pusle Width modulation

//EX: P0M50/n is PUMP 0 at 50% PWM 
                 //0 1 2 3 4 5 -> PUMP ASSIGNMENTS 
int pumpPins[6] = {2,3,4,5,6,7}; 
const int NUMBER_OF_PUMPS = 6; 
String  command = "";             // a string to hold incoming data
boolean stringComplete = false;   // whether the string is complete 
int     pumps[NUMBER_OF_PUMPS + 1 ] ;  // this will hold the PWM values of each pump currently in use index is pump number and value at index is PWM value
// 0 PWM is OFF .... The extra one is for a dummy pump where variables that are not initalized well will go 
//this way we don't mess up the whole program in the case of an error or out of sync commands.
void setup() {
    // put your setup code here, to run once:
    for (int i = 0; i < NUMBER_OF_PUMPS; i++){
      pumps[i] = 0;
      pinMode(pumpPins[i], OUTPUT);
    }
    
    
    //command.reserve(8);
    Serial.begin(9600); // baud rate
    Serial.flush();
}

void loop() {
  // put your main code here, to run repeatedly:
  // print the string when a newline arrives:
  if (stringComplete) {
    //do stuff with the command here
    parseCommand(command);
    delay(10);
    // clear the string:
    stringComplete = false;
    command = "";
    
    for (int i = 0; i < NUMBER_OF_PUMPS; i++){
      Serial.print("Pump "); Serial.print(i);
      Serial.print(" is: "); Serial.print(pumps[i]); Serial.print(" ");
      delay(2);
      analogWrite(pumpPins[i], pumps[i]);
      
    }
    Serial.println("");
    
  }
  
  
  
}

void parseCommand(String command) {
    int pumpNumb = 6; //initialized to the biggest part of the array (the dummy pump)
    int pumpPWM  = 0; //in the case that is connected to anything we don't break it. 
    int i,j,k = -1; 
    Serial.print("COMMAND: "+ command);
    for (int charval = 0; charval < sizeof(command); charval++){
        if (command[charval] == 'P') {
            i = charval;
        }
        if (command[charval] == 'M') {
            j = charval;
        }
    }
    
    pumpNumb = command.substring(i+1,j).toInt();
    //Serial.println("PumpNumber is " + command.substring(i+1,j));
    
    pumpPWM  = command.substring(j+1).toInt();
    //Serial.println("PumpPWM is " + command.substring(j+1));
    
    pumps[pumpNumb] = pumpPWM;
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
    command += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
