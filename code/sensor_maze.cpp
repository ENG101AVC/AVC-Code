// Import Libraries
#include <stdio.h>
#include <time.h>

// Import method statements. 
extern "C" int init(int d_lev);
extern "C" int set_motor(int motor , int speed );
extern "C" int read_digital(int chan, int direct);
extern "C" int select_IO(int chan, int direct);
extern "C" int Sleep( int sec , int usec );

void navigate_maze(){
	//Define local variables
	int testClock = 0; //For testing the RPi.  Can terminate movement.

	printf("Running Analog Code");
	while(testClock < 1000){
		
		
		testClock++;
	}
	
	testClock = 0;
	set_motor(1, 0);
	set_motor(2, 0);
	
	printf("Running Digital Code");
	while(testClock < 1000){
		
		
		testClock++;
	}
	
	set_motor(1, 0);
	set_motor(2, 0);
	return;
}

// This is the Main() method.  Controls the holistic behaviour or the RPi.

int main(){
	
	init(0);			// Initialise Hardware

	navigate_maze();	// Navigate the Walled Maze

	//Emergency Stop
	set_motor(1, 0);
	set_motor(2, 0);

	return 0;
}
