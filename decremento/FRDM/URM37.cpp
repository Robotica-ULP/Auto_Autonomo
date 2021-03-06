// # Editor    :Jiang from DFRobot
// # Data      :18.09.2012

// # Product name:ultrasonic scanner
// # Product SKU:SEN0001
// # Version :  0.2

// # Description:
// # The Sketch for scanning 180 degree area 4-500cm detecting range

// # Connection:
// #       Pin 1 VCC (URM V3.2) -> VCC (Arduino)
// #       Pin 2 GND (URM V3.2) -> GND (Arduino)
// #       Pin 4 PWM (URM V3.2) -> Pin 3 (Arduino)
// #       Pin 6 COMP/TRIG (URM V3.2) -> Pin 5 (Arduino)
// #
#include <mbed.h>
DigitalOut URTRIG(D2); // A low pull on pin COMP/TRIG
AnalogIn URPWM(PTA2);  // Sending Enable PWM mode command
Timer timer;
int begin, end;
unsigned int Distance=0;
uint8_t EnPwmCmd[4]= {0x44,0x02,0xbb,0x01};   // distance measure command

void PWM_Mode_Setup()
{
    URTRIG = 0;                // Set to HIGH
 
    for(int i=0; i<4; i++) {
        Serial.write(EnPwmCmd[i]);
    }
}

void PWM_Mode()
{
    // a low pull on pin COMP/TRIG  triggering a sensor reading
    URTRIG = 1;
    URTRIG = 0;         // reading Pin PWM will output pulses
    int pulse = URPWM;
    if(pulse == 1)
    {
       timer.start(); 
       begin = timer.read_us();
    }
    while(pulse == 1){}
    end = timer.read_us();
    timer.stop();
    unsigned long DistanceMeasured = end - begin;

    if(DistanceMeasured>=10200) {
        // the reading is invalid.
        printf("Invalid \n");
    } else {
        Distance=DistanceMeasured/50;           // every 50us low level stands for 1cm
        printf("Distance= %dcm \n", Distance);
    }

}


int main()
{

    PWM_Mode_Setup();
    while(true) {
        PWM_Mode();
        wait_ms(20);
    }                      //PWM mode setup function
}
