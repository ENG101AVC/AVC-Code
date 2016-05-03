#include <stdio.h>
#include <time.h>
extern "C" int InitHardware();
extern "C" int WriteDigital(int chan, char level);
extern "C" int Sleep(int sec, int usec);
extern "C" int ReadAnalog(int ch_adc);

int main(){
    InitHardware();
}

int forward(int time){
    
}
