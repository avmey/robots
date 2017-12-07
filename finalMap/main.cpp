#include "mbed.h"
#include "m3pi_ng.h"
#include "btbee.h"

m3pi lildicky;
btbee bt;
DigitalOut myled(LED1);
Timer timer;

//controller
float speed = 0.65;
float kp = 0.5;
float kd = 5.5;
float left;
float right;
float e = 0;
float u;
float e_prev;
float e_prev_prev;

int sum1;
int sum2;
int node;
int rawData[5];

    
void play(char music[],int size_music) {
    lildicky.playtune(music, size_music);
}

void seven_nations() {
    //7 nation army first little bit:
    char bar1[] = "V15 L4 MS e. L16 R L8 eg.e.d L2 ML c <b";
    char bar2[] = "L4 MS e. L16 R L8 eg.e.d L6 ML c d c L4 <b.";
    char bar3[] = "L8 MS <g<g<g<g<g<g<g<g";
    char bar4[] = "L8 MS <a<a<a<a<a<a<a<a";
    int one = sizeof(bar1);
    int two = sizeof(bar2);
    int three = sizeof(bar3);
    int four = sizeof(bar4);

    play(bar1, one);
    wait(4.25);
    play(bar1, one);
    wait(4.25);
    play(bar1, one);
    wait(4.25);
    
    play(bar2, two);
    wait(4); 
    
    play(bar3, three);
    wait(2);

    play(bar4, four);
    wait(2);
    
    play(bar1, one);    
}

void followLine(int nodeTurn) {
    timer.start();
    node = 0;

    while(node < nodeTurn){
        // NODE COUNTER
        lildicky.calibrated_sensor(rawData);
        sum1 = rawData[0] + rawData[1] + rawData[2];
        sum2 = rawData[2] + rawData[3] + rawData[4];
        if((sum1 > 2150) or
           (sum2 > 2150) or
           (sum1 + sum2 > 5000)) {
            if(timer.read() > .05){
                timer.reset();
                node++;
                myled = !myled;
            }
        }

        // LINE FOLLOWING
        if(timer.read() <= 0.05) {
            e = 0;
        }
        else {
            e = lildicky.line_position();
        }
        
        u = kp*e + kd*(e-e_prev);
        e_prev = e;

        left = speed + u;
        right = speed - u;

        if (left > 1) {
            left = 1;
        }
        if (right > 1) {
            right = 1;
        }
        if (left < -1) {
            left = -1;
        }
        if (right < -1) {
            right = -1;
        }

        lildicky.right_motor(right);
        lildicky.left_motor(left);
    }
    lildicky.stop();
    wait(0.05);
    timer.stop();
    timer.reset();
}

void turn(int direction) {
    // 0 = right, 1 = left, 2 = 180turn
    if(direction == 0){
        lildicky.right_motor(-0.8);
        lildicky.left_motor(0.8);
        wait(.314/4);
    } else if(direction == 1) {
        lildicky.right_motor(0.8);
        lildicky.left_motor(-0.8);
        wait(.314/4);
    } else {
        lildicky.right_motor(0.8);
        lildicky.left_motor(-0.8);
        wait(.314/2);
    }
    lildicky.stop();
}

void goOne() {
    followLine(3); turn(0);
    followLine(1); turn(1);
    followLine(1); turn(2);

    followLine(1); turn(0);
    followLine(1); turn(1);
    followLine(3); turn(2);
}

void goTwo() {
    followLine(2); turn(0);
    followLine(1); turn(0);
    followLine(1); turn(2);

    followLine(1); turn(1);
    followLine(1); turn(1);
    followLine(2); turn(2);
}

void goFour() {
    followLine(1); turn(1);
    followLine(1); turn(1);
    followLine(1); turn(2);

    followLine(1); turn(0);
    followLine(1); turn(0);
    followLine(1); turn(2);
}

void goFive() {
    followLine(1); turn(1);
    followLine(1); turn(0);
    followLine(2); turn(0);
    followLine(1); turn(1);
    followLine(1); turn(2);

    followLine(1); turn(0);
    followLine(1); turn(1);
    followLine(2); turn(1);
    followLine(1); turn(0);
    followLine(1); turn(2);
}

void goSix() {
    followLine(4); turn(1);
    followLine(1); turn(0);
    followLine(1); turn(2);

    followLine(1); turn(1);
    followLine(1); turn(0);
    followLine(4); turn(2);
}

int main() {
    lildicky.sensor_auto_calibrate();
    char btinput[8] = {0,0,0,0,0,0,0,0};
    int inputAddress;
    lildicky.cls();
    
    while (true) {  
        inputAddress = 0;
        for(int i = 0; i < 8; i++){
            btinput[i] = 0;
        }
        bt.read_all(btinput, 8, &inputAddress); // fetch bt data
        lildicky.cls();
        
        if(inputAddress && btinput[0] == 13){   // if valid data
            for(int i = 1; i < 8; i++){
                int stop = btinput[i];
                if(stop == 1) {
                    goOne();
                } else if(stop == 2) {
                    goTwo();
                } else if(stop == 4) {
                    goFour();
                } else if(stop == 5) {
                    goFive();
                } else if(stop == 6) {
                    goSix();
                } else {
                    seven_nations();
                    break;
                }
            }   
        }
    }
}
