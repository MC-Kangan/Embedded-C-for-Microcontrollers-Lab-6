#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and PWM for DC motor control
void initDCmotorsPWM(int PWMperiod){
	//initialise your TRIS and LAT registers for PWM
    
    TRISEbits.TRISE2=0; //output on RE2
    TRISCbits.TRISC7=0; //output on RC7 
    TRISGbits.TRISG6=0; //output on RG6 
    TRISEbits.TRISE4=0; //output on RE4 
    LATEbits.LATE2=0; // output on RE2 set low (power)
    LATCbits.LATC7=0; // output on RC7 set low (power)
    LATGbits.LATG6=0; // output on RG6 set high (direction: forward) right motor
    LATEbits.LATE4=0; // output on RE4 set high (direction: forward) left  motor

    // timer 2 config
    T2CONbits.CKPS=0b011; // 1:8 prescaler, according to calculation
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=200-1; //Period reg 10kHz base period
    T2CONbits.ON=1;
    
    RE2PPS=0x0A; //set PWM6 on RE2 (power pin), thus RE4 is direction pins for left motor
    RC7PPS=0x0B; //set PMW7 on RC7 (power pin), thus RG6 is direction pins for right motor

    PWM6DCH=0; //0% power (PWM Duty Cycle Register, P436)
    PWM7DCH=0; //0% power (PWM Duty Cycle Register, P436)
    
    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
}


// function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward (When direction channel is high, the power is controlled by PWM off period)
		// low time increases with power
		PWMduty=m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100;
	}
	else { //if reverse (When direction channel is low, the power is controlled by PWM on period)
		// high time increases with power
		PWMduty=((int)(m->power)*(m->PWMperiod))/100;
	}

	*(m->dutyHighByte) = PWMduty; //set high duty cycle byte
        
	if (m->direction){ // if direction is high
		*(m->dir_LAT) = *(m->dir_LAT) | (1<<(m->dir_pin)); // set dir_pin bit in LAT to high without changing other bits
	} else {
		*(m->dir_LAT) = *(m->dir_LAT) & (~(1<<(m->dir_pin))); // set dir_pin bit in LAT to low without changing other bits
	}
}

//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    //if (mL->power > mR->power){ mL->power = mR -> power;}
    //if (mR->power > mL->power){ mR->power = mL -> power;}
    while (mL->power >0 && mR->power >0){
        mL->power -= 10;
        mR->power -= 10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(10);
    }
}

//function to make the robot turn left 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    while (mL->power <TURNING_POWER_L){
        mL->power += 1;
        mR->power = 0;
        setMotorPWM(mL);
        //setMotorPWM(mR);
        __delay_ms(10);
    }
}

//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{
      while (mR->power <TURNING_POWER_R){
        mR->power += 1;
        mL->power = 0;
        setMotorPWM(mR);
        //setMotorPWM(mL);
        __delay_ms(10);
    }
 
}

//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
{
    mL->direction=1;
    mR->direction=1;
    mL->power = 0; // set the power for both motor to be 0 to start with, otherwise, due to previous movement, they might have different speed.
    mR->power = 0;
    while (mL->power<70 && mR->power<70){
        mL->power += 10;
        mR->power += 10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(10);
    }
}

//function to make the robot turn right 180 degree
void turn180(struct DC_motor *mL, struct DC_motor *mR)
{
    while (mR->power <TURNING_POWER_R + 5){
        mR->power += 1;
        mL->power = 0;
        //setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(10);
    }
 
}
