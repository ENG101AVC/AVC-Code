#include <stdio.h>
#include <time.h>
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int init(int d_lev);
extern "C" int set_motor(int motor , int speed );
extern "C" int Sleep( int sec , int usec );
extern "C" int take_picture();
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);
extern "C" int display_picture();

const int CONST_PROPORTIONAL = 1;
const int CONST_DIFFERENTIAL = 0;
const int CONST_INTEGRAL = 0;

int SUM_PREV; //FOR WHEN WE USE DIFFERENTIATE
int SUM_TIME; //FOR WHEN WE DIFFERENTIATE

int main(){
    init(0);
    network();
    //follow_the_line();
    return 0;
}

void network() {
    connect_to_server("130.195.6.196", 1024); //TODO: fix Server IP
   //sends a message to the connected server
   send_to_server("Please");

   //receives message from the connected server
   char message[24];
   receive_from_server(message); //this may be buggy!
   //printf("%s", message);
   send_to_server(message);
}

void follow_the_line(){
    take_picture();
    int sum; // 
    int c; //maybe char
    int i;
    for (i=-120; i < 120; i++){
        c = get_pixel(160, i+120, 3);
        if(c>127){c=1;}
        else{c=0;}
        sum = sum + c*i;
    }
    printf("%d", sum); //make sure 127 + sum IS NOT GREATER THAN 255
    
    int motor_adjustment = sum*CONST_PROPORTIONAL;
    if (motor_adjustment > 125 || motor_adjustment < -125){
	motor_adjustment=125;
    }
	
    
    set_motor(1, 127 + motor_adjustment);
    set_motor(2, 127 - motor_adjustment);
}


