// Import Libraries
#include <stdio.h>
#include <time.h>

// Import method statements. 
extern "C" int init(int d_lev);
extern "C" int set_motor(int motor , int speed );
extern "C" int read_analog(int ch_adc);
extern "C" int read_digital(int chan);
extern "C" int select_IO(int chan, int direct);
extern "C" int Sleep( int sec , int usec );

// Don't Modify This: is a backup!

/*void follow_the_line(){

	//Define local variables
	int testClock = 0;							// For testing the RPi.  Can terminate movement.
	int proportional_signal_previous;
	int num_of_white = 0;
	

	while(true){

		bool seeLine = false;					// Whether or not the line can be seen.
		int current_error = 0;
		num_of_white = 0;
		int threshold = determine_average();

		take_picture();

		for(int i=0; i<320; i++){
			int error = average_error(i);

			if(error >= threshold){				// If RPi sees 'white'
				error = 1;						// Converts to binary represenation
				seeLine = true;					// The Line can be seen
				num_of_white++;
			}
			else{								// If RPi sees 'black'
				error = 0;						// Converts to binary representation
			}

			current_error = current_error + error*(i-160);
		}

		int proportional_signal = (int) (current_error/PROPORTIONAL);	//Sets proportional signal

		// Print checks:
		//printf("Current Error: %d\n", current_error);
		//printf("Proportional Signal: %d\n", proportional_signal);
		//printf("Number of White Pixels: %d\n", num_of_white);
		printf("%d\n", num_of_white);

		if(num_of_white>=280){
			printf("SWITCHING TO TURNING CODE\n");
			set_motor(1, 35);
			set_motor(2, 35);
			Sleep(1,0);
			return;
		}
		else if(seeLine){
			set_motor(1, 35+proportional_signal);
			set_motor(2, 35-proportional_signal);
			proportional_signal_previous = proportional_signal;
		}
		else{
			set_motor(1, 50+proportional_signal_previous*7);
			set_motor(2, 50-proportional_signal_previous*7);
		}

	testClock++;
	}
	set_motor(1, 0);
	set_motor(2, 0);
	return;
}




void Complete_until_maze(){ //Experimental
	//Define local variables
	int testClock = 0;							// For testing the RPi.  Can terminate movement.
	int proportional_signal_previous;
	int num_of_white = 0;
	int leftpixel = 0;
	int rightpixel = 0;
	int frontpixel = 0;
	int threshold = determine_average();

	while(true){

		bool seeLine = false;					// Whether or not the line can be seen.
		int current_error = 0;
		num_of_white = 0;

		take_picture();

		for(int i=0; i<320; i++){
			int error = average_error(i);

			if(error >= threshold){				// If RPi sees 'white'
				error = 1;						// Converts to binary represenation
				seeLine = true;					// The Line can be seen
				num_of_white++;
			}
			else{								// If RPi sees 'black'
				error = 0;						// Converts to binary representation
			}

			current_error = current_error + error*(i-160);
		}

		int proportional_signal = (int) (current_error/PROPORTIONAL);	//Sets proportional signal

		// Print checks:
		//printf("Current Error: %d\n", current_error);
		//printf("Proportional Signal: %d\n", proportional_signal);
		//printf("Number of White Pixels: %d\n", num_of_white);
		
		//Turn
		if(num_of_white > 150){
			if(average_error(0) >= threshold){
				leftpixel = 1;
			}
			else{
				leftpixel = 0;
			}
		
			if(average_error(320) >= threshold) {
				rightpixel = 1;
			}
			else{
				rightpixel = 0;
			}
			if(get_pixel(160, 0, 3) >= threshold) {
				frontpixel = 1;
			}
			else{
				frontpixel = 0;
			}
			
			
			if(leftpixel == 1 && rightpixel == 1 && num_of_white>310 && frontpixel == 0){
				printf("Bend: T, Turn: Left\n");
				proportional_signal = -20;
			}
			else if(leftpixel == 1 && rightpixel == 0 && frontpixel == 0){
				printf("Bend: L, Turn: Left\n");
				proportional_signal = -20;
			}
			else if(leftpixel == 0 && rightpixel == 1 && frontpixel == 0){
				printf("Bend: L, Turn: Right\n");
				proportional_signal = 20;
			}
			else if(leftpixel == 1 && rightpixel == 0 && frontpixel == 1){
				printf("Bend: -|, Turn: Front\n");
				proportional_signal = 0;
			}
			else if(leftpixel == 0 && rightpixel == 1 && frontpixel == 1){
				printf("Bend: |-, Turn: Front\n");
				proportional_signal = 0;
			}
			
		}
		
		if(seeLine){
			set_motor(1, 30+proportional_signal);
			set_motor(2, 30-proportional_signal);
			proportional_signal_previous = proportional_signal;
		}
		
		if(read_analog(0) > 550 && read_analog(1) > 550)  //If on Red Square (Maze Detector)
		{
			printf("SWITCHING TO WALLED CODE\n");
			return;
		}
		
	testClock++;
	}
	set_motor(1, 0);
	set_motor(2, 0);
	return;

}
*/

void navigate_maze(){
	//Define local variables
	int testClock = 0; //For testing the RPi.  Can terminate movement.
	
	//For Data collection
	int left; 
	int right;
	int front;
	int dir = 1;
	
	int select_IO(0, 1);

	printf("Running Analog Code");
	while(testClock < 1000){
		//Sets 
		left = read_analog(0);
		right = read_analog(1);
		//front = read_analog(2);
		front = read_digital(0);
		
		if(dir == 1)
		{
			if(front > 1)
			{
				set_motor(1,50);
				set_motor(2,50);
			}
			else{
				set_motor(1,50);
				set_motor(2,-50);
			}
		}
		else
		{
			if(front > 1)
			{
				set_motor(1,50);
				set_motor(2,50);
			}
			else{
				set_motor(1,-50);
				set_motor(2,50);
			}	
		}
		
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
