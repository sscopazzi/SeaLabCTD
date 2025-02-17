// salinity
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 2                                          //define what pin rx is going to be
#define tx 3                                          //define what pin tx is going to be
SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work
String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

//data init
double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;
float sensTemp, sensPressure, sensDepth;

// pressure & temp
#include <Wire.h>
#include "MS5837.h"
MS5837 newSensor;

// VIZI
//#include <string>
#include <SD.h>
char filename[20];
int linecount = 0;
int one_flag = 0;
int time_minute = 0, time_second = 0;
unsigned long start_time = millis(), curr_time = 0;

// file stuff
File sd_count;
File sd_log;

////////
// VIZI - creates valid file name (skipping previously used names)
//  + attempts to create a file named "log_###.txt"
//    if file already exists, increment count and try again
void file_namer() {
		int count = 0;
		snprintf(filename, sizeof(filename), "log_%03d.txt", count);
		while (SD.exists(filename)) {
				count++;
				snprintf(filename, sizeof(filename), "log_%03d.txt", count);
		}
}
////////

////////
// VIZI - calculate elapsed time
//  + grabs starting time in milliseconds, (essentially 0)
//    subtracts start time from current time
//    divide by 1000 to convert to seconds
//
//  + divide by 60 to convert to minutes
//  + modulo by 60 to convert remaining seconds
void time_master() {
		curr_time = millis();

		int inner_seconds = ((curr_time - start_time) / 1000);
		time_minute = inner_seconds / 60;
		time_second = inner_seconds % 60;
}
////////

////////
// VIZI - Start logging and output debug to console
//  + open log file using 'char filename[20]' created in file_namer()
//  
//  + if log file is NOT opened properly, output error to console
//    otherwise, announce active, filename, lines of data, runtime
void sd_logfile() {
	sd_log = SD.open(filename, FILE_WRITE);
	if (!sd_log) {
		Serial.println("[[ SD LOG INACTIVE ]]");
		Serial.println("[[ !! DATA NOT BEING RECORDED !! ]]");
	}
	else if (sd_log) {
		Serial.println("[[ SD_LOG ACTIVE ]]");
		Serial.print("filename : ");
		Serial.println(filename);
		Serial.print("lines of data : ");
		Serial.println(linecount);

		// print time to console
		Serial.print("runtime (minutes : seconds) : ");
		Serial.print(time_minute);
		Serial.print(":");
		if (time_second < 10) {
				Serial.print('0');
		}
		Serial.println(time_second);
	}
}
////////

// MOVED OUT OF LOOP DUE TO FUNCTION ERROR
void serialEvent() {                                  //if the hardware serial port_0 receives a char
	inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
	input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}

void print_EC_data(void) {         //this function will parse the string 

  char sensorstring_array[30];                        //we make a char array
  char *EC;                                           //char pointer used in string parsing
  float f_ec;                                         //used to hold a floating point number that is the EC
  
  sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
  EC = strtok(sensorstring_array, ",");               //let's pars the array at each comma
 
  Serial.print(EC);                                 //this is the EC value
  Serial.print(",");

  ////////
  // VIZI - print EC data to card
  sd_log.print(EC);
  sd_log.print(",");
  ////////

  // sensors.requestTemperatures(); 
  
  // // goes through all devices, prints by index at sensors
  // for (int i = 0;  i < deviceCount;  i++) {
  //  tempC = sensors.getTempCByIndex(i);
  //  Serial.print(tempC);
  //  Serial.print(",");

  //  /////////
  //  // VIZI - print remaining sensors
  //  sd_log.print(tempC);
  //  sd_log.print(",");
  //  ////////
  //}

}

void setup() {
		// computer stuff
		//set baud rate for the hardware serial port_0 to 9600
		Serial.begin(9600);                                 
		//set baud rate for the software serial port to 9600
		myserial.begin(9600);                               
		
		// salinity sensor setup
		//set aside some bytes for receiving data from the PC
		inputstring.reserve(10);
		//set aside some bytes for receiving data from Atlas Scientific product
		sensorstring.reserve(30);                           

		// begin wire stuff
		Wire.begin();

		// Initialize pressure sensor
		// Returns true if initialization was successful
		// We can't continue with the rest of the program unless we can initialize the sensor
		while (!newSensor.init()) {
		Serial.println("Init failed!");
		Serial.println("Are SDA/SCL connected correctly?");
		Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
		Serial.println("\n\n\n");
		delay(5000);
	}

		// .init sets the sensor model for us but we can override it if required.
		// Uncomment the next line to force the sensor model to the MS5837_30BA.
		//sensor.setModel(MS5837::MS5837_30BA);

		newSensor.setFluidDensity(1029); // kg/m^3 (freshwater, 997 for freshwater)

		////////
		// VIZI - SD card init
		delay(500);

		Serial.print("Initializing SD card");
		Serial.println("...");

		// arduino output -> sd card 10
		pinMode(10, OUTPUT);

		if (!SD.begin(10)) { // if 10 not active
		Serial.println("[[error]] sd card bad init");
		return; // close setup  
		}
		Serial.println("init done");

		file_namer();
		////////

}

void loop() {
	// gets time
	time_master();

	////////
	// INPUT INFORMATION
	if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
		myserial.print(inputstring);                      //send that string to the Atlas Scientific product
		myserial.print('\r');                             //add a <CR> to the end of the string
		inputstring = "";                                 //clear the string
		input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
	}
	////////
	
	///////
	// READ SALINITY SENSOR
	if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
		char inchar = (char)myserial.read();              //get the char we just received
		sensorstring += inchar;                           //add the char to the var called sensorstring
		if (inchar == '\r') {                             //if the incoming character is a <CR>
			sensor_string_complete = true;                  //set the flag
		}
	}
	
  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    ////////
    // VIZI - enable log file logging
    sd_logfile();
    ////////

		if (isdigit(sensorstring[0]) == false) {          //if the first character in the string is a digit
			Serial.println(sensorstring);                   //send that string to the PC's serial monitor
		} else {                                             //if the first character in the string is NOT a digit
			////////
			// VIZI - print time elapsed
			sd_log.print(time_minute);
			sd_log.print(":");
			if (time_second < 10) {
				sd_log.print('0');
			}
			sd_log.print(time_second);
			sd_log.print(",");
			////////

			print_EC_data();                                //then call this function 

			// get data from MSS5837
			newSensor.read();
			
			// temp data capture
			sensTemp = newSensor.temperature();
			Serial.println(sensTemp);
			// pressure data capture
			sensPressure = newSensor.pressure();
			Serial.println(sensPressure);
			// depth data capture
			sensDepth = newSensor.depth();
			Serial.println(sensDepth);

			////////
			// VIZI - Print the temp, pressure, depth with newlines and incrementations
			linecount++;
			sd_log.println(sensTemp);
			linecount++;
			sd_log.println(sensPressure);
			linecount++;
			sd_log.println(sensDepth);
			////////
		}

		sensorstring = ""; // reset string
		sensor_string_complete = false; // reset capture flag
	}

	////////
	// VIZI - close and save
	sd_log.close();
	////////
}
