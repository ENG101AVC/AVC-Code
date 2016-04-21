#include <stdio.h>
#include <time.h>
extern "C" int InitHardware();
extern "C" int WriteDigital(int chan, char level);
extern "C" int Sleep(int sec, int usec);
extern "C" int ReadAnalog(int ch_adc);

int main(){
    InitHardware();
    
    //For following the line.  First thoughts from Lecture 20/04/16
    take_picture();
    int sum = 0;
    int i;
    for (i=-160, i<160, i++){
        w = get_pixel(120, i, 3);
        if(w<127){
            w = 0;
        }
        else if(w>=127){
            w = 255;
        }
        sum = sum+i*w;
    }
}
