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

int main(){
    init(0);
    network();
    return 0;
}

void network() {
    connect_to_server("192.168.1.2", 1024); //TODO: fix Server IP
   //sends a message to the connected server
   send_to_server("Hello server");
   //receives message from the connected server
   char message[24];
   receive_from_server(message); //this may be buggy!
   //printf("%s", message);
   send_to_server(message);
}
