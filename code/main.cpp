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

const double CONST_PROPORTIONAL = 1;
const int CONST_DIFFERENTIAL = 0;
const int CONST_INTEGRAL = 0;

int prev_sum; //FOR WHEN WE USE DIFFERENTIATE
int prev_time; //FOR WHEN WE DIFFERENTIATE

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
   int proportional_signal = 0;
   double derivative_signal = 0;
   double sum_of_pixels = 0;
   int specific_pixel= 0;
   prev_time = (int)time(NULL);

   while(testClock<200){

      take_picture();
      //GET max and min values to use
      for(int i = 1; i < 320; i++){
         if(get_pixel(i, 120, 3) > max_val){
            max_val = get_pixel(i, 120, 3);
         }
         if(get_pixel(i, 120, 3)< min_val){
            min_val = get_pixel(i, 120, 3);
         }
      }

      for (int i=-160; i < 160; i++){
         specific_pixel = get_pixel(i+161, 120, 3);
         if(specific_pixel>(max_val-min_val)/1.5+min_val){specific_pixel=1;}
         else{specific_pixel=0;}

         sum_of_pixels = sum_of_pixels + specific_pixel*i;
      }
      printf("\n");

      proportional_signal = sum_of_pixels * CONST_PROPORTIONAL;
      derivative_signal = CONST_DIFFERENTIAL * (double)(sum_of_pixels - prev_sum)/((int)time(NULL)-prev_time);

      prev_sum = sum_of_pixels;
      prev_time = (int)time(NULL);

      printf("The Proportional Signal is:  %f\n", proportional_signal);
      printf("The Derivative Signal is: %f\n\n", derivative_signal);

      //Ideally sum_of_pixels = 0.
      set_motor(1, 50+(proportional_signal/160*-1*CONST_PROPORTIONAL));
      set_motor(2, 50+(proportional_signal/160*1*CONST_PROPORTIONAL));

      testClock++;
   }
}

void beta_follow_the_line(){
   int testClock = 0;
   int max_val = 0;
   int min_val = 255;
   int current_error = 0;
   double kp = 0.5;
   int proportional_signal = 0;
   while(testClock<100){
      take_picture();
      int error = 0;
      for (int i=1; i<320; i++){
         error = get_pixel(i, 120, 3);
         if(error>max_val){max_val = error;}
         else if(error<min_val){min_val = error;}
      }

      for (int i=1; i<320; i++){
         //get the 'whiteness' of a pixel
         error = get_pixel(i, 120, 3);
         //if the error is greater than the middle of the range
         if (error >= (max_val + min_val)/2) {
            error = 1;
         }
         else { error = 0; }
         printf("%d: %d\n", i, error);
         current_error = current_error + (i-160)*error;
         /* printf("Inside For Loop"); */
      }
      /* printf("Terminated for Loop"); */
      proportional_signal = current_error*kp;
      /* printf("Proportional Signal"); */
      printf("Proportional signal is: %d\n", proportional_signal );

      set_motor(1, (proportional_signal/(160*1*kp))*255);
      set_motor(2, (proportional_signal/(160*-1*kp))*255);
      /* testClock++; */
   }
}

void maze_navigation(){
   //Try get both sensors to detect around the same number [Same distance apart/Center of Maze Walls]
   //If one side is just larger the the other do turn script, if really large turn that direction.
}

int main(){
   printf("%d", (int)time(NULL));
   init(0);
   //network();
   //follow_the_line();
   beta_follow_the_line();
   //maze_navigation();


   //Turn off motors
   set_motor(1,0);
   set_motor(2,0);

   return 0;
}
