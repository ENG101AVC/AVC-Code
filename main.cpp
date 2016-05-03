#include <stdio.h>
#include <time.h>
extern "C" char get_pixel(int row,int col,int colour);
extern "C" int init(int d_lev);
extern "C" int set_motor(int motor , int speed );
extern "C" int Sleep( int sec , int usec );
extern "C" int take_picture();

int main(){
    init(0);
    set_motor(1, 255);
    set_motor(2, 255);
    Sleep(1,0);
    set_motor(1,0);
    set_motor(2,0);
    
//    //For following the line.  First thoughts from Lecture 20/04/16
//    take_picture();
//    int sum = 0;
//    int i;
//    for (i=-160, i<160, i++){
//        w = get_pixel(120, i, 3);
//        if(w<127){
//            w = 0;
//        }
//        else if(w>=127){
//            w = 255;
//        }
//        sum = sum+i*w;
//    }
}
