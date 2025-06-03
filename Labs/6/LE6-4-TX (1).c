/*==================================================
AUTHORS				: 		CRAIG JOSEPH C. GOC-ONG
DESCRIPTION			:		A program that will
							send a character based 
							on the key pressed.
DATE				:		APRIL 17, 2024    
==================================================*/

/*==================================================
HEADER FILE
==================================================*/
#include <xc.h>

/*==================================================
MACROS
==================================================*/
#define _XTAL_FREQ 20000000

/*==================================================
CONFIGURATION BITS
==================================================*/
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

/*==================================================
FUNCTIONS
==================================================*/

// Function to get the keypad value
unsigned int keypadValue()
{
    unsigned int keypad[16] = {0x01,0x02,0x03,0x00,0x04,0x05,0x06,0x00,0x07,0x08,0x09,0x00,0x0D,0x0E,0xF};
    unsigned int input = PORTB & 0x0F;
    return keypad[input];
}
// Function to get the seven segment value
void sevenSegCombo(int key)
{
    unsigned int SSC[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    TXREG = SSC[key];
}

/*==================================================
MAIN FUNCTION
==================================================*/
void main ()
{
    TRISB = 0x1F; // Setting up PORTB

    SPBRG = 0x19; // 9.6K baud rate @ FOSC=4MHz, asynchronous high speed

    SYNC = 0; // asynchronous mode (TXSTA reg)
    SPEN = 1; // enable serial port (RCSTA reg)
    TX9 = 0; // 8-bit transmission (TXSTA reg)
    BRGH = 1; // asynchronous high-speed (TXSTA reg)
    TXEN = 1; // transmit enable (TXSTA reg)
	TXREG = 0x3F;
	  while(1) {

	        unsigned int key = keypadValue(); // Fetching the keypad value

		if(RB4 = 1) //if DATAVBL is pressed
		{
		        if (key != 0x00) //filtering
			{  
		    
		            if (key <= 9) 
				sevenSegCombo(key);
		            
			    if (key == 13)
				TXREG = 0x00;
			    if (key == 14)
				TXREG = 0x3F;
			    if (key == 15)
				TXREG = 0x00; 
		        }
		
		        __delay_ms(40); 
		  }
		}
}
