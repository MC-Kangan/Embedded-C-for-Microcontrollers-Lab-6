// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "rc_servo.h"
#include "ADC.h"
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz 
#define TURNING_POWER_L 70 // This number needs to be adjusted according to different floor condition
#define TURNING_POWER_R 40 // This number needs to be adjusted according to different floor condition
#define TEST 1
// #define TEST 0 
      
void main(void){
    Timer0_init();
    Interrupts_init();
    ADC_init();  
    initDCmotorsPWM(199);
    
    struct DC_motor motorL, motorR;
    
    // For the motor on the left
    motorL.power = 0; // Set power to 0
    motorL.direction = 1; // Set direction to 1 (forward)
    motorL.dutyHighByte = (unsigned char *)(&PWM6DCH); // Store address of PWM duty high byte
    motorL.dir_LAT = (unsigned char *)(&LATE);
    motorL.dir_pin = 4; // With dir_lat above, the direction pin for left motor is E4
    motorL.PWMperiod = 199; // Same as T2PR defined in dc_motor.c
    
    // For the motor on the right
    motorR.power = 0; // Set power to 0
    motorR.direction = 1; // Set direction to 1 (forward)
    motorR.dutyHighByte = (unsigned char *)(&PWM7DCH); // Store address of PWM duty high byte
    motorR.dir_LAT = (unsigned char *)(&LATG);
    motorR.dir_pin = 6; // With dir_lat above, the direction pin for left motor is G6
    motorR.PWMperiod = 199; // Same as T2PR defined in dc_motor.c
   
    __delay_ms(3000);
    while(1){
        if (TEST == 1){
            fullSpeedAhead(&motorL, &motorR);
            __delay_ms(1000);
            stop(&motorL, &motorR);
            __delay_ms(200);
            turnLeft(&motorL, &motorR);
            __delay_ms(1000);
            stop(&motorL, &motorR);
            __delay_ms(200);
            
        }
    
        if (TEST == 0){
            unsigned char i;
            for (i = 0; i < 3; i++){
                fullSpeedAhead(&motorL, &motorR);
                __delay_ms(1000);
                stop(&motorL, &motorR);
                __delay_ms(500);
                turnLeft(&motorL, &motorR);
                __delay_ms(1000);
                stop(&motorL, &motorR);
                __delay_ms(500);
            }

            fullSpeedAhead(&motorL, &motorR);
            __delay_ms(1000);
            stop(&motorL, &motorR);
            __delay_ms(500);
            turn180(&motorL, &motorR);
            __delay_ms(2000);
            stop(&motorL, &motorR);
            __delay_ms(500);

            for (i = 0; i < 3; i++){
                fullSpeedAhead(&motorL, &motorR);
                __delay_ms(1000);
                stop(&motorL, &motorR);
                __delay_ms(500);
                turnRight(&motorL, &motorR);
                __delay_ms(1000);
                stop(&motorL, &motorR);
                __delay_ms(500);
            }

            fullSpeedAhead(&motorL, &motorR);
            __delay_ms(1000);
            stop(&motorL, &motorR);
            __delay_ms(500);
            turn180(&motorL, &motorR);
            __delay_ms(2000);
        }
        
    }
}
