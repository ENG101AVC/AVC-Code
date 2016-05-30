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
extern "C" int read_analog(int ch_adc);
extern "C" int display_picture(int delay_sec, int delay_usec);

const double PROPORTIONAL = 500;	// Proportional Constant
const double DIFFERENTIAL = 0;		// Differential Constant
const double INTEGRAL = 0;			// Integral Constant

int num_of_decisions = 0;


// This method determines the average white/black levels.

int determine_average(){
	int max = 0;
	int min = 255;
	int average;

	take_picture();							// Takes initial picture.
	for(int i = 0; i<320; i++){

		if(get_pixel(i, 120, 3)>max){		// Establishes Max.
			max = get_pixel(i, 120, 3);
		}

		if(get_pixel(i, 120, 3)<min){		// Establishes Min.
			min = get_pixel(i, 120, 3);

		}
	}

	average = (max+min)/2;
	return average;
}


// This method finds the error for 5 rows and averages them to create one.

int average_error(int i){
	int error1 = get_pixel(i, 120, 3);
	int error2 = get_pixel(i, 118, 3);
	int error3 = get_pixel(i, 122, 3);
	int error4 = get_pixel(i, 116, 3);
	int error5 = get_pixel(i, 124, 3);

	int average_error = (int) (error5+error4+error3+error2+error1)/5;

	return average_error;
}

// This is the network() method.  Opens the gate.

void network(){
	char message[24];							// Assigns memory to password.

	connect_to_server("130.195.6.196", 1024);	// Connects to Gate.
	send_to_server("Please");					// Requests permission.

	receive_from_server(message);				// Assigns the password to 'message'.
	send_to_server(message);					// Sends password to server

	printf(message);
}



// This is the follow_the_line() method.  Should have the RPi follow the line.

void follow_the_line(){

	//Define local variables
	int testClock = 0;							// For testing the RPi.  Can terminate movement.
	int proportional_signal_previous;
	int num_of_white = 0;
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

		if(num_of_white>=310){
			printf("SWITCHING TO TURNING CODE\n");
			set_motor(1, 40);
			set_motor(2, 40);
			Sleep(0,700000);
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
	int threshold = determine_average() + 50;
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
			if(get_pixel(0, 120, 3) >= threshold){
				leftpixel = 1;
			}
			else{
				leftpixel = 0;
			}
		
			if(get_pixel(320, 120, 3) >= threshold) {
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
			printf("%d\n",num_of_white);
			
			if(leftpixel == 1 && rightpixel == 0 && frontpixel == 0){
				printf("Bend: L, Turn: Left\n");
				proportional_signal = -40;
			}
			else if(leftpixel == 0 && rightpixel == 1 && frontpixel == 0){
				printf("Bend: L, Turn: Right\n");
				proportional_signal = 40;
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

// This is the Main() method.  Controls the holistic behaviour or the RPi.

int main(){
	
	init(0);			// Initialise Hardware
			//Assigns a place in memory for average whiteness.	int proportional_signal_previous = 0;

	//network();			// Open Gate
	Sleep(2,0);  //Walking to Track
	follow_the_line();
	Complete_until_maze();    // Begin Following Line


	//Emergency Stop
	set_motor(1, 0);
	set_motor(2, 0);

	return 0;
}
