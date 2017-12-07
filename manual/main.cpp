#include "mbed.h"
#include "m3pi_ng.h"
#include "btbee.h"
#include <cmath>

#define up 1
#define down 2
#define left 4
#define right 8
#define escape 16

m3pi lildicky;
btbee bt;
Timer timer;

bool vroom(int input, float &vRight, float &vLeft) {
    bool turnCheck = false;
    if((input & escape) != 0) {   // RESET
        vRight = 0;
        vLeft = 0;
    } 
          
    if((input & up) != 0) {       // UP
        vRight += .04;
        vLeft += .04;
    }
           
    if((input & down) != 0) {     // DOWN
        vRight -= .05;
        vLeft -= .05;   
    }
    
    if ((input & right) != 0) {   // RIGHT TURN
        vRight -= .03;
        vLeft += .03;
        turnCheck = true;
    }
            
    if ((input & left) != 0) {    // LEFT TURN
        vRight += .03;
        vLeft -= .03;
        turnCheck = true;
    }
    return turnCheck;
} 

void range(float &value){
    if(value > 1) {
        value = 1;
    } else if(value < -1) {
        value = -1;
    } 
}

int main() {
    float vRight = 0, vLeft = 0;
    char btinput[2];
    int inputAddress;
    bool turnCheck = false;
    lildicky.cls();
    
    while (true) {    
        inputAddress = 0;
        turnCheck = false;
        bt.read_all(btinput, 2, &inputAddress); // fetch bt data
        
        if(inputAddress && btinput[0] == 13){   // if valid data
            int input = btinput[1];   
            turnCheck = vroom(input, vRight, vLeft);
        }
        
        // go straight if not turning
        if(turnCheck == false) { 
            double total = ((double) vRight + (double) vLeft) / 2;
            vRight = vLeft = (float) total;
        }
              
        // check for values too large             
        range(vRight); 
        range(vLeft);
            
        lildicky.right_motor(vRight);
        lildicky.left_motor(vLeft);
        wait(0.1);
    }
}