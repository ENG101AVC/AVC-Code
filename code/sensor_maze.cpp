// Import Libraries
#include <stdio.h>
#include <time.h>

// Import method statements. 
extern "C" int init(int d_lev);
extern "C" int set_motor(int motor , int speed );
extern "C" int read_analog(int ch_adc);
extern "C" int read_digital(int chan, int direct);
extern "C" int select_IO(int chan, int direct);
extern "C" int Sleep( int sec , int usec );

void navigate_maze(){
	//Define local variables
	int testClock = 0; //For testing the RPi.  Can terminate movement.
	
	//For Data collection
	int left; 
	int right;
	int front;
	
	int select_IO(0, 1);

	printf("Running Analog Code");
	while(testClock < 1000){
		//Sets 
		left = read_analog(0);
		right = read_analog(1);
		//front = read_analog(2);
		front = read_digital(0);
		
		if(front < /*value*/)
		{
			if (left > 450)
			{
				set_motor(1, -45);
				set_motor(2, 45);	
			}
			else if(right > 450){
				set_motor(1, 45);
				set_motor(2, -45);	
			}
			else{
				set_motor(1, -80);
				set_motor(2, -80);	
			}
		}
		else
		{
			set_motor(1, 80);
			set_motor(2, 80);
		}
		
		Sleep(0,500000);
		
		testClock++;
	}
	
	set_motor(1, 0);
	set_motor(2, 0);
	return;
}

// This is the Main() method.  Controls the holistic behaviour or the RPi.

int main(){
	
	init(0);		// Initialise Hardware

	navigate_maze();	// Navigate the Walled Maze

	//Emergency Stop
	set_motor(1, 0);
	set_motor(2, 0);

	return 0;
}
