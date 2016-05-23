// Import Libraries
#include <stdio.h>
#include <time.h>

// Import method statements. 
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int init(int d_lev);
extern "C" int set_motor(int motor , int speed );
extern "C" int Sleep( int sec , int usec );
extern "C" int take_picture();
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int display_picture(int delay_sec, int delay_usec);

const double PROPORTIONAL = 500;	// Proportional Constant
const double DIFFERENTIAL = 0;	// Differential Constant
const double INTEGRAL = 0;		// Integral Constant

void navigate_maze(){

	//Define local variables
	int testClock = 0;						// For testing the RPi.  Can terminate movement.
	int threshold = determine_average();	//Assigns a place in memory for average whiteness.	int proportional_signal_previous = 0;
	int proportional_signal_previous;

	while(testClock < 1500){

		bool seeLine = false;					// Whether or not the line can be seen.
		int current_error = 0;
		int num_of_white = 0;

		take_picture();

		for(int i=0; i<320; i++){
			int error = average_error(i);

			if(error >= threshold){			// If RPi sees 'white'
				error = 1;				// Converts to binary represenation
				seeLine = true;			// The Line can be seen
				num_of_white++;
			}
			else{							// If RPi sees 'black'
				error = 0;				// Converts to binary representation
			}

			current_error = current_error + error*(i-160);
		}

		//if(whiteness > 300){return;}	// Moves to White Line Maze

		int proportional_signal = (int) (current_error/PROPORTIONAL);	//Sets proportional signal

		// Print checks:
		printf("Current Error: %d\n", current_error);
		printf("Proportional Signal: %d\n", proportional_signal);
		printf("Number of White Pixels: %d\n", num_of_white);

		if(seeLine){
			set_motor(1, 40+proportional_signal);
			set_motor(2, 40-proportional_signal);
			proportional_signal_previous = proportional_signal;
		}
		else{
			set_motor(1, 50+proportional_signal_previous*6);
			set_motor(2, 50-proportional_signal_previous*6);
		}

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
