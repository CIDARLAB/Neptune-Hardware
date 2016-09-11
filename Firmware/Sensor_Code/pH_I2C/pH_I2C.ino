#include <Wire.h>                //enable I2C.
/*     Addressing    */
const int MAX_NUMBER_OF_SENSORS = 101;
const int TYPE_IDENTIFIER_LENGTH = 2;
const int RESPONSE_LENGTH = 8;
char addressMatrix[MAX_NUMBER_OF_SENSORS][TYPE_IDENTIFIER_LENGTH];   //holds the type of sensor at index/address with 2 chars
char responseMatrix[MAX_NUMBER_OF_SENSORS][RESPONSE_LENGTH]; //holds the most recent data from the specic sensor from the last read that was made
/*      -------      */
/*   Timing Delays   */
int LONG_DELAY = 1800;
int SHORT_DELAY = 300;
int time_ = LONG_DELAY;                //used to change the delay needed depending on the command sent to the EZO Class pH Circuit.
/*      -------      */
/*  I2C Status Codes & Data */
byte code = 0;                   //used to hold the I2C response code.
char ph_data[20];                //we make a 20 byte character array to hold incoming data from the pH circuit.
byte in_char = 0;                //used as a 1 byte buffer to store in bound bytes from the pH Circuit.
byte sensor_data_counter = 0;                      //counter used for ph_data array.
/*      -------      */
/*   Serial Com PC   */
byte received_from_computer = 0; //we need to know how many characters have been received.
String command;
bool commandTransmitted = false;
/*     -------     */
/*   STATE CODES   */
int systemState = 0;
const int STANDBY = 0; //open to receiving commands
const int SINGLE_READ = 1; //Single Read
const int CONTINUOUS_READ = 2; //Continuous Read
const int COMPLETED = 3; //completed a read or a calibration and will send packets of data back to PC at some point
/*     -------     */
/*  Sensor Reading */
int continuousReadTimeInterval = 1; // in Minutes
bool shouldExecuteNextRead = false;
/*     -------     */
/*  Time Stamping */
unsigned int minutes = 0;
unsigned int seconds = 0;
int lastReadTime = 0; //in minutes
bool firstSample = true;
/* -------------  */
/*  ATLAS Specific Commands  */
char readCommand[] = {'r'};
/*   --------- * /
//this is just to initialize the array for what I have.
/* [ ['**'], ["PH"], ["DO"]]  -> this type of format where the index is the I2C address
*/
void initializeSensorAddressArray() {
  int i;
  for (i = 0 ; i < MAX_NUMBER_OF_SENSORS; i++ ) {
    int j;
    for (j = 0; j < 2 ; j++ ) {
      addressMatrix[i][j] = '*';
    }
  }
}

/////////////////////     Timer2 Function      /////////////////////
int count = 0;
int statusCount = 0;
//Timer2 Overflow Interrupt Vector, called every 1ms
ISR(TIMER2_OVF_vect) {
  count++;               //Increments the interrupt counter
  statusCount++;
  if (count > 999) { //will trigger on the 1000th time this interrupt vector will be called right now (1 second time, replace this delay with whatever you need to do).
    seconds += 1;
    if (seconds > 59) {
      minutes += 1;
      seconds = 0;
    }
    count = 0;           //Resets the interrupt counter
  }
  if (statusCount > 4999) {
    switch (systemState)
    {
      case STANDBY:
        //Serial.println("SB");
        break;
      case SINGLE_READ:
        //Serial.println("RS");
        break;
      case CONTINUOUS_READ:
        //Serial.println("RC");
        break;
      case COMPLETED:
        //Serial.println("C");
        break;
      default:
        //Serial.println("ER");
        break;
    }
    statusCount = 0;
  }
  TCNT2 = 130;           //Reset Timer to 130 out of 255 //TODO: CHECK THIS MATH, I MIGHT NEED TO CHANGE THE CLOCK SPEED FOR THIS TO TRIGGER 1MS.
  TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
};

////////////////////////////////////////////////////////////////////

void setup()                    //hardware initialization.
{
  Serial.begin(9600);           //enable serial port.
  Wire.begin();                 //enable I2C port.
  command.reserve(20);
  initializeSensorAddressArray();
  char code12[]  = {'P', 'H'};
  char code50[]  = {'P', 'H'};
  char code100[] = {'D', 'O'};
  char code20[] = {'P','H'};
  setSensorAtAddressIndex(12, code12);
  setSensorAtAddressIndex(50, code50);
  setSensorAtAddressIndex(100, code100);
  setSensorAtAddressIndex(20, code20);

  //Setup Timer2 to fire every 1ms
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 130;         //Reset Timer Count to 130 out of 255
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Normal port operation, Wave Gen Mode normal
  TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128
}

//NEED TO LOOK INTO INTERRUPT FOR SERIAL RECEIVE. in there quit a process if we want it to.
void loop() {                   //the main loop.
  switch (systemState) {
    case STANDBY:
      //this sets timing parameters for the sensors and stuff like that
      if (commandTransmitted) {
        processComputerData(command, received_from_computer);
        // clear the string:
        command = "";
        commandTransmitted = false;
        received_from_computer = 0;
      }
      break;
    case SINGLE_READ:
      //NEED TO USE THE RESPONSE CODES TO EITHER TRY AGAIN, GIVE UP, OR CONTINUE
      executeCommandOnAllSensors(readCommand, 1);
      readDataFromAllSensors();
      shouldExecuteNextRead = false;
      systemState = COMPLETED;
      break;
    case CONTINUOUS_READ:
      if ( ((minutes - lastReadTime) >= continuousReadTimeInterval) || firstSample){
        firstSample = false;
        lastReadTime = minutes;
        executeCommandOnAllSensors(readCommand, 1);
        readDataFromAllSensors();
        systemState = COMPLETED;
      } 
      break;
    case COMPLETED:
      sendDataPacketsToComputer();
      if (shouldExecuteNextRead) {
        systemState = CONTINUOUS_READ;
      } else {
        systemState = STANDBY;
        firstSample = true;
      }
      break;
    default:
      //unknown state UHHH
      break;
  }
}
void executeCommandOnAllSensors(char command[] , int len) { //I wish this was swift for function declarations at least...
  //need to figure out a way to quit
  //implement the
  if (command[0] == 'c' || command[0] == 'r')time_ = LONG_DELAY;    //if a command has been sent to calibrate or take a reading we wait a longer delay so that the circuit has time to take the reading.
  else time_ = SHORT_DELAY;    //if any other command has been sent we wait a shorter delay
  int address;
  for (address = 0; address < MAX_NUMBER_OF_SENSORS; address++) {
    //has an address been configured? if so CHOOSE IT
    if (addressMatrix[address][0] != '*' && addressMatrix[address][1] != '*') {
      //Serial.print("Address: "); Serial.print(address); Serial.print(" Command: ");
      int i;
      for (i = 0; i < len ; i++) {
        if (command[i] == '\r') break;
        else {
          //Serial.print(command[i]);
        }
      }
      //Serial.print(" Type: "); Serial.print(addressMatrix[address][0]); Serial.println(addressMatrix[address][1]);
      Wire.beginTransmission(address); //call the circuit by its ID number.
      Wire.write(command, len);        //transmit the command that was sent through the serial port.
      Wire.endTransmission();          //end the I2C data transmission.
    }
  }
  delay(time_);
  //Somewhere else I'll send the data and return to standby or processing
}

bool readDataFromAllSensors() {
  int address;
  for (address = 0 ; address < MAX_NUMBER_OF_SENSORS ; address++) {
    if (addressMatrix[address][0] != '*' && addressMatrix[address][1] != '*') {
      Wire.requestFrom(address, 20, 1); //call the circuit and request 20 bytes (this may be more than we need)
      //Serial.print("A: ");
      //Serial.println(address);
      code = Wire.read();             //the first byte is the response code, we read this separately.
      switch (code) {                 //switch case based on what the response code is.
        case 1:                       //decimal 1.
          //Serial.println("Success");  //means the command was successfully received
          break;
        case 2:                        //decimal 2.
          //Serial.println("Failed");    //means the command has failed.
          break;                         //exits the switch case.
        case 254:                       //decimal 254.
          //Serial.println("Pending");    //means the command has not yet been finished calculating.
          break;
        case 255:                      //decimal 255.
          //Serial.println("No Data");   //means there is no further data to send.
          break;
      }
      bool shouldLoop = true;
      while (Wire.available() && shouldLoop) {                  //are there bytes to receive.
        in_char = Wire.read();                                  //receive a byte.
        responseMatrix[address][sensor_data_counter] = in_char; //load this byte into the index for the response array.
        sensor_data_counter += 1;                               //increase the counter for the array element.
        if (in_char == 0) {                                     //if we see that we have been sent a null char to signify the end of the transmission.
          int i; 
          for (i = sensor_data_counter; i < RESPONSE_LENGTH; i++){
            responseMatrix[address][sensor_data_counter] = '\0';
          } 
          sensor_data_counter = 0;                              //reset the counter i to 0.
          Wire.endTransmission();                               //end the I2C data transmission.
          shouldLoop = false;
        }
      }
      //Serial.println(responseMatrix[address]);          //print the data
      delay(time_);
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
    command += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    received_from_computer += 1;
    if (inChar == '\r') {
      commandTransmitted = true;
    }
    if (inChar == 'q') {
      shouldExecuteNextRead = false;
    }
  }
}

void sendDataPacketsToComputer() {
  //implement
  //Serial.print("Data Sent at Time: ");
  //Serial.print( minutes ); Serial.print(":"); Serial.println(seconds);
  //need to create the data packet to send each packet 
  int address;
  for (address = 0; address < MAX_NUMBER_OF_SENSORS; address++){
     if (addressMatrix[address][0] != '*' && addressMatrix[address][1] != '*') {
       int currentMinutes = minutes;
       int currentSeconds = seconds;
       //valid sensor do stuff
       //create my packet of data
       //Address and Type
       Serial.print(address); Serial.print('S'); Serial.print(addressMatrix[address][0]); Serial.print(addressMatrix[address][1]); 
       //Value
       int i;
       for (i = 0; i < RESPONSE_LENGTH; i++){
         Serial.print(responseMatrix[address][i]);
       }
       Serial.print('T');
       //Time Stamp Minutes
       if (currentMinutes < 10) {
         Serial.print('0'); Serial.print(currentMinutes);
       } else {
         Serial.print(currentMinutes);
       }
       //Time Stamp Seconds
       if (currentSeconds < 10) {
         Serial.print('0'); Serial.print(currentSeconds);
       } else {
         Serial.print(currentSeconds);
       }
       Serial.print('\r');
     } 
  }
  
   //don't care about the fact that the address has not been set.
   
}

//each address corresponds to the index of the array (except address 0, that IS NOT ALLOWED)
void setSensorAtAddressIndex(int index, char code[]) {
  char currentCode[] = { '*', '*' };
  currentCode[0] = addressMatrix[index][0];
  currentCode[1] = addressMatrix[index][1];
  if (currentCode[0] == '*' && currentCode[1] == '*') {
    //available
    addressMatrix[index][0] = code[0];
    addressMatrix[index][1] = code[1];
  } else {
    //index is currently being used, cannot use it. //Output an error? handle different?
  }
}

void removeSensorAtAddressIndex(int index) {
  addressMatrix[index][0] = '*';
  addressMatrix[index][1] = '*';
}

void processComputerData(String rawString, int len) {
  //I'll get commands in the form of READ<CR> or CAL<CR> or <READC,{TIME}<CR> .. they all end in <CR> so that's what I need to figure out.
  String commandParsed = "";
  String parameters[] = {""};
  int i;
  for ( i = 0; i < len ; i++) {
    if (rawString[i] == ',' || rawString[i] == '\r') {
      break;
    }
    commandParsed += rawString[i];
  }

  i++;

  for ( ; i < len ; i++) {
    if (rawString[i] == ',') break;
    parameters[0] += rawString [i];
  }

  if ( commandParsed == "read" ) {
    systemState = SINGLE_READ;
    resetSystemClock();
  } else if (commandParsed == "readc") {
    continuousReadTimeInterval = parameters[0].toInt();
    shouldExecuteNextRead = true;
    systemState = CONTINUOUS_READ;
    resetSystemClock();
    lastReadTime = minutes;
  }
  else if ( commandParsed == "cal") {
    //Serial.println("Cal Understood");
  }
}

void resetSystemClock(){
  minutes = 0;
  seconds = 0;
  count = 0;
}
