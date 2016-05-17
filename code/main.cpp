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
extern "C" int display_picture(int delay_sec, int delay_usec);

const double CONST_PROPORTIONAL = 10;
const int CONST_DIFFERENTIAL = 0;
const int CONST_INTEGRAL = 0;

int SUM_PREV; //FOR WHEN WE USE DIFFERENTIATE
int SUM_TIME; //FOR WHEN WE DIFFERENTIATE

//LEFT wheel is motor ONE
//RIGHT wheel is motor TWO

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
    int testClock = 0;
    int max_val = 0;
    int min_val = 255;
    
     for(int i = 0; i < 320; i++){
            if(get_pixel(i, 120, 3) > max_val){
                max_val = get_pixel(i, 120, 3);
            }
            if(get_pixel(i, 120, 3)< min_val){
                min_val = get_pixel(i, 120, 3);
            }
        }
    
    while(testClock<200){
        
        take_picture();
        double sum_of_pixels = 0;  
        int specific_pixel= 0; 
        
       
       
        
        for (int iteration=-160; iteration < 160; iteration++){
            specific_pixel = get_pixel(iteration+160, 120, 3);
            if(specific_pixel>(max_val-min_val)/1.5+min_val){specific_pixel=1;}
            else{specific_pixel=0;}
            
            sum_of_pixels = sum_of_pixels + specific_pixel*iteration;
        }
        printf("\n");
    
        double proportional_signal = sum_of_pixels;
        
        printf("The Proportional Signal is:  %f\n", proportional_signal);
    
        //Ideally sum_of_pixels = 0.
        set_motor(1, 50+(proportional_signal/(160*-1*CONST_PROPORTIONAL)));
        set_motor(2, 50+(proportional_signal/(160*1*CONST_PROPORTIONAL)));
     
        testClock++;
    }
}

void maze_navigation(){
    //Try get both sensors to detect around the same number [Same distance apart/Center of Maze Walls]
    //If one side is just larger the the other do turn script, if really large turn that direction.
}

//LEAVE AT BOTTOM OF CODE
int main(){
    init(0);
    //network();
    follow_the_line();
    //maze_navigation();


    
    set_motor(1,0);
    set_motor(2,0);
    
    return 0;
}
