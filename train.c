#include <taskLib.h>
#include <logLib.h>
#include <ioLib.h>
#include <msgQLib.h>
#include <stdio.h>

/* Global data structures */
#define ENGINE_ADDRESS 1
#define CHECK_DISTANCE 100

unsigned char SerBuf[3], x;
int ser_fd;
int dir;
int choice;
int valid;
int numByteWritten;
int bellOn;
int distance;
int leftStation;

/* Function declarations */
int init(void);
int dispose(void);
void sendCommand(unsigned char cmd);
void ringBell(void);
void startEngine(void);
void stopEngine(void);
void blowHorn(void);

int main(int argc, const char* argv[])
{
	printf("CS 4397.001 Project 1: Trainmaster Train System\n");
	printf("===============================================\n\n");
	
	printf("Starting train...\n");
	init();
	bellOn = 0;
	mode=1;
	
	/*modes: 0=waiting to leave station, 1=departing, 2=cruising, 3=arriving at station*/
	while(1)
	{
		/* TODO: distance = getDistance() */
		
		switch(mode)
		{
			case 0: break; /* TODO: wait 6000 milliseconds, then blow horn, etc. */
			
			case 1: if(distance>CHECK_DISTANCE) mode=2;
				break; /*TODO: set speed relative to distance from station */
				
			case 2: if(distance<CHECK_DISTANCE) mode=3; 
				break;
			case 3: break; /*TODO: set speed relative to distance from station, stop when distance is very small */
		}
	}
}

/*
   Initializes the serial connection.
   Returns: 1 if connection was successfully opened, -1 otherwise.
*/
int init(void)
{
	/* Open serial port */
	ser_fd = open("/tyCo/1", O_WRONLY, 0);
	printf("\nTrain ID: %d\n", ENGINE_ADDRESS);

	if (ser_fd == -1)
	{
		printf("[ERROR] Unable to open serial port.\n");
		return -1;
	}
	else
	{
		printf("Successfully opened serial port; fd = %d\n", ser_fd);
		return 1;
	}
}

/*
   Closes the serial connection.
   Returns: 0 if connection was successfully closed, -1 otherwise.
*/
int dispose(void)
{
	if (ser_fd > 0)
	{
		return close(ser_fd);
	}

	return -1;
}

/*
   Sends a command over the serial connection.
*/
void sendCommand(unsigned char cmd)
{
	/* Ensure serial connection is open */
	if (ser_fd <= 0)
	{
		init();
	}
	
	x = cmd;
	
	/* Write to serial connection */
	numByteWritten = 0;
	SerBuf[0] = (unsigned char)254;
	SerBuf[1] = (unsigned char)0;
	SerBuf[2] = cmd;
	numByteWritten = write(ser_fd, SerBuf, 3);
}

/*
   Instructs the train to start its engine.
*/
void startEngine(void)
{
	/* Set direction, set speed to 10, and boost to start moving */
	sendCommand(0x80);
	sendCommand(0xf0);
	sendCommand(0x84);
}

/*
   Instructs the train to stop its engine.
*/
void stopEngine(void)
{
	/* Activate brakes and set speed to 0 */
	sendCommand(0x87);
	sendCommand(0xe0);
}

/*
   Instructs the train to ring its bell.
*/
void ringBell(void)
{
	/* Ring bell */
	sendCommand(0x9d);
	/* sendCommand(0x9d); */
	bellOn = bellOn == 0 ? 1 : 0;
}

/*
   Instructs the train to blow its horn.
*/
void blowHorn(void)
{
	/* Blow horn */
	sendCommand(0x9c);
}
