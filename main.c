// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "rc_servo.h"
#include "ADC.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  
      
void main(void){
    Timer0_init();
    Interrupts_init();
    ADC_init();  
    
	//don't forget TRIS for your output!
    TRISDbits.TRISD5 = 0;   //set initial output state
    LATDbits.LATD5 = 0;
    
    int angle = -90;
    char clockwise = 1;
    //unsigned int set_brightness = 2;
    angle2PWM(angle);
    __delay_ms(50);
    while(1){ 
        while (clockwise == 1 && T0CON0bits.T0OUT) {
            angle2PWM(angle++); 
            if (angle == 90) {clockwise=0;} 
            __delay_ms(50); 
            
        } 
        while (clockwise == 0 && T0CON0bits.T0OUT) { 
            angle2PWM(angle--); 
            if (angle == -90) {clockwise=1;}
            __delay_ms(50); 
            
        }
	
        //if (ADC_getval() > set_brightness) {
        //    angle2PWM(90);
        //}
        //else {angle2PWM(-90);}
    }
}
