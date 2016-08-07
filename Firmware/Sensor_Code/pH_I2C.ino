//**THIS CODE WILL WORK ON ANY ARDUINO**
//This code has intentionally has been written to be overly lengthy and includes unnecessary steps.
//Many parts of this code can be truncated. This code was written to be easy to understand.
//Code efficiency was not considered. Modify this code as you see fit.
//This code will output data to the Arduino serial monitor. Type commands into the Arduino serial monitor to control the EZO pH Circuit in I2C mode.
//this code was last updated 7-25-2016


#include <Wire.h>                //enable I2C.


int addresses[3] = {12, 50, 100}; //I2C addresses
int numberOfSensors = 3;
int LONG_DELAY = 1800;
int SHORT_DELAY = 100;
char computerdata[20];           //we make a 20 byte character array to hold incoming data from a pc/mac/other.
byte received_from_computer = 0; //we need to know how many characters have been received.
byte code = 0;                   //used to hold the I2C response code.
char ph_data[20];                //we make a 20 byte character array to hold incoming data from the pH circuit.
byte in_char = 0;                //used as a 1 byte buffer to store in bound bytes from the pH Circuit.
byte i = 0;                      //counter used for ph_data array.
int time_ = LONG_DELAY;                //used to change the delay needed depending on the command sent to the EZO Class pH Circuit.
float ph_float;                  //float var used to hold the float value of the pH.
bool isDirectCommand = false;     //Flag that lets one choose to command 1 address specifically
int  directCommandAddress = 0;    //this not changing will cause an error in the stamp (blink red and thus DEBUGGING)
char command[20];



void setup()                    //hardware initialization.
{
  Serial.begin(9600);           //enable serial port.
  Wire.begin();                 //enable I2C port.
}



void loop() {                   //the main loop.

  if (Serial.available() > 0) {                                           //if data is holding in the serial buffer
    received_from_computer = Serial.readBytesUntil(13, computerdata, 20); //we read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received.
    computerdata[received_from_computer] = 0;                             //stop the buffer from transmitting leftovers or garbage.
    computerdata[0] = tolower(computerdata[0]);                           //we make sure the first char in the string is lower case.
    if (computerdata[0] == 'c' || computerdata[0] == 'r')time_ = LONG_DELAY;    //if a command has been sent to calibrate or take a reading we wait 1800ms so that the circuit has time to take the reading.
    else time_ = SHORT_DELAY;                                                     //if any other command has been sent we wait only 300ms.

    if (computerdata[0] == 'd' && computerdata[1] == 'c') {
      isDirectCommand = true;
    } else {
      isDirectCommand = false;
    }



    if (!isDirectCommand) {
      int address;
      for (address = 0; address < numberOfSensors; address++) {
        Wire.beginTransmission(addresses[address]); //call the circuit by its ID number.
        Wire.write(computerdata);        //transmit the command that was sent through the serial port.
        Wire.endTransmission();          //end the I2C data transmission.
      }
      
      delay(time_);

      int j;
      for (j = 0 ; j < numberOfSensors ; j++) {
        Wire.requestFrom(addresses[j], 20, 1); //call the circuit and request 20 bytes (this may be more than we need)
        Serial.print("A: ");
        Serial.println(addresses[j]);
        code = Wire.read();             //the first byte is the response code, we read this separately.

        switch (code) {                 //switch case based on what the response code is.
          case 1:                       //decimal 1.
            Serial.println("Success");  //means the command was successfully received
            break;
          case 2:                        //decimal 2.
            Serial.println("Failed");    //means the command has failed.
            break;                         //exits the switch case.
          case 254:                       //decimal 254.
            Serial.println("Pending");    //means the command has not yet been finished calculating.
            break;
          case 255:                      //decimal 255.
            Serial.println("No Data");   //means there is no further data to send.
            break;
        }
        bool shouldLoop = true;
        while (Wire.available() && shouldLoop) {         //are there bytes to receive.
          in_char = Wire.read();           //receive a byte.
          ph_data[i] = in_char;            //load this byte into our array.
          i += 1;                          //incur the counter for the array element.
          if (in_char == 0) {              //if we see that we have been sent a null command.
            i = 0;                         //reset the counter i to 0.
            Wire.endTransmission();        //end the I2C data transmission.
            shouldLoop = false;
          }
        }
        Serial.println(ph_data);          //print the data
        delay(time_);
      }

    } else { // it is a direct command, send to the little baby you love the most
      int i;
      int radix = 100;
      directCommandAddress = 0;
      for (i = 3; i < 6; i++) {
        directCommandAddress += (computerdata[i] - '0') * radix; //gets the value out of a char array
        radix = radix / 10;
      }

      for (i = 7; i < received_from_computer; i++ ) {
        command[i - 7] = computerdata[i];
      }
      if (command[0] == 'c' || command[0] == 'r')time_ = LONG_DELAY;    //if a command has been sent to calibrate or take a reading we wait 1800ms so that the circuit has time to take the reading.
      else time_ = SHORT_DELAY;         //if any other command has been sent we wait only 300ms.
      Wire.beginTransmission(directCommandAddress);
      Wire.write(command);
      Wire.endTransmission();
      delay(time_);

      Wire.requestFrom(directCommandAddress, 20, 1); //call the circuit and request 20 bytes (this may be more than we need)
      Serial.print("A: ");
      Serial.println(directCommandAddress);
      code = Wire.read();             //the first byte is the response code, we read this separately.
      switch (code) {                 //switch case based on what the response code is.
        case 1:                       //decimal 1.
          Serial.println("Success");  //means the command was successfully received
          break;
        case 2:                        //decimal 2.
          Serial.println("Failed");    //means the command has failed.
          break;                         //exits the switch case.
        case 254:                       //decimal 254.
          Serial.println("Pending");    //means the command has not yet been finished calculating.
          break;
        case 255:                      //decimal 255.
          Serial.println("No Data");   //means there is no further data to send.
          break;
      }

      i = 0;
      while (Wire.available() ) {          //are there bytes to receive.
        in_char = Wire.read();           //receive a byte.
        ph_data[i] = in_char;            //load this byte into our array.
        i += 1;                          //incur the counter for the array element.
        if (in_char == 0) {              //if we see that we have been sent a null command.
          Wire.endTransmission();        //end the I2C data transmission.
          break;
        }
      }
      Serial.println(ph_data);

      isDirectCommand = false;

    }
  }
  //Uncomment this section if you want to take the pH value and convert it into floating point number.
  //ph_float=atof(ph_data);
}






