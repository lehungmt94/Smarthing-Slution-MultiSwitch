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


//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
#define PIN_THING_RX    3
#define PIN_THING_TX    2

#define LED_GREEN		4
#define LED_RED			5
#define LED_BLUE		8
#define LED_YELLOW		7

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

// the setup function runs once when you press reset or power the board
void setup() {
	if (isDebugEnabled)
	{ // setup debug serial port
		Serial.begin(9600);         // setup serial with a baud rate of 9600
		Serial.println("Setup Serial Finish..");  // print out 'setup..' on start
	}
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	pinMode(LED_YELLOW, OUTPUT);
	Serial.println("Setup for I/O Finish..");

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
		smartthing.send("off1");
	}
	else if (message.equals("green1"))
	{
		digitalWrite(LED_GREEN, HIGH);
		smartthing.send("on1");
	}

	//Red Led check event
	if (message.equals("red0"))
	{
		digitalWrite(LED_RED, LOW);
		smartthing.send("off2");
	}
	else if (message.equals("red1"))
	{
		digitalWrite(LED_RED, HIGH);
		smartthing.send("on2");
	}


	//Blue Led check event
	if (message.equals("blue0"))
	{
		digitalWrite(LED_BLUE, LOW);
		smartthing.send("off3");
	}
	else if (message.equals("blue1"))
	{
		digitalWrite(LED_BLUE, HIGH);
		smartthing.send("on3");
	}

	//Yellow Led check event
	if (message.equals("yellow0"))
	{
		digitalWrite(LED_YELLOW, LOW);
		smartthing.send("off4");
	}
	else if (message.equals("yellow1"))
	{
		digitalWrite(LED_YELLOW, HIGH);
		smartthing.send("on4");
	}
	

}
