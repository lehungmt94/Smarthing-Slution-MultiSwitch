/*
 Name:		MulitSwitch.ino
 Created:	12/14/2016 2:56:44 PM
 Author:	Le Hung
 Email:		lehungmt94@gmail.com
 Company:	Deki Connet
 Website:	xinkciti.com
			mculearning.wordpress.com
*/


#include <SoftwareSerial.h>
#include <SmartThings.h>
#include <EEPROM.h>


//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
// (and EEROM Adress)   V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
#define PIN_THING_RX    3
#define PIN_THING_TX    2

#define LED_GREEN		7
#define LED_RED			8
#define LED_BLUE		9
#define LED_YELLOW		10

#define LED_GREEN2		11
#define LED_RED2		12
//#define LED_BLUE2		0
//#define LED_YELLOW2	0


//*****************************************************************************
// Remember satus Definitions of Switch     | | | | | | | | | | | | | | | | | | 
//                                          V V V V V V V V V V V V V V V V V V
//*****************************************************************************
#define ON_STATUS		1
#define OFF_STATUS		0

//*****************************************************************************
// Debug config                             | | | | | | | | | | | | | | | | | | 
//                                          V V V V V V V V V V V V V V V V V V
//*****************************************************************************
#define isDebugEnabled  1


//*****************************************************************************
// Global Variables   | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
SmartThingsCallout_t messageCallout;    // call out function forward decalaration
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor
int stateNetwork;       // state of the network

void setNetworkStateLED()
{
	SmartThingsNetworkState_t tempState = smartthing.shieldGetLastNetworkState();
	if (tempState != stateNetwork)
	{
		switch (tempState)
		{
		case STATE_NO_NETWORK:
			if (isDebugEnabled) Serial.println("NO_NETWORK");
			//smartthing.shieldSetLED(2, 0, 0); // red
			break;
		case STATE_JOINING:
			if (isDebugEnabled) Serial.println("JOINING");
			//smartthing.shieldSetLED(2, 0, 0); // red
			break;
		case STATE_JOINED:
			if (isDebugEnabled) Serial.println("JOINED");
			//smartthing.shieldSetLED(0, 0, 0); // off
			break;
		case STATE_JOINED_NOPARENT:
			if (isDebugEnabled) Serial.println("JOINED_NOPARENT");
			//smartthing.shieldSetLED(2, 0, 2); // purple
			break;
		case STATE_LEAVING:
			if (isDebugEnabled) Serial.println("LEAVING");
			//smartthing.shieldSetLED(2, 0, 0); // red
			break;
		default:
		case STATE_UNKNOWN:
			if (isDebugEnabled) Serial.println("UNKNOWN");
			//smartthing.shieldSetLED(0, 2, 0); // green
			break;
		}
		stateNetwork = tempState;
	}
}


void IOconfig(void) {
	//Setup ouput for all LED (is relay in PCB design)
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	pinMode(LED_YELLOW, OUTPUT);
	pinMode(LED_GREEN2, OUTPUT);
	pinMode(LED_RED2, OUTPUT);
}

void getRememberEEROM(void) {

	//Get remember status LED_GREEN in EEROM 
	if(EEPROM.read(LED_GREEN)==ON_STATUS)
		digitalWrite(LED_GREEN, HIGH);
	else
		digitalWrite(LED_GREEN, LOW);
	
	//Get remember status LED_RED in EEROM 
	if (EEPROM.read(LED_RED) == ON_STATUS)
		digitalWrite(LED_RED, HIGH);
	else
		digitalWrite(LED_RED, LOW);

	//Get remember status LED_BLUE in EEROM 
	if (EEPROM.read(LED_BLUE) == ON_STATUS)
		digitalWrite(LED_BLUE, HIGH);
	else
		digitalWrite(LED_BLUE, LOW);

	//Get remember status LED_YELLOW in EEROM 
	if (EEPROM.read(LED_YELLOW) == ON_STATUS)
		digitalWrite(LED_YELLOW, HIGH);
	else
		digitalWrite(LED_YELLOW, LOW);

	//Get remember status LED_GREEN2 in EEROM 
	if (EEPROM.read(LED_GREEN2) == ON_STATUS)
		digitalWrite(LED_GREEN2, HIGH);
	else
		digitalWrite(LED_GREEN2, LOW);

	//Get remember status LED_RED2 in EEROM 
	if (EEPROM.read(LED_RED2) == ON_STATUS)
		digitalWrite(LED_RED2, HIGH);
	else
		digitalWrite(LED_RED2, LOW);
}
// the setup function runs once when you press reset or power the board
void setup() {
	if (isDebugEnabled)
	{ // setup debug serial port
		Serial.begin(9600);         // setup serial with a baud rate of 9600
		Serial.println("Setup Serial Finish..");  // print out 'setup..' on start
	}

	IOconfig();
	Serial.println("Setup for I/O Finish..");

	getRememberEEROM();
	Serial.println("Setup for status befor shutdown Finish..");
}

// the loop function runs over and over again until power down or reset
void loop() {
	smartthing.run();
	setNetworkStateLED();
}

void messageCallout(String message)
{
	// if debug is enabled print out the received message
	if (isDebugEnabled)
	{
		Serial.print("Received message: '");
		Serial.print(message);
		Serial.println("' ");
	}

	// if message contents equals to 'on' then call on() function
	// else if message contents equals to 'off' then call off() function
	
	//Green Led check event
	if (message.equals("green0"))
	{
		digitalWrite(LED_GREEN, LOW);
		smartthing.send("off1");					//responsive to Smartthing Hub
		EEPROM.write(LED_GREEN, OFF_STATUS);		//remember status off led green
	}
	else if (message.equals("green1"))
	{
		digitalWrite(LED_GREEN, HIGH);
		smartthing.send("on1");						//responsive to Smartthing Hub
		EEPROM.write(LED_GREEN, ON_STATUS);			//remember status on led green
	}

	//Red Led check event
	if (message.equals("red0"))
	{
		digitalWrite(LED_RED, LOW);
		smartthing.send("off2");
		EEPROM.write(LED_RED, OFF_STATUS);
	}
	else if (message.equals("red1"))
	{
		digitalWrite(LED_RED, HIGH);
		smartthing.send("on2");
		EEPROM.write(LED_RED, ON_STATUS);
	}


	//Blue Led check event
	if (message.equals("blue0"))
	{
		digitalWrite(LED_BLUE, LOW);
		smartthing.send("off3");
		EEPROM.write(LED_BLUE, OFF_STATUS);
	}
	else if (message.equals("blue1"))
	{
		digitalWrite(LED_BLUE, HIGH);
		smartthing.send("on3");
		EEPROM.write(LED_BLUE, ON_STATUS);
	}

	//Yellow Led check event
	if (message.equals("yellow0"))
	{
		digitalWrite(LED_YELLOW, LOW);
		smartthing.send("off4");
		EEPROM.write(LED_YELLOW, OFF_STATUS);
	}
	else if (message.equals("yellow1"))
	{
		digitalWrite(LED_YELLOW, HIGH);
		smartthing.send("on4");
		EEPROM.write(LED_YELLOW, ON_STATUS);
	}

	//Green2 check event
	if (message.equals("greenb0"))
	{
		digitalWrite(LED_GREEN2, LOW);
		smartthing.send("off5");
		EEPROM.write(LED_GREEN2, OFF_STATUS);
	}
	else if (message.equals("greenb1"))
	{
		digitalWrite(LED_GREEN2, HIGH);
		smartthing.send("on5");
		EEPROM.write(LED_GREEN2, ON_STATUS);
	}

	//RED2 check event
	if (message.equals("redb0"))
	{
		digitalWrite(LED_RED2, LOW);
		smartthing.send("off6");
		EEPROM.write(LED_RED2, OFF_STATUS);
	}
	else if (message.equals("redb1"))
	{
		digitalWrite(LED_RED2, HIGH);
		smartthing.send("on6");
		EEPROM.write(LED_RED2, ON_STATUS);
	}


	

}
