#include <mbed.h>
#include <m3pi_ng.h>
 
m3pi Rob;
Timer timer;
 
// Minimum and maximum motor speeds
float MAX = 1.0;
 
// PID terms
float P = 2.21;
float D = 24.6;
 
//timer vars
int t = 0;
int values[5];
int cal_sum;
 
 
int main()
{
    Rob.sensor_auto_calibrate();
   
    float c_pos = 0.0;
    float p_pos = 0.0;
    float der, prop = 0;
    float power;
    float speed = MAX;
   
    while (1)
    {
        c_pos = Rob.line_position();       
        
        prop = c_pos;
        der = c_pos - p_pos;
 
        power = (prop * P ) + (der * D );
       
        
        if (power > speed)
        {
            power = speed;
        }
       
        if (power < -speed)
        {
            power = -speed;
        }
       
        if (power < 0)
        {
            Rob.left_motor(speed + power);
            Rob.right_motor(speed);
        }
       
        else
        {
            Rob.left_motor(speed);
            Rob.right_motor(speed - power);
        }
       
        p_pos = c_pos;
 
        //***************** Timer ****************//
        Rob.calibrated_sensor(values);
        cal_sum = 0;
        for(int z = 0; z<5; z++)
        {
            cal_sum = cal_sum + values[z];
        }
       
        if (cal_sum >= 4900 && t == 0)
        {
            timer.start();
            t++;
        }
       
        if (t==1)
        {
            Rob.printf("timing");
            t++;
        }
       
        if (cal_sum >= 4900 && timer.read() > 1)
        {
            timer.stop();
            Rob.cls();
            Rob.printf("%.2f", timer.read());
        }
    }
}